var app = angular.module('starter.controllers', ['ionic','firebase','ngCordova','LocalStorageModule']);

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

/**********************************
 * [DIRECTIVE] SELECT ISSUES KEYBOARD
 *********************************/

app.directive('select', function() {
  return {
    restrict: 'E',
    link: function(scope, element, attrs) {
      element.on('touchstart focus', function(e) {
        if (window.cordova && window.cordova.plugins.Keyboard) {
          cordova.plugins.Keyboard.hideKeyboardAccessoryBar(false);
        }
      });
      element.bind('blur', function(e) {
        if (window.cordova && window.cordova.plugins.Keyboard) {
          cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
        }
      });
    }
  };
});