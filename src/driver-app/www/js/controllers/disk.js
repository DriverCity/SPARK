/**********************************
 * [CONTROLLER] DISK
 *********************************/

app.controller('DiskCtrl', function($scope, $state) {

  $scope.date = new Date();

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});