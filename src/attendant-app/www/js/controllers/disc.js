app.controller('DiscCtrl', function(Firebase, $firebaseArray, ionicTimePicker, $scope, $state, $interval, $timeout, blePerpheralsService) {

  /****************************
   * VARIABLES
   ***************************/

  var seconds = 5;      // Countdown seconds for scanning
  var tempArray = [];   // Temporary array to save all scanned beacons
  var timeReduction;    // Countdown function
  var debug = true;

  $scope.scanning = false;            // True if teh scanning is in progress
  $scope.isScanBtnDisabled = false;   // If true the scanning button will be disabled


  // Settings for ionicTimePicker used in selectTimeLimit function
  var timeSettings = {
    callback: function (val) {
      if (typeof (val) === 'undefined') {
        console.log('Time not selected');
      } else {
        $scope.selectedTime = new Date(val * 1000);
        $scope.selectedTimeText = $scope.selectedTime.getUTCHours() + 'h' + ($scope.selectedTime.getUTCMinutes() == "0" ? "" : $scope.selectedTime.getUTCMinutes());
      }
    },
    inputTime: 7200,
    format: 24,
    step: 15,
    setLabel: 'Set'
  };

  // Fake beacons that can be used for testing without actual beacons
/*  
  $scope.blePeripherals = [
    {
      id:"123456789",
      name:"ABC123_00318",
    },
    {
      id:"223456789",
      name:"DEF456_00490",
    }
  ]
*/
  /****************************
   * TIMEPICKER
   ***************************/

  // Function that opens the ionicTimepicker when user wanst to set the time limit
  $scope.selectTimeLimit = function() {
    ionicTimePicker.openTimePicker(timeSettings);
  }

  /****************************
   * CLOUD CHECK
   ***************************/

  /*
   * name: checkCloudValidity
   * use: retrieves the information related to the scanned beacons from the cloud and checks if the parking evenst are valid
   * parameters:
   *   - beaconList: list of beacons scanned
   *   - selectedTimeLimit: time limit the user has selected using ionicTimePicker
   */
  $scope.checkCloudValidity = function(beaconList, selectedTimeLimit) {
    var currentTime = new Date(); //save the current time
    // Check availability of mandatory variable
    if(beaconList == undefined) {
      alert("Please scan before checking");
    } else if(selectedTimeLimit == undefined) {
      alert("Please set up time limit before checking")
    } else {

      // Check each beacon
      angular.forEach(beaconList,function(value,index) {
        var parkingDiscRef = firebase.database().ref().child("parkingAreaParkingEvent/PARKING_DISC_AREA/"+value.name);
        var resultRetrieve = $firebaseArray(parkingDiscRef);

        // Retrieve result for the beacon
        resultRetrieve.$loaded(function() {
          console.log(resultRetrieve);
          if(resultRetrieve.length == 0) {
            for(var i=0; i<beaconList.length; i++) {
              if(beaconList[i].id == value.id) {
                beaconList.splice(i, 1);
              }
            }
          } else {
            var eventTimestamp = resultRetrieve[resultRetrieve.$indexFor("timestamp")].$value;
            // var eventTime = new Date(eventTimestamp); Not working on mobile
            var eventTime = new Date();

            //change the string from cloud to Date
            var splitSpace = eventTimestamp.split(" ");
            var dateSplit = splitSpace[0].split("-");
            var timeSplit = splitSpace[1].split(":");

            eventTime.setUTCFullYear(dateSplit[0]);           // Year
            eventTime.setUTCMonth(parseInt(dateSplit[1])-1);  // Month
            eventTime.setUTCDate(dateSplit[2]);               // Day

            eventTime.setHours(timeSplit[0]);    // Hours for Finland
            eventTime.setMinutes(timeSplit[1]);  // Minutes
            eventTime.setSeconds(timeSplit[2]);  // Seconds

            var selectedTimeInMinutes = selectedTimeLimit.getUTCHours() * 60 + selectedTimeLimit.getUTCMinutes();
            var eventTimePlusLimit = new Date(eventTime.getTime() + selectedTimeInMinutes * 60000);

            // Check validity compared to current time
            value.validity = false; //validity false if not valid
            if(eventTimePlusLimit > currentTime){
              value.validity = true;// validity true if valid
            }
            value.treated = true;
          }

        });
      });
    }
  }

  /****************************
   * BLUETOOTH
   ***************************/

  /*
   * Description:
   * name: onDiscoverDevice
   * use: Add discovered device to tempArray
   * parameters: device: Object returned by BLE plugin including
   * information about discovered device
   */
  var onDiscoverDevice = function(device) {
    if(device.name.indexOf("spark") == 0) {
      var similar = false;
      //check that the same beacon is not added twice
      for(var i=0; i<tempArray.length; i++) {
        if(tempArray[i].id == device.id) {
          similar = true;
          break;
        }
      }
      if(!similar && device.name != null) {
        device.name = device.name.substring(5, device.name.length);
        tempArray.push(device);
      }
    }
  };


  /*
   * Description:
   * name: setDeviceList
   * use: Set device list and make visible
   */
  var setDeviceList = function(){
    $interval.cancel(timeReduction) //stop the countdown

    // Check validity
    $scope.blePeripherals = tempArray;
    $scope.checkCloudValidity($scope.blePeripherals, $scope.selectedTime);

    // Update status
    $scope.scanText = "Scan Complete";
    $scope.scanning = false;
    $scope.isScanBtnDisabled = false;

  }

  /*
   * Description:
   * name: scan
   * use: Scan for all BLE peripheral devices or for devices with a specific service UUID
   */
  $scope.scan = function() {

    if($scope.selectedTime == undefined) {
      alert("Time limit must be set before scanning")
    } else {

      // Initialize variables
      $scope.isScanBtnDisabled = true;
      seconds = 5;
      tempArray = [];
      $scope.scanText = "Scanning (5s left)"; //the text shown as countdwown
      $scope.scanning = true;

      // Scan withouth specifications
      ble.scan([], seconds, onDiscoverDevice, blePerpheralsService.onError);
      // Prepare variables
      var secondsLeft = seconds;

      // Set interval to visibly display the remaining seconds
      var timeReduction = $interval(function() {
        secondsLeft = secondsLeft - 1;
        if(secondsLeft > 0) {
          $scope.scanText = "Scanning (" + secondsLeft + "s left)";
        }
      }, 1000)

      // Set Timeout for applying device info to cards
      $timeout(setDeviceList, (seconds*1000));
    }
  }

  // Select beacon
  $scope.selectBeacon = function(name) {
    $scope.beacon = name;
  }

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };
});
