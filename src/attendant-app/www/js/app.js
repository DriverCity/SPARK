angular.module('starter', ['ionic', 'starter.controllers', 'starter.services'])

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {

    if (window.cordova && window.cordova.plugins && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(true);
      cordova.plugins.Keyboard.disableScroll(true);
    }
    if (window.StatusBar) {
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})

.config(function($stateProvider, $urlRouterProvider) {

  $stateProvider

  .state('tab', {
    url: '/tab',
    abstract: true,
    templateUrl: 'templates/tabs.html'
  })

  /****************************
   * MAP
   ***************************/

  .state('tab.map', {
    url: '/map',
    views: {
      'tab-map': {
        templateUrl: 'templates/map/tab-map.html',
        controller: 'MapCtrl'
      }
    }
  })

  /****************************
   * SITUATION
   ***************************/

  .state('tab.situation', {
    url: '/situation',
    views: {
      'tab-situation': {
        templateUrl: 'templates/situation/tab-situation.html',
        controller: 'SituationCtrl'
      }
    }
  })

//view for disk parking context
    .state('tab.disc', {
      url: '/disc',
      views: {
        'tab-situation': {
          templateUrl: 'templates/situation/tab-disc.html',
          controller: 'DiscCtrl'
        }
      }
    })

//view for paid parking context
    .state('tab.meter', {
      url: '/meter',
      views: {
        'tab-situation': {
          templateUrl: 'templates/situation/tab-meter.html',
          controller: 'MeterCtrl'
        }
      }
    })

  // if none of the above states are matched, use this as the fallback
  $urlRouterProvider.otherwise('/tab/situation');

});
