app.controller('SettingsCtrl', function(VehicleSrv, $scope, $state, $ionicModal, localStorageService, $interval, $timeout, blePerpheralsService) {

  /****************************
   * VARIABLES
   ***************************/

  $scope.localVehicles = getItem("vehicles");

  var seconds = 5;      // Scan delay
  var tempArray = [];   // Temporary array used to store beacons during the scanning process
  var timeReduction;    // Interval for manage the time during the scanning process

  $scope.scanning = false;            // Boolean representing the scanning status
  $scope.isScanBtnDisabled = false;   // Boolean representing the status of the button in the UI

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

  /*
   * Description : Save new maker value and call models assiociated
   */
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

  /*
   * Description : Save new model value and call trims assiociated
   */
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

  /*
   * Description : Save new trims value and get info assiocated
   */
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

  /*
   * Description : Save the data selected by the user on the local storage of the smartphone
   */
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

  /*
   * Description : Create a modal for add a new car
   */
  $ionicModal.fromTemplateUrl('templates/settings/vehicle.html', {
    scope: $scope,
    animation: 'slide-in-up'
  }).then(function(modal) {
    $scope.modalVehicle = modal;
  });

  /*
   * Description : Open the modal
   */
  $scope.openVehicle = function() {
    $scope.modalVehicle.show();
  };

  /*
   * Description : Close the modal
   */
  $scope.closeVehicle = function() {
    $scope.modalVehicle.hide();
  };

  /****************************
   * UTILS
   ***************************/

  /*
   * Description : Go to another state
   */
  $scope.changeState = function(location) {
    $state.go(location);
  };

  /*
   * Description : Save a value at a key on the local storage
   */
  function submit(key, val) {
   return localStorageService.set(key, val);
  }

  /*
   * Description : Get a value at a key on the local storage
   */
  function getItem(key) {
   return localStorageService.get(key);
  }
});
