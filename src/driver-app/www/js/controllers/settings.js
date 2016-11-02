app.controller('SettingsCtrl', function(VehicleSrv, $scope, $state, localStorageService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.currentData = getItem("vehicle");

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  function submit(key, val) {
   return localStorageService.set(key, val);
  }

  function getItem(key) {
   return localStorageService.get(key);
  }
});