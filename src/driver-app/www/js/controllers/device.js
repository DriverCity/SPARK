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
    price_per_hour:3.5,
    resolution:15,
    limit:60
  }
*/
  $scope.timeValidity = null;

  
  $scope.timeSelected = null;
  $scope.price = null;
  // $scope.timeSelected = $scope.meterInfo.resolution;
  // $scope.price = $scope.meterInfo.price_per_hour;

  /****************************
   * PRICE INPUT
   ***************************/

  $scope.manageTime = function(way) {
    if($scope.meterInfo != null) {
      if(way == "+" && $scope.timeSelected != $scope.meterInfo.limit) {
        $scope.timeSelected += $scope.meterInfo.resolution;
        $scope.price += $scope.meterInfo.price_per_hour;
      } 
      if(way == "-" && $scope.timeSelected != $scope.meterInfo.resolution) {
        $scope.timeSelected -= $scope.meterInfo.resolution;
        $scope.price -= $scope.meterInfo.price_per_hour;
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

      $scope.timeSelected = $scope.meterInfo.resolution;
      $scope.price = $scope.meterInfo.price_per_hour;
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
    $scope.changeState("tab.meter");
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

  // $scope.readData();

});