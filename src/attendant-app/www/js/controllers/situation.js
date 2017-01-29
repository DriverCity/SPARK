app.controller('SituationCtrl', function($scope, $state) {
//controller for the main view
  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});
