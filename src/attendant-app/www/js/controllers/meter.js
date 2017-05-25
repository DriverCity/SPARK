app.controller('MeterCtrl', function(Firebase, $scope, $state, $ionicModal, $interval, $timeout, $cordovaGeolocation, $firebaseArray, blePerpheralsService, $q) {

//controller for the paid parking context
  /****************************
   * VARIABLES
   ***************************/

  var seconds = 5;//countdown seconds for scanning
  var tempArray = [];//temporary array to save all scanned beacons
  var timeReduction;//countdown function

  $scope.latitude = 0;// current latitude
  $scope.longitude = 0;//current longitude
  $scope.scanning = false;//true if teh scanning is in progress
  $scope.isScanBtnDisabled = false;//if true the scanning button will be disabled
  $scope.mapLoaded = false;

  /****************************
   * POSITIONING
   ***************************/

  $scope.autoRefreshPosition = true;// true if user wants to contantly update the position

//name:changeRefreshPosition
//use: start or stop the automatic position checking
  $scope.changeRefreshPosition = function() {

    if(!$scope.autoRefreshPosition) {
      $scope.startTimerPostion();
    } else {
      $interval.cancel($scope.timerReductionPosition)
    }

    // Invert refresh variable
    $scope.autoRefreshPosition = !$scope.autoRefreshPosition;
  }

//name: startTimerPostion
//use: update the position and show timer for next check.
//Function is called by changeRefreshPosition
  $scope.startTimerPostion = function() {
    var limit = 30; //next update in 30 seconds
    var secondsLeft = limit;
    $scope.positionText = "next check in "+limit+"s";

    $scope.timerReductionPosition = $interval(function() {
      secondsLeft = secondsLeft - 1;
      if(secondsLeft > 0) {
        $scope.positionText = "next check in " + secondsLeft + "s";
      } else {
        //update the position
        $scope.checkCurrentPosition();
        $scope.findArea();
        secondsLeft = limit;
      }
    }, 1000);
  }
  $scope.startTimerPostion();

//name: checkCurrentPosition
//use: check urrent GPS location and call findArea function to check
//if that position is on some parking area
  $scope.checkCurrentPosition = function() {
    console.log("Checking current GPS location");
    var options = {
      timeout: 10000,
      enableHighAccuracy: true
    };

    $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
      $scope.positionGPS = position;
      $scope.positionMAP = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
      $scope.findArea();
    });
  }

  /****************************
   * MAP
   ***************************/

//name: findArea
//use: find the parking area based on users current GPS location
  $scope.findArea = function() {
    console.log("Checking current Area id based on location");
    $scope.parkingArea.$loaded(function() {
      console.log("Parking area loaded from Firebase");
      var newAreaId = null;
      // Verify all the area
      for(var i=0; i<$scope.parkingArea.length; i++) {
        // Retrieve the coordinatess
        var coords = $scope.parkingArea[i].geometry.coordinates[0];
        // Format coordinates
        var parkingCoords = [];

        for(var j=0; j<coords.length; j++) {
          parkingCoords.push(
            {
              lat:parseFloat(coords[j][1]),
              lng:parseFloat(coords[j][0])
            }
          );
        }

        var areaGeometry = new google.maps.Polygon({
          paths: parkingCoords
        });

        // Compare these coordinatess with the current position
        if($scope.positionMAP != undefined && google.maps.geometry.poly.containsLocation($scope.positionMAP, areaGeometry)) {
          newAreaId = $scope.parkingArea[i].area_number;
        }
      }
      //if the coordinates match a parking place the parking area id is updated
      if(newAreaId != null) {
        console.log("Area id : "+newAreaId);
        $scope.currentAreaId = newAreaId;
        console.log("currentAreaId : "+$scope.currentAreaId);
      }
    });
  }

//name:createClickableMap
//use: create the map for the user to choose their location/parkign area
//they are on by clicking the map
  $scope.createClickableMap = function() {
    console.log("call create clickable")

    var options = {
      timeout: 10000,
      enableHighAccuracy: true
    };

    $scope.mapLoaded = true;

    $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
      var positionGoogle = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);

      // Define options
      var mapOptions = {
        center: positionGoogle,
        zoom: 18,
        mapTypeId: google.maps.MapTypeId.ROADMAP
      };

      // Create the map
      $scope.map = new google.maps.Map(document.getElementById("map"), mapOptions);

      // Adding marker current position
      google.maps.event.addListenerOnce($scope.map, 'idle', function(){
        var marker = new google.maps.Marker({
          map: $scope.map,
          animation: google.maps.Animation.DROP,
          position: positionGoogle
        });
      });

      $scope.mapLoaded = false;

      $scope.parkingArea.$loaded(function() {
        for(var i=0; i<$scope.parkingArea.length; i++) {
          var coords = $scope.parkingArea[i].geometry.coordinates[0];
          var parkingCoords = [];

          for(var j=0; j<coords.length; j++) {
            parkingCoords.push(
              {
                lat:parseFloat(coords[j][1]),
                lng:parseFloat(coords[j][0])
              }
            );
          }

          var areaGeometry = new google.maps.Polygon({
            paths: parkingCoords,
            strokeColor: '#FF0000',
            strokeOpacity: 0.8,
            strokeWeight: 2,
            fillColor: '#FF0000',
            fillOpacity: 0.35,
            area_number:$scope.parkingArea[i].area_number
          });
          areaGeometry.setMap($scope.map);

          areaGeometry.addListener('click', function(event) {
            $scope.currentAreaId = this.area_number;
            $scope.modal.hide();
          });
        }
      });
    });
  }

//name: startingInstruction
//use: called by the view to retriveve parking areas from the cloud
  $scope.startingInstruction = function() {
    console.log("start instruction");

    // Retrieve parking Aree
    var parkingAreaRef = firebase.database().ref().child("parkingArea");
    $scope.parkingArea = $firebaseArray(parkingAreaRef);

    // Check current position
    $scope.checkCurrentPosition();
  }

  /****************************
   * CLOUD
   ***************************/
   //name: checkCloudValidity
   //use: retrieves the information related to the scanned beacons from the cloud
   //and checks if the parking evenst are valid
   //parameters: beaconDetectedList: list of beacons scanned
  $scope.checkCloudValidity = function(beaconsDetectedList) {
    var currentTime = new Date();
    // Check availability of mandatory variable

    if($scope.currentAreaId == undefined || $scope.currentAreaId == null) {
      alert("Nothing retrieved, area unkown");
    } else if(beaconsDetectedList == null || beaconsDetectedList.length == 0) {
      alert("Nothing retrieved, beacons' list empty or not defined")
    } else {
      var parkingPaidRef = firebase.database().ref().child("parkingAreaParkingEvent/"+$scope.currentAreaId);
      var arrayRetrieved = $firebaseArray(parkingPaidRef);

      arrayRetrieved.$loaded(function() {
        console.log(arrayRetrieved);

        for(var i=0; i<beaconsDetectedList.length; i++) {
          // Try to find index of beacon in array from firebase
          var index = arrayRetrieved.$indexFor(beaconsDetectedList[i].name);
          if(index == -1) {
            beaconsDetectedList[i].inThisArea = false;
            beaconsDetectedList[i].validity = false;
          } else {
            beaconsDetectedList[i].inThisArea = true;

            var eventTimestamp = arrayRetrieved[index].timestamp;
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

            eventTimePlusDuration = new Date(eventTime.getTime() + arrayRetrieved[index].parkingDurationInMinutes * 60000);

            console.log(eventTime);
            console.log(eventTimePlusDuration);

            // Check validity compared to current time
            if(eventTimePlusDuration > currentTime){
              beaconsDetectedList[i].validity = true; //the parking event is valid
            } else {
              beaconsDetectedList[i].validity = false;//the parking event is not valid
            }
          }
        }
      });
    }
  }

  /****************************
   * BLUETOOTH
   ***************************/

  /*
   * Description:
   *name: onDiscoverDevice
   *use: Add discovered device to tempArray
   *parameters: device: Object returned by BLE plugin including
   *information about discovered device
   */
  var onDiscoverDevice = function(device) {
    if(device.name.indexOf("spark") == 0) {
      device.name = device.name.substring(5, device.name.length);
      tempArray.push(device);
    }
  };

  /*
   * Description:
   *name: setDeviceList
   *use: Set device list and make visible
   */
  var setDeviceList = function(){
    $interval.cancel(timeReduction)//stop the countdown

    // Check validity
    $scope.blePeripherals = tempArray;
    $scope.checkCloudValidity($scope.blePeripherals);

    // Update status
    $scope.scanText = "Scan Complete";
    $scope.scanning = false;
    $scope.isScanBtnDisabled = false;
  }

  /*
   * Description:
   *name: scan
   *use: Scan for all BLE peripheral devices or for devices with a specific service UUID
   */
  $scope.scan = function() {
    $scope.isScanBtnDisabled = true;
    // Initialize variables
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

  // Select beacon
  $scope.selectBeacon = function(name) {
    $scope.beacon = name;
  }

  /**********************************
  * [MODAL] Settings
  *********************************/
//modal for opening the map that user can use to select a position
  $ionicModal.fromTemplateUrl('templates/situation/clickMap.html', {
    scope: $scope,
    animation: 'slide-in-up'
  }).then(function(modal) {
    $scope.modal = modal;
  });

//name: openClickMap
//use: open the map for selecting a position
  $scope.openClickMap = function() {
    $scope.modal.show();
    $scope.createClickableMap();
  };

  $scope.$on('$destroy', function () {
      $scope.modal.remove();
  });

  //Set $scope.map to null
  $scope.$on('modal.hidden', function () {
      $scope.$on('$destroy', function () {
          $scope.map = null;
      });
  });

  /****************************
  * UTILS
  ***************************/
//name: comeBackHome
//use: go back to start view
  $scope.comeBackHome = function() {
    $interval.cancel($scope.timerReductionPosition);
    $state.go("tab.situation");
  };

  $scope.changeState = function(location) {
    $state.go(location);
  };
});
