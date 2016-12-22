app.controller('MeterCtrl', function(Firebase, $scope, $state, $ionicModal, $interval, $timeout, $cordovaGeolocation, $firebaseArray, blePerpheralsService, CloudSrv, $q) {

  /****************************
   * VARIABLES
   ***************************/

  var seconds = 5;
  var tempArray = [];
  var timeReduction;

  $scope.latitude= 0;
  $scope.longitude= 0;
  $scope.scanning = false;
  $scope.isScanBtnDisabled = false;

  $scope.currentAreaId = null;

  /****************************
   * TEST THINGS
   ***************************/

  $scope.fakeBeacons = [
    {
      id:"123456789",
      name:"ABC-123"
    },
    {
      id:"223456789",
      name:"DEF-123"
    }
  ]

  // Fake position inside an area
  var position = {
    coords: {
      latitude: 61.4991694,
      longitude: 23.7600491
    }
  }

  $scope.fakeAreaId = 777;

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
    var limit = 10;
    var secondsLeft = limit;
    $scope.positionText = "position checked ("+limit+"s left)";

    $scope.timerReductionPosition = $interval(function() {
      secondsLeft = secondsLeft - 1;
      if(secondsLeft > 0) {
        $scope.positionText = "position checked (" + secondsLeft + "s left)";
      } else {
        $scope.checkCurrentPosition();
        $scope.findArea();
        secondsLeft = limit;
      }
    }, 1000);
  }
  $scope.startTimerPostion();

  $scope.checkCurrentPosition = function() {
    console.log("checking");
    var options = {
      timeout: 10000,
      enableHighAccuracy: true
    };
    
    // $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
      $scope.positionGPS = position;
      $scope.positionMAP = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
    // }
  }

  /****************************
   * MAP
   ***************************/

  $scope.findArea = function() {
    $scope.parkingArea.$loaded(function() {
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
      $scope.currentAreaId = newAreaId;
    });
  }

  $scope.createClickableMap = function() {
    // Define options
    var mapOptions = {
      center: $scope.positionMAP,
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
        position: $scope.positionMAP
      });
    });

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
          $scope.closeClickMap();
        });
      }
    });
  }

  $scope.retrieveParkingArea = function() {
    var parkingAreaRef = firebase.database().ref().child("parkingArea");
    $scope.parkingArea = $firebaseArray(parkingAreaRef);
    $scope.parkingArea.$loaded(function() {
      $scope.findArea();
    });
  }

  // When the controller is loaded
  $scope.checkCurrentPosition();
  $scope.retrieveParkingArea();
  
  $scope.parkingAreaLoaded = false;
  $scope.parkingArea.$loaded(function() {
    $scope.parkingAreaLoaded = true;
  });

  /****************************
   * CLOUD
   ***************************/

  $scope.checkBeaconsValidity = function(array) {
    var currentTime = Date.now();
    // Iterate for each beacon retrieve
    for(var i=0; i<array.length; i++) {
      requestFirebaseVerification(i, array[i]).then(function(data) {
        var beacon = data.beacon;
        var lastParkEvent = data.lastParkEvent;
        
        // Compare
        var eventTimestamp = lastParkEvent.timestamp;
        var parsedTime = Date.parse(eventTimestamp);
        var duration = lastParkEvent.parkingDurationInMinutes * 60 * 1000;

        var validity = false;
        if(parsedTime + duration > currentTime){
          validity = true;
        }
        array[data.id].validity = validity;
      });
    }
  }

  $scope.checkBeaconsValidity($scope.fakeBeacons);

  function requestFirebaseVerification(id, beacon) {
    var defer = $q.defer();

    var beaconRef = firebase.database().ref().child("parkingAreaParkingEvent/" + $scope.fakeAreaId + "/" + beacon.name);
    var query = beaconRef.orderByChild("timestamp").limitToLast(1);
    $firebaseArray(query).$loaded(function(lastParkEvent) {
      defer.resolve({
        id:id,
        beacon:beacon,
        lastParkEvent:lastParkEvent
      });
    });

    return defer.promise;
  }

  /****************************
   * BLUETOOTH
   ***************************/

  /*
   * Description: Add discovered device to tempArray
   */
  var onDiscoverDevice = function(device) {
    console.log(JSON.stringify(device));
    // Add device to array
    tempArray.push(device);
  };

  /*
   * Description: Set device list and make visible
   */
  var setDeviceList = function(){
    $scope.scanText = "Scan Complete";
    $interval.cancel(timeReduction)
    $scope.blePeripherals = tempArray;
    $scope.scanning = false;
    //  $scope.checkBeaconsValidity($scope.blePeripherals);
  }

  /*
   * Description: Scan for all BLE peripheral devices or for devices with a specific service UUID
   */
  $scope.scan = function() {

    // Initialize variables
    seconds = 5;
    tempArray = [];
    $scope.scanText = "Scanning (5s left)";
    $scope.scanning = true;

    // Scan withouth specifications
    ble.scan([], seconds, onDiscoverDevice, blePerpheralsService.onError);
    // Prepare variables
    $scope.isScanBtnDisabled = true;
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
    $scope.modalSettigs = modal;
  });

  $scope.openClickMap = function() {
    $scope.modalSettigs.show();
    $scope.createClickableMap();
  };

  $scope.closeClickMap = function() {
    $scope.modalSettigs.hide();
  };

  /****************************
  * UTILS
  ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };
});
