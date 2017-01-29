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
  $scope.timeSelected = 0;      // Time selected by the user. Initialized by the resolution retrieved from the start meter
  $scope.price = 0;             // Price per hour retrieved from the smart meter

  // $scope.timeSelected = $scope.meterInfo.resolution;
  // $scope.price = $scope.meterInfo.price_per_hour;

  /****************************
   * PRICE INPUT
   ***************************/

  /* Description :
   * Function for increase the current time selected by the user.
   * Based on resolution, limit and price per hour retrieved from the smart-meter
   */
  $scope.increaseTime = function() {
    if($scope.meterInfo != null) {
      if($scope.meterInfo.limit == 0 || $scope.meterInfo.limit != 0 && $scope.timeSelected != $scope.meterInfo.limit ) {
        $scope.timeSelected += $scope.meterInfo.resolution;
        $scope.price = $scope.meterInfo.price_per_hour / 60 * $scope.timeSelected;
        $scope.price = parseFloat($scope.price.toFixed(2));
      }
    }
  }

  /* Description :
   * Function for decrease the current time selected by the user.
   * Based on resolution, limit and price per hour retrieved from the smart-meter
   */
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

  /* Description :
   * Convert String to ArrayBuffer 
   */
  var stringToBytes = function(string) {
    var array = new Uint8Array(string.length);
    for (var i = 0, l = string.length; i < l; i++) {
      array[i] = string.charCodeAt(i);
    }
    return array.buffer;
  }

  /* Description :
   * Convert ArrayBuffer to String
   */
  var bytesToString = function(buffer) {
    return String.fromCharCode.apply(null, new Uint8Array(buffer));
  }

  /****************************
   * READ DATA
   ***************************/

  /* Description :
   * Function called by the read request when smart-meter send a response
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

  /* Description :
   * Send a read request to the smart-meter to retrieve information about prices, time limitation and resolution
   */
  $scope.readData = function() {
    ble.read(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), onRead, blePerpheralsService.onError);
  }

  /****************************
   * WRITE
   ***************************/

  /* Description :
   * Function called by the write request when smart-meter send a success response
   */
  var onWriteSuccess = function() {
    $scope.disconnect();
  }

  /* Description :
   * Function called by the write request when smart-meter send a failure response
   */
  var onWriteError = function() {
    alert("Parking unexpected error");
  }

  /****************************
   * DISCONNECT
   ***************************/

  /* Description :
   * Transition to parking state and reset peripheral list
   */
  var backToHome = function () {
    console.log("Connection disconnected");
    $scope.changeState("tab.parking");
    $scope.blePeripherals = [];
  };

  /* Description :
   * Disconnect from the BLE peripheral
   */
  $scope.disconnect = function() {
    ble.disconnect(blePerpheralsService.getSelectedDeviceId(), backToHome, blePerpheralsService.onError);
  };

  /****************************
   * FUNCTION
   ***************************/

  /* Description :
   * Fake fill of the bank card form on the UI
   */
  $scope.fakeFill = function() {
    $scope.number = "4242 4242 4242 4242";
    $scope.expiry = "12/2017";
    $scope.cvc = "123";
  }

  /* Description :
   * Function called when stripe API send a callback
   */
  $scope.stripeCallback = function (code, result) {
    if (result.error) {
      alert('It failed! error: ' + result.error.message);
    } else {
      alert('Card valid, send payment request to the smart meter');
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