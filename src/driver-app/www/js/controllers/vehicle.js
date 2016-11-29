app.controller('VehicleCtrl', function($scope, $state, $stateParams, localStorageService, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.localVehicles = getItem("vehicles");
  $scope.currentVehicle = null;

  /****************************
   * FUNCTIONS
   ***************************/

  $scope.selectVechicle = function(aVehicle) {
    parkCarService.setSelectedVehicle(aVehicle);
    $scope.currentVehicle = aVehicle;
  }

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  $scope.parkCar = function() {
    var location = ($stateParams.for == "meter") ? "tab.meter" : "tab.disk" ;
    $state.go(location);
  };

  function getItem(key) {
   return localStorageService.get(key);
  }
  
});