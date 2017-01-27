app.controller('DiscCtrl', function(Firebase, $firebaseArray, ionicTimePicker, $scope, $state, $interval, $timeout, blePerpheralsService, CloudSrv) {

  /****************************
   * VARIABLES
   ***************************/

  var seconds = 5;
  var tempArray = [];
  var timeReduction;
  var debug = true;

  $scope.scanning = false;
  $scope.isScanBtnDisabled = false;


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

/*
    $scope.blePeripherals = [
    {
      id:"123456789",
      name:"ABC123_00318",
    },
    {
      id:"223456789",
      name:"DEF123_00318",
    }
  ]
*/

  /****************************
   * TIMEPICKER
   ***************************/

  $scope.selectTimeLimit = function() {
    ionicTimePicker.openTimePicker(timeSettings);
  }

  /****************************
   * CLOUD CHECK
   ***************************/

  $scope.checkCloudValidity = function(beaconList, selectedTimeLimit) {
    var currentTime = new Date();
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
          if(resultRetrieve.length == 0) {
            value.validity = false;
            value.treated = true;
          } else {
            var eventTimestamp = resultRetrieve[resultRetrieve.$indexFor("timestamp")].$value;
            // var eventTime = new Date(eventTimestamp); Not working on mobile
            var eventTime = new Date();

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

            // alert(eventTimePlusLimit);
            // alert(currentTime);

            // Check validity
            value.validity = false;
            if(eventTimePlusLimit > currentTime){
              value.validity = true;
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
   * Add discovered device to tempArray
   */
  var onDiscoverDevice = function(device) {
    if(device.name.indexOf("spark") == 0) {
      var similar = false;
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
   * Set device list and make visible
   */
  var setDeviceList = function(){
    $interval.cancel(timeReduction)

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
   * Scan for all BLE peripheral devices or for devices with a specific service UUID
   */
  $scope.scan = function() {

    if($scope.selectedTime == undefined) {
      alert("Time limit must be set before scanning")
    } else {

      // Initialize variables
      $scope.isScanBtnDisabled = true;
      seconds = 5;
      tempArray = [];
      $scope.scanText = "Scanning (5s left)";
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
