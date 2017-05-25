/**********************************
 * [CONTROLLER] DISK
 *********************************/

app.controller('DiskCtrl', function($scope, $state, CloudSrv, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

  /* Type : Variable
   * Usage : Current date
   */
  $scope.date = new Date();

  /* Type : Variable
   * Usage : Park event rightly formatted for the cloud based on the Vehicle selected in the previous state and saved in parkCarService
   */
  $scope.parkEvent = {
    parkingAreaId: 0,
    parkingContextType: "PARKING_DISC",
    registerNumber: parkCarService.getSelectedVehicle().beacon
  }

  /****************************
   * CLOUD
   ***************************/

  /* Type : Variable
   * Usage : Send the event formatted to the Cloud service and wait for the response
   */
  CloudSrv.parkEventRequest($scope.parkEvent).then(
    function(ret) {
      // Success
      $scope.cloudResponse = true;
    }
  ).catch(function(response) {
    // Error during the process
    $scope.cloudResponse = false;
    $scope.cloudError = response;
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