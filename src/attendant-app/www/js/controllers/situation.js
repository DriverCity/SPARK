app.controller('SituationCtrl', function($scope, $state) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});
