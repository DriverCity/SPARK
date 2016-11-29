var bleno = require('bleno');

var BlenoPrimaryService = bleno.PrimaryService;

var DeviceInformationService = require('./device-information-service');
var deviceInformationService = new DeviceInformationService();

console.log('bleno - ble peripheral');

bleno.on('stateChange', function(state) {
  console.log('on -> stateChange: ' + state);

  if (state === 'poweredOn') {
    bleno.startAdvertising('smart-meter', ['ec00']);
  }   
  else {
    if(state === 'unsupported'){
      console.log("NOTE: BLE and Bleno configurations not enabled on board, see README.md for more details...");
    }
    bleno.stopAdvertising();
  }
});

bleno.on('advertisingStart', function(error) {
  console.log('on -> advertisingStart: ' + (error ? 'error ' + error : 'success'));

  if (!error) {
    bleno.setServices([
      deviceInformationService
    ]);
  }
});

bleno.on('accept', function(clientAddress) {
    console.log("Accepted Connection with Client Address: " + clientAddress);
});

bleno.on('disconnect', function(clientAddress) {
    console.log("Disconnected Connection with Client Address: " + clientAddress);
});