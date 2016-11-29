var util = require('util');
var bleno = require('bleno');

var BlenoCharacteristic = bleno.Characteristic;

var ConnexionCharacteristic = function() {
  ConnexionCharacteristic.super_.call(this, {
    uuid: 'ec0e',
    properties: ['read', 'write', 'notify'],
    value: null
  });

  this._value = new Buffer(0);
  this._updateValueCallback = null;
};

util.inherits(ConnexionCharacteristic, BlenoCharacteristic);

ConnexionCharacteristic.prototype.onReadRequest = function(offset, callback) {
  console.log('ConnexionCharacteristic - onReadRequest: value = ' + this._value.toString("utf-8"));
  callback(this.RESULT_SUCCESS, this._value);
};

ConnexionCharacteristic.prototype.onWriteRequest = function(data, offset, withoutResponse, callback) {
  this._value = data;
    console.log('ConnexionCharacteristic - onWriteRequest: value = ' + this._value.toString("utf-8"));

  if (this._updateValueCallback) {
    console.log('ConnexionCharacteristic - onWriteRequest: notifying');

    this._updateValueCallback(this._value);
  }

  callback(this.RESULT_SUCCESS);
};

module.exports = ConnexionCharacteristic;