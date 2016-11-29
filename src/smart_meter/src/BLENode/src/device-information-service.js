var util = require('util');
var bleno = require('bleno');

var BlenoPrimaryService = bleno.PrimaryService;

var ParkCharacteristic = require('./parkCharacteristic');

function DeviceInformationService() {
  DeviceInformationService.super_.call(this, {
    uuid: 'ec00',
    characteristics: [
      new ParkCharacteristic()
    ]
  });
}

util.inherits(DeviceInformationService, BlenoPrimaryService);

module.exports = DeviceInformationService;