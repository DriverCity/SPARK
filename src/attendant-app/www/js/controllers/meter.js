app.controller('MeterCtrl', function(Firebase, $scope, $state, $ionicModal, $interval, $timeout, $cordovaGeolocation, $firebaseArray, blePerpheralsService, CloudSrv, $q) {

  /****************************
   * VARIABLES
   ***************************/

  var seconds = 5;
  var tempArray = [];
  var timeReduction;

  $scope.latitude = 0;
  $scope.longitude = 0;
  $scope.scanning = false;
  $scope.isScanBtnDisabled = false;
  $scope.mapLoaded = false;

  /****************************
   * POSITIONING
   ***************************/

  $scope.autoRefreshPosition = true;

  $scope.changeRefreshPosition = function() {

    if(!$scope.autoRefreshPosition) {
      $scope.startTimerPostion();
    } else {
      $interval.cancel($scope.timerReductionPosition)
    }

    // Invert refresh variable
    $scope.autoRefreshPosition = !$scope.autoRefreshPosition;
  }

  $scope.startTimerPostion = function() {
    var limit = 30;
    var secondsLeft = limit;
    $scope.positionText = "next check in "+limit+"s";

    $scope.timerReductionPosition = $interval(function() {
      secondsLeft = secondsLeft - 1;
      if(secondsLeft > 0) {
        $scope.positionText = "next check in " + secondsLeft + "s";
      } else {
        $scope.checkCurrentPosition();
        $scope.findArea();
        secondsLeft = limit;
      }
    }, 1000);
  }
  $scope.startTimerPostion();

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

  $scope.findArea = function() {
    console.log("Checking current Area id based on location");
    $scope.parkingArea.$loaded(function() {
      console.log("Parking area loaded from Firebase");
      var newAreaId = null;
      // Verify all the area
      for(var i=0; i<$scope.parkingArea.length; i++) {
        // Retrieve the coords
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

        // Compare these coords with the current position
        if($scope.positionMAP != undefined && google.maps.geometry.poly.containsLocation($scope.positionMAP, areaGeometry)) {
          newAreaId = $scope.parkingArea[i].area_number;
        }
      }
      if(newAreaId != null) {
        console.log("Area id : "+newAreaId);
        $scope.currentAreaId = newAreaId;
        console.log("currentAreaId : "+$scope.currentAreaId);
      }
    });
  }

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

            if(eventTimePlusDuration > currentTime){
              beaconsDetectedList[i].validity = true;
            } else {
              beaconsDetectedList[i].validity = false;
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
   * Add discovered device to tempArray
   */
  var onDiscoverDevice = function(device) {
    if(device.name.indexOf("spark") == 0) {
      device.name = device.name.substring(5, device.name.length);
      tempArray.push(device);
    }
  };

  /*
   * Description: Set device list and make visible
   */
  var setDeviceList = function(){
    $interval.cancel(timeReduction)

    // Check validity
    $scope.blePeripherals = tempArray;
    $scope.checkCloudValidity($scope.blePeripherals);

    // Update status
    $scope.scanText = "Scan Complete";
    $scope.scanning = false;
    $scope.isScanBtnDisabled = false;
  }

  /*
   * Description: Scan for all BLE peripheral devices or for devices with a specific service UUID
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

  $ionicModal.fromTemplateUrl('templates/situation/clickMap.html', {
    scope: $scope,
    animation: 'slide-in-up'
  }).then(function(modal) {
    $scope.modal = modal;
  });

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

  $scope.comeBackHome = function() {
    $interval.cancel($scope.timerReductionPosition);
    $state.go("tab.situation");
  };

  $scope.changeState = function(location) {
    $state.go(location);
  };
});
