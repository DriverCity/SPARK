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

  /* Type : Variable
   * Usage : Area displayed as overlay on the Google Map
   */
  var areaDisplayed = [];

  /* Type : Variables
   * Usage : Colors scale used for parking areas displayed
   */
  var unknow = "#a2a2a2", low = "#0fe067", medium = "#ff9933", lot = "#e03a0f";

  /* Type : Function
   * Usage : Return the color based on the occupancy rate received in parameter
   */
  $scope.defineColor = function(occupancyRate) {
    if(occupancyRate == "UNKNOWN" || occupancyRate == null) return unknow;
    if(parseFloat(occupancyRate) < 0.3) return low;
    if(parseFloat(occupancyRate) > 0.7) return lot;
    return medium;
  };

  /* Type : Function
   * Usage : Remove a specific area passed in paramter from the array of displayed areas
   */
  $scope.removeFromMap = function(area) {
    console.log("removed");
    for(var i=0; i<areaDisplayed.length; i++) {
      if(areaDisplayed[i].area_number == area) {
        areaDisplayed[i].setMap(null);
        areaDisplayed.splice(i, 1);
      }
    }
  }

  /* Type : Function
   * Paramter : JSON array extracted from the cloud reponses (child)
   * Usage : Important information are extracted from the child to create an area added to the areas displayed array and set on the map
   */
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

  /* Type : Function
   * Usage : Retrieve parking areas from Firebase and define the action in case of 'add' / 'update' events
   */
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

  /* Type : Variable
   * Usage : Option used for the geolocation
   */
  var options = {
    timeout: 10000,
    enableHighAccuracy: true
  };

  /* Type : Variable
   * Usage : Boolean used for the loading spinner
   */
  $scope.located = false;
 
  /* Type : Plug-in call
   * Usage : Start the geolocation
   */
  $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
  
    /* Position rightly formatted for Google Map API based on the position retrieved by the plug-in */
    var LatLng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);    

    /* Options for the Google Map */
    var mapOptions = {
      center: LatLng,
      zoom: 15,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };

    /* Create the map */
    $scope.map = new google.maps.Map(document.getElementById("map"), mapOptions);

    /* Adding marker at the current position */
    google.maps.event.addListenerOnce($scope.map, 'idle', function(){
      var marker = new google.maps.Marker({
        map: $scope.map,
        animation: google.maps.Animation.DROP,
        position: LatLng
      });
    });

    /* Update location status */
    $scope.located = true;

    /* Add areas */
    $scope.createOverlay();
  
  }, function(error){
    alert("Unexpected error, could not get location ...");
  });

  /****************************
   * UTILS
   ***************************/

  /* Type : Function
   * Usage : Go to another state of the application
   */
  $scope.changeState = function(location) {
    $state.go(location);
  };
  
});