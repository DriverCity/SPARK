app.controller('ParkingCtrl', function($scope, $state) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location, where) {
    $state.go(location, { 'for': where });
  };
  
});