app.controller('SettingsCtrl', function($scope) {

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});