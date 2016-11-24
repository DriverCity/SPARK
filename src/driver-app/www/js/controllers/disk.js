/**********************************
 * [CONTROLLER] DISK
 *********************************/

app.controller('DiskCtrl', function($scope, $state, CloudSrv) {

  $scope.date = new Date();

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