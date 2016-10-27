app.controller('DiskCtrl', function($scope, $state) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});