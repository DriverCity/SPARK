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

  $scope.timeValidity = null;
  $scope.price = 0;


  $scope.meterInfo = {
    price_per_hour:3.5,
    resolution:15,
    limit:60
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
    // Convert data received
    var dataReceived = bytesToString(data);
    $scope.$apply(function() {
      $scope.meterInfo = angular.fromJson(dataReceived);
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
  var onWrite = function(buffer) {
    // Decode the ArrayBuffer into a typed Array based on the data you expect
    alert("Something " + buffer);
  }

  /* Description:
   * Write data to BLE peripheral
   */
  $scope.writeData = function(information) {
    var str = JSON.stringify(information);
    ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
  }

  /****************************
   * DISCONNECT
   ***************************/

  /* Description:
   * Transition to smart view and reset peripheral list
   */
  var backToHome = function () {
    console.log("Connection disconnected");
    $scope.changeState("tab.meter");
    $scope.blePeripherals = [];
  };

  /* Description:
  Disconnect from the BLE peripheral*/
  $scope.disconnect = function() {
    ble.disconnect(blePerpheralsService.getSelectedDeviceId(), backToHome, blePerpheralsService.onError);
  };

  /****************************
   * FUNCTION
   ***************************/

  $scope.timeParkValidator = function(parkTime) {
    if(parkTime == null) {
      $scope.timeValidity = null;
      $price = 0;
    } else {
      if((parkTime % $scope.meterInfo.resolution == 0) && (parkTime <= $scope.meterInfo.limit)) {
        $scope.timeValidity = 'valid';
        $scope.price = (parkTime / $scope.meterInfo.resolution) * $scope.meterInfo.price_per_hour;
      } else {
        $scope.timeValidity = 'invalid';
        $scope.price = 0;
      }
    }
  }

  $scope.fakeFill = function() {
    $scope.number = "4242 4242 4242 4242";
    $scope.expiry = "12/2017";
    $scope.cvc = "123";
  }

  $scope.stripeCallback = function (code, result) {
    if (result.error) {
        alert('It failed! error: ' + result.error.message);
    } else {
        alert('Success! token: ' + result.id);
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

  // $scope.readData();

});