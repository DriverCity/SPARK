/**********************************
 * [CONTROLLER] MAP
 *********************************/

app.controller('MapCtrl', function(Firebase, $scope, $state, $cordovaGeolocation, $firebaseArray) {
  // Geolocation options
  var options = {
    timeout: 10000,
    enableHighAccuracy: true
  };
  $scope.located = false;
 
  $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
 
    // Position coordinates
    var latLng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);
 
    // Define options
    var mapOptions = {
      center: latLng,
      zoom: 15,
      mapTypeId: google.maps.MapTypeId.ROADMAP
    };
 
    // Create the map
    $scope.map = new google.maps.Map(document.getElementById("map"), mapOptions);
    $scope.located = true;

    // Adding marker current position
    google.maps.event.addListenerOnce($scope.map, 'idle', function(){
      var marker = new google.maps.Marker({
        map: $scope.map,
        animation: google.maps.Animation.DROP,
        position: latLng
      });
    });

    // Reference of parking open data in Firebase
    var featureRef = firebase.database().ref().child('features');
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
        parkingGeometry.setMap($scope.map);
      }
    });
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