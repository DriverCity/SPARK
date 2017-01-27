app.controller('SettingsCtrl', function(VehicleSrv, $scope, $state, $ionicModal, localStorageService, $interval, $timeout, blePerpheralsService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.localVehicles = getItem("vehicles");

  var seconds = 5;
  var tempArray = [];
  var timeReduction;

  $scope.scanning = false;
  $scope.isScanBtnDisabled = false;

  /****************************
   * BLUETOOTH
   ***************************/

  /*
   * Description: Add discovered device to tempArray
   */
  var onDiscoverDevice = function(device) {
    if(device.name.indexOf("spark") == 0) {
      var similar = false;
      for(var i=0; i<tempArray.length; i++) {
        if(tempArray[i].id == device.id) {
          similar = true;
          break;
        }
      }
      if(!similar && device.name != null) {
        device.name = device.name.substring(5, device.name.length);
        tempArray.push(device);
      }
    }
  };

  /*
   * Description: Set device list and make visible
   */
  var setDeviceList = function(){
    $scope.scanText = "Scan Complete";
    $interval.cancel(timeReduction)
    $scope.blePeripherals = tempArray;
    $scope.scanning = false;
    $scope.isScanBtnDisabled = false;
  }

  /*
   * Description: Scan for all BLE peripheral devices or for devices with a specific service UUID
   */
  $scope.scan = function() {

    // Initialize variables
    $scope.isScanBtnDisabled = true;
    seconds = 5;
    tempArray = [];
    $scope.scanText = "Scanning (5s left)";
    $scope.scanning = true;

    // Scan withouth specifications
    ble.scan([], seconds, onDiscoverDevice, blePerpheralsService.onError);

    // Prepare variables
    var secondsLeft = seconds;

    // Set interval to visibly display the remaining seconds
    var timeReduction = $interval(function() {
      secondsLeft = secondsLeft - 1;
      if(secondsLeft > 0) {
        $scope.scanText = "Scanning (" + secondsLeft + "s left)";
      }
    }, 1000)

    // Set Timeout for applying device info to cards
    $timeout(setDeviceList, (seconds*1000));
  }

  // Select beacon
  $scope.selectBeacon = function(name) {
    $scope.beacon = name;
  }

  /****************************
   * VEHICLE DATA
   ***************************/

  /*
   * Description : Call get makes function from Vehicle service
   */
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

  /****************************
   * VALIDATION PROCESS
   ***************************/

  $scope.validationData = function(make, model, id, beacon) {
    // Local storage
    if(localStorageService.isSupported) {
      // Create vehicle
      var vehicle = {
        id:id,
        make:make,
        model:model,
        beacon:beacon
      };

      // Retrieve and update list of vehicles
      var vehicles = getItem("vehicles");
      if(vehicles == null || vehicles == undefined) {
        vehicles = [];
      }
      vehicles.push(vehicle);
      submit("vehicles",vehicles);

      // Update view
      $scope.localVehicles = getItem("vehicles");
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
