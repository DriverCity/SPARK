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

    if(!window.ble) {
      if(navigator.notification) {
        navigator.notification.alert(
          'The Bluetooth Low Energy (BLE) Central Plugin [com.megster.cordova.ble] is missing.',  // message
          alertDismissed,         // callback
          'Plugin Not Found',     // title
          'Ok'                    // buttonName
        );
      }
    } else {
      ble.isEnabled(
        function() {  /* Bluetooth enabled */  },
        function() {  /* Bluetooth not yet enabled */
          ble.enable(
            function(){ /* bluetooth now enabled */ },
            function(err){
              alert('Cannot enable bluetooth');
            }
          );
        }
      );
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

    .state('tab.meter', {
      url: '/meter',
      views: {
        'tab-parking': {
          templateUrl: 'templates/parking/tab-meter.html',
          controller: 'MeterCtrl'
        }
      }
    })

      .state('tab.device', {
        url: '/device/:id',
        views: {
          'tab-parking': {
            templateUrl: 'templates/parking/tab-device.html',
            controller: 'DeviceCtrl'
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

  /****************************
   * DEFAULT ROUTE
   ***************************/

  $urlRouterProvider.otherwise('/tab/parking');

});
