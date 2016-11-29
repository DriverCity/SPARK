app.controller('VehicleCtrl', function($scope, $state, $stateParams, localStorageService) {

  $scope.id = $stateParams.id;
  $scope.localVehicles = getItem("vehicles");

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  function getItem(key) {
   return localStorageService.get(key);
  }
  
});