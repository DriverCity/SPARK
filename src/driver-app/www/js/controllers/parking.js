app.controller('ParkingCtrl', function($scope, $state) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };
  
});