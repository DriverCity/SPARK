/**********************************
 * [CONTROLLER] MAP
 *********************************/

app.controller('MapCtrl', function(Firebase, $scope, $state, $cordovaGeolocation, $firebaseArray) {
  
  /**********************
   * VARIABLES
   *********************/

  $scope.map = null;

  /**********************
   * OVERLAY AREA
   *********************/

  var areaDisplayed = [];
  var unknow = "#a2a2a2", low = "#0fe067", medium = "#ff9933", lot = "#e03a0f";

  $scope.defineColor = function(occupancyRate) {
    if(occupancyRate == "UNKNOWN" || occupancyRate == null) return unknow;
    if(parseFloat(occupancyRate) < 0.3) return low;
    if(parseFloat(occupancyRate) > 0.7) return lot;
    return medium;
  };
  
  $scope.removeFromMap = function(area) {
    console.log("removed");
    for(var i=0; i<areaDisplayed.length; i++) {
      if(areaDisplayed[i].area_number == area) {
        areaDisplayed[i].setMap(null);
        areaDisplayed.splice(i, 1);
      }
    }
  }

  $scope.addOnMap = function(child) {
    console.log("added");
    var featureCoords = child.geometry.coordinates[0];
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
      strokeColor: $scope.defineColor(child.occupancyRate),
      strokeOpacity: 0.9,
      strokeWeight: 2,
      fillColor: $scope.defineColor(child.occupancyRate),
      fillOpacity: 0.35,
      area_number: child.area_number
    });
    // Set map
    parkingGeometry.setMap($scope.map);
    areaDisplayed.push(parkingGeometry);
  }


  $scope.createOverlay = function() {
    // Retrieve areas
    var parkingAreaRef = firebase.database().ref().child('parkingArea');
    $scope.parkingArea = $firebaseArray(parkingAreaRef);

    $scope.parkingArea.$watch(function(e) {
      // New child
      if(e.event == "child_added") {
        $scope.addOnMap($scope.parkingArea.$getRecord(e.key));
      }
      // Updated child
      else if(e.event == "child_changed") {
        console.log("changed");
        // Remove his last state
        $scope.removeFromMap(e.key);
        // Add the new value
        $scope.addOnMap($scope.parkingArea.$getRecord(e.key));
      }
    });
  }


  /**********************
   * GEOLOCATION
   *********************/

  var options = {
    timeout: 10000,
    enableHighAccuracy: true
  };

  $scope.located = false;
 
  $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
  
    var LatLng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);    

    var mapOptions = {
      center: LatLng,
      zoom: 15,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    // Create the map
    $scope.map = new google.maps.Map(document.getElementById("map"), mapOptions);

    // Adding marker current position
    google.maps.event.addListenerOnce($scope.map, 'idle', function(){
      var marker = new google.maps.Marker({
        map: $scope.map,
        animation: google.maps.Animation.DROP,
        position: LatLng
      });
    });

    // Update location status
    $scope.located = true;

    // Add area
    $scope.createOverlay();
  
  }, function(error){
    alert("Unexpected error, could not get location ...");
  });

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };
  
});