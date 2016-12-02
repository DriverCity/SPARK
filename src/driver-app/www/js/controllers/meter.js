/**********************************
 * [CONTROLLER] METER
 *********************************/

app.controller('MeterCtrl', function($scope, $state, $interval, $timeout, blePerpheralsService, $ionicPopup, parkCarService) {

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

    var similar = false;
    for(var i=0; i<tempArray.length; i++) {
      if(tempArray[i].id == device.id) {
        similar = true;
        break;
      }
    }
    if(!similar && device.name != null) {
      tempArray.push(device);
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

  /*
   * Description: Connect to a BLE peripheral device
   */
  $scope.connect = function(id, name) {
    // Set selected device characteristics
    blePerpheralsService.setSelectedDeviceId(id);
    blePerpheralsService.setSelectedDeviceName(name);
    
    // Define action after connexion
    var onConnect = function() {
      $state.go('tab.device', { 'id': id });
    }

    // Execute connexion
    ble.connect(blePerpheralsService.getSelectedDeviceId(), onConnect, blePerpheralsService.onError);
  }

  /****************************
   * UTILS
   ***************************/

  $scope.changeState = function(location) {
    $state.go(location);
  };

  $scope.showAlert = function(title,text) {
    var alertPopup = $ionicPopup.alert({
      title: title,
      template: text
    });

    alertPopup.then(function(res) {
      console.log('Alert show');
    });
  };
});