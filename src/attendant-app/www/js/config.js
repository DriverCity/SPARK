var app = angular.module('starter.controllers', ['ionic','firebase','ngCordova','ionic-timepicker']);

app.config(function (ionicTimePickerProvider) {
  var timePickerObj = {
    inputTime: (((new Date()).getHours() * 60 * 60) + ((new Date()).getMinutes() * 60)),
    format: 12,
    step: 15,
    setLabel: 'Set',
    closeLabel: 'Close'
  };
  ionicTimePickerProvider.configTimePicker(timePickerObj);
});

/**********************************
 * [DIRECTIVE] TEXTAREA
 *********************************/

app.directive('textarea', function() {
  return {
    restrict: 'E',
    link: function(scope, element, attr) {
      var update = function() {
        element.css("height", "auto");
        var height = element[0].scrollHeight;
        if(height != 0) {
          element.css("height", element[0].scrollHeight + "px");
        }
      };
      scope.$watch(attr.ngModel, function() {
        update();
      });
    }
  };
});