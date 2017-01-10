/**********************************
 * [CONTROLLER] DISK
 *********************************/

app.controller('DiskCtrl', function($scope, $state, CloudSrv, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.date = new Date();

  $scope.parkEvent = {
    parkingAreaId: 0,
    parkingContextType: "PARKING_DISC",
    registerNumber: parkCarService.getSelectedVehicle().beacon
  }

  /****************************
   * CLOUD
   ***************************/

  CloudSrv.parkEventRequest($scope.parkEvent).then(
    function(ret) {
      $scope.cloudResponse = true;
    }
  ).catch(function(response) {
    $scope.cloudResponse = false;
    $scope.cloudError = response;
  });

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});