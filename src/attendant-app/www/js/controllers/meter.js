app.controller('MeterCtrl', function($scope, $state, $ionicModal, $interval, $timeout, $cordovaGeolocation, blePerpheralsService, CloudSrv) {

  /****************************
   * CLOUD
   ***************************/

  $scope.testCloudRequest = function(name) {
alert("ok");
    CloudSrv.testRequest(name).then(
      function(ret) {
        $scope.cloudResponse = ret;
        alert("message");
      }
    );
  }

  /**var options = {
    timeout: 10000,
    enableHighAccuracy: true
  };

  $cordovaGeolocation.getCurrentPosition(options).then(function(position) {
**/
    // Position coordinates
  //  var latLng = new google.maps.LatLng(position.coords.latitude, position.coords.longitude);

    //};

  /****************************
     * VARIABLES
     ***************************/

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
  console.log(JSON.stringify(device));
  // Add device to array
  tempArray.push(device);

  $scope.testCloudRequest(device.name);

};

/*
 * Description: Set device list and make visible
 */
var setDeviceList = function(){
  $scope.scanText = "Scan Complete";
  $interval.cancel(timeReduction)
  $scope.blePeripherals = tempArray;
  $scope.scanning = false;
}


/*
 * Description: Scan for all BLE peripheral devices or for devices with a specific service UUID
 */
$scope.scan = function() {

  // Initialize variables
  seconds = 5;
  tempArray = [];
  $scope.scanText = "Scanning (5s left)";
  $scope.scanning = true;

  // Scan withouth specifications
  ble.scan([], seconds, onDiscoverDevice, blePerpheralsService.onError);
  // Prepare variables
  $scope.isScanBtnDisabled = true;
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
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

});
