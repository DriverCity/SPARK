/**********************************
 * [CONTROLLER] DEVICE
 *********************************/

app.controller('DeviceCtrl', function($scope, $state, blePerpheralsService, parkCarService) {

  /****************************
   * VARIABLES
   ***************************/

  // Pre defined UUID for Connexion service
  blePerpheralsService.setServiceId('ec00');
  blePerpheralsService.setCharacteristicId('ec0e');

/*
  $scope.meterInfo = {
    price_per_hour:NaN,
    resolution:5,
    limit:240
  }
*/

  $scope.timeValidity = null;

  $scope.timeSelected = 0;
  $scope.price = 0;
  // $scope.timeSelected = $scope.meterInfo.resolution;
  // $scope.price = $scope.meterInfo.price_per_hour;

  /****************************
   * PRICE INPUT
   ***************************/

  $scope.increaseTime = function() {
    if($scope.meterInfo != null) {
      if($scope.meterInfo.limit == 0 || $scope.meterInfo.limit != 0 && $scope.timeSelected != $scope.meterInfo.limit ) {
        $scope.timeSelected += $scope.meterInfo.resolution;
        $scope.price = $scope.meterInfo.price_per_hour / 60 * $scope.timeSelected;
        $scope.price = parseFloat($scope.price.toFixed(2));
      }
    }
  }

  $scope.decreaseTime = function() {
    if($scope.meterInfo != null) {
      if($scope.timeSelected != $scope.meterInfo.resolution) {
        $scope.timeSelected -= $scope.meterInfo.resolution;
        $scope.price = $scope.meterInfo.price_per_hour / 60 * $scope.timeSelected;
        $scope.price = parseFloat($scope.price.toFixed(2));
      }
    }
  }

  /****************************
   * DATA CONVERTION
   ***************************/

  /* Description:
   * Convert String to ArrayBuffer 
   */
  var stringToBytes = function(string) {
    var array = new Uint8Array(string.length);
    for (var i = 0, l = string.length; i < l; i++) {
      array[i] = string.charCodeAt(i);
    }
    return array.buffer;
  }

  /* Description:
   * Convert ArrayBuffer to String
   */
  var bytesToString = function(buffer) {
    return String.fromCharCode.apply(null, new Uint8Array(buffer));
  }

  /****************************
   * READ DATA
   ***************************/

  /* Function onRead:
   * callback
   */
  var onRead = function(data) {
    var dataReceived = bytesToString(data);
    $scope.$apply(function() {
      $scope.meterInfo = angular.fromJson(dataReceived);

      if(isNaN($scope.meterInfo.price_per_hour)) {
        $scope.disconnect();
      }

      $scope.meterInfo.price_per_hour = parseFloat($scope.meterInfo.price_per_hour);
      $scope.meterInfo.resolution = parseFloat($scope.meterInfo.resolution);
      $scope.meterInfo.limit = parseFloat($scope.meterInfo.limit);

      $scope.timeSelected = $scope.meterInfo.resolution;
      $scope.price = $scope.meterInfo.price_per_hour / 60 * $scope.timeSelected;
      $scope.price = parseFloat($scope.price.toFixed(2));
    });
  }

  /* Description:
   * Read Data from a BLE connected peripheral
   */
  $scope.readData = function() {
    ble.read(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), onRead, blePerpheralsService.onError);
  }

  /****************************
   * WRITE
   ***************************/

  /* Function onWrite:
   * Callback
  */
  var onWriteSuccess = function() {
    // Decode the ArrayBuffer into a typed Array based on the data you expect
    alert("Parking success !");
    // Go back home
    $scope.disconnect();
  }

  var onWriteError = function() {
    // Decode the ArrayBuffer into a typed Array based on the data you expect
    alert("Parking unexpected error");
  }

  /****************************
   * DISCONNECT
   ***************************/

  /* Description:
   * Transition to smart view and reset peripheral list
   */
  var backToHome = function () {
    console.log("Connection disconnected");
    $scope.changeState("tab.parking");
    $scope.blePeripherals = [];
  };

  /* Description:
   * Disconnect from the BLE peripheral
   */
  $scope.disconnect = function() {
    ble.disconnect(blePerpheralsService.getSelectedDeviceId(), backToHome, blePerpheralsService.onError);
  };

  /****************************
   * FUNCTION
   ***************************/

  $scope.fakeFill = function() {
    $scope.number = "4242 4242 4242 4242";
    $scope.expiry = "12/2017";
    $scope.cvc = "123";
  }

  $scope.stripeCallback = function (code, result) {
    if (result.error) {
      alert('It failed! error: ' + result.error.message);
    } else {
      var str = "Park;"+parkCarService.getSelectedVehicle().beacon+";2000-01-01 12:30;"+$scope.timeSelected+";SERVICE_1;valid_test_hash\n";
      ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWriteSuccess, onWriteError);
    }
  }

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  /****************************
   * ONLOAD
   ***************************/

  $scope.readData();

});