angular.module('starter', ['ionic', 'starter.controllers', 'starter.services'])

.run(function($ionicPlatform) {
  $ionicPlatform.ready(function() {
    // Hide the accessory bar by default (remove this to show the accessory bar above the keyboard
    // for form inputs)
    if (window.cordova && window.cordova.plugins && window.cordova.plugins.Keyboard) {
      cordova.plugins.Keyboard.hideKeyboardAccessoryBar(false);
      cordova.plugins.Keyboard.disableScroll(true);
    }
    if (window.StatusBar) {
      // org.apache.cordova.statusbar required
      StatusBar.styleDefault();
    }
  });
})

.config(function (localStorageServiceProvider) {
  localStorageServiceProvider
    .setNotify(true, true);
})

.config(function($stateProvider, $urlRouterProvider) {

  /****************************
   * TAB CONFIGURATION
   ***************************/

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
   * PARKING
   ***************************/

  .state('tab.parking', {
    url: '/parking',
    views: {
      'tab-parking': {
        templateUrl: 'templates/parking/tab-parking.html',
        controller: 'ParkingCtrl'
      }
    }
  })

    .state('tab.disk', {
      url: '/disk',
      views: {
        'tab-parking': {
          templateUrl: 'templates/parking/tab-disk.html',
          controller: 'DiskCtrl'
        }
      }
    })

  /****************************
   * SETTINGS
   ***************************/

  .state('tab.settings', {
    url: '/settings',
    views: {
      'tab-settings': {
        templateUrl: 'templates/settings/tab-settings.html',
        controller: 'SettingsCtrl'
      }
    }
  })

    .state('tab.vehicle', {
      url: '/vehicle',
      views: {
        'tab-settings': {
          templateUrl: 'templates/settings/tab-vehicle.html',
          controller: 'VehicleCtrl'
        }
      }
    })

  /****************************
   * DEFAULT ROUTE
   ***************************/

  $urlRouterProvider.otherwise('/tab/parking');

});
