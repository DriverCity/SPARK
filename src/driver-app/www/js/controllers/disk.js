/**********************************
 * [CONTROLLER] DISK
 *********************************/

app.controller('DiskCtrl', function($scope, $state, CloudSrv, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.date = new Date();

  $scope.parkEvent = {
    vehicle:parkCarService.getSelectedVehicle(),
    date:$scope.date.toLocaleDateString(),
    time:$scope.date.toLocaleTimeString(),
  }

  /****************************
   * CLOUD
   ***************************/

  $scope.testCloudRequest = function() {
    CloudSrv.testRequest('smth').then(
      function(ret) {
        $scope.cloudResponse = ret;
      }
    );
  }

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});