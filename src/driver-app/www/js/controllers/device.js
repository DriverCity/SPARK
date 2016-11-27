/**********************************
 * [CONTROLLER] DEVICE
 *********************************/

app.controller('DeviceCtrl', function($scope, $state, $stateParams, blePerpheralsService) {

  $scope.id = $stateParams.id;

  // Pre defined UUID for Service and Characteristic
  blePerpheralsService.setServiceId('ec00');
  blePerpheralsService.setCharacteristicId('ec0e');

  /****************************
   * BLUETOOTH
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

  /* Function onRead:
   * callback
   */
  var onRead = function(data) {
    console.log("data read");

    var str = bytesToString(data);
    console.log(str);
    
    $scope.$apply(function() {
      $scope.dataMessage = str;
    });
  }

  /* Description:
   * Read Data from a BLE connected peripheral
   */
  $scope.readData = function() {
    ble.read(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), onRead, blePerpheralsService.onError);
  }

  /* Function onWrite:
   * Callback
  */
  var onWrite = function() {
    alert("data written to BLE peripheral");
  }

  /* Description:
   * Write data to BLE peripheral
   */
  $scope.writeData = function() {
    alert("writeData");
    var str = "Hello from our amazing app!!";
    ble.write(blePerpheralsService.getSelectedDeviceId(), blePerpheralsService.getServiceId(), blePerpheralsService.getCharacteristicId(), stringToBytes(str), onWrite, blePerpheralsService.onError);
  }

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
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});