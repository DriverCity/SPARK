app.controller('SettingsCtrl', function(VehicleSrv, $scope, $state, $ionicModal, localStorageService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.currentData = getItem("vehicle");

  /****************************
   * VEHICLE DATA
   ***************************/

  VehicleSrv.getMakes().then(
    function(ret) {
      $scope.makes = ret.Makes;
    }
  );

  $scope.inputMakeChange = function(newValue, oldValue) {
    $scope.models = $scope.trims = $scope.infos = null;
    if(newValue != "" && newValue != null) {
      VehicleSrv.getModels(newValue).then(
        function(ret) {
          $scope.models = ret.Models;
        }
      );
    }
  };

  $scope.inputModelChange = function(maker, newValue, oldValue) {
    $scope.trims = $scope.infos = null;
    if(newValue != "" && newValue != null) {
      VehicleSrv.getTrims(maker, newValue).then(
        function(ret) {
          $scope.trims = ret.Trims;
        }
      );
    }
  };

  $scope.inputTrimChange = function(newValue, oldValue) {
    $scope.infos = null;
    if(newValue != "" && newValue != null) {
      VehicleSrv.getInfos(newValue).then(
        function(ret) {
          $scope.infos = ret[0];
        }
      );
    }
  };

  $scope.validationData = function(make, model, id) {
    // Local storage
    if(localStorageService.isSupported) {
      var vehicle = {
        make:make,
        model:model,
        id:id
      };
      submit("vehicle",vehicle);
      $scope.currentData = vehicle;
      $scope.closeVehicle();
    }
  }

  /**********************************
   * [MODAL] VEHICLE
   *********************************/

  $ionicModal.fromTemplateUrl('templates/settings/vehicle.html', {
    scope: $scope,
    animation: 'slide-in-up'
  }).then(function(modal) {
    $scope.modalVehicle = modal;
  });

  $scope.openVehicle = function() {
    $scope.modalVehicle.show();
  };

  $scope.closeVehicle = function() {
    $scope.modalVehicle.hide();
  };

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  function submit(key, val) {
   return localStorageService.set(key, val);
  }

  function getItem(key) {
   return localStorageService.get(key);
  }
});