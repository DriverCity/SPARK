app.controller('VehicleCtrl', function($scope, $state, $stateParams, localStorageService, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

/*
  $scope.fakeVehicle = {
    "id":"0",
    "make":"aMaker",
    "model":"aModel",
    "beacon":"ABC123_00318"
  }
*/

  $scope.localVehicles = getItem("vehicles");   // Retrieve all the previous vehicles
  $scope.currentVehicle = null;                 // Vehicle selected

  /****************************
   * FUNCTIONS
   ***************************/

  /*
   * Description : Save a vehicle in the parkCarService for a future use
   */
  $scope.selectVechicle = function(aVehicle) {
    parkCarService.setSelectedVehicle(aVehicle);
    $scope.currentVehicle = aVehicle;
  }

  /****************************
   * UTILS
   ***************************/

  /*
   * Description : Go to another state
   */
  $scope.changeState = function(location) {
    $state.go(location);
  };

  /*
   * Description : Redirected user to the right page based on his previous choice on the state "tab.parking"
   */
  $scope.parkCar = function() {
    var location = ($stateParams.for == "meter") ? "tab.meter" : "tab.disk" ;
    $state.go(location);
  };

  /*
   * Description : Get a value at a key on the local storage
   */
  function getItem(key) {
   return localStorageService.get(key);
  }
  
});