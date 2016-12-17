app.controller('MeterCtrl', function(Firebase, $scope, $state, $ionicModal, $interval, $timeout, $cordovaGeolocation, $firebaseArray, blePerpheralsService, CloudSrv) {

/*
  var options = {
    timeout: 10000,
    enableHighAccuracy: true
  };


  $cordovaGeolocation.getCurrentPosition(options).then(function(position) {

    // Position coordinates
    var latLng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
    $scope.inParkingArea= "not found";

    var featureRef = firebase.database().ref().child('parkings');
    // Real-time array for data reference
    $scope.features = $firebaseArray(featureRef);
    $scope.features.$watch(function(e) {
      if(e.event == "child_added") {
        // Extract added feature
        var feature = $scope.features[$scope.features.length-1];
        var featureCoords = feature.geometry.coordinates[0];

        // Convert coords to Google Map format
        var parkingCoords = [];
        for(var i=0;i<featureCoords.length;i++) {
          parkingCoords.push(
            {
              lat:parseFloat(featureCoords[i][1]),
              lng:parseFloat(featureCoords[i][0])
            }
          );
        }

        // Adding it on the map
        var parkingGeometry = new google.maps.Polygon({
          paths: parkingCoords,
          strokeColor: '#FF0000',
          strokeOpacity: 0.8,
          strokeWeight: 2,
          fillColor: '#FF0000',
          fillOpacity: 0.35
        });

        if(google.maps.geometry.poly.containsLocation(latLng, parkingGeometry) == true) {
          console.log(e);
            $scope.inParkingArea = e.key;
        } else {
          console.log("no ok");
        }

      }
    });
  });
*/

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

  /****************************
  * TEST THINGS
  ***************************/

  $scope.fakeBeacons = [
    {
      id:"123456789",
      name:"ABC-123"
    }
  ]

  $scope.fakeAreaId = 777;

  /****************************
   * CLOUD
   ***************************/

  $scope.checkBeaconsValidity = function(array) {
    for(var i=0;i<array.length;i++) {
      var beaconRef = firebase.database().ref().child("parkingAreaParkingEvent/" + $scope.fakeAreaId + "/" + array[i].name);
      var query = beaconRef.orderByChild("timestamp").limitToLast(25);
      $scope.parkEventsOrdered = $firebaseArray(query);
    }
  }

  $scope.checkBeaconsValidity($scope.fakeBeacons);
 
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


  /****************************
  * UTILS
  ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});
