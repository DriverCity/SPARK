app.controller('DiskCtrl', function($scope, $state, $stateParams) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});
