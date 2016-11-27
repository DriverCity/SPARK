var util = require('util');
var bleno = require('bleno');

var BlenoCharacteristic = bleno.Characteristic;

var SmartCharacteristic = function() {
  SmartCharacteristic.super_.call(this, {
    uuid: 'ec0e',
    properties: ['read', 'write', 'notify'],
    value: null
  });

  this._value = new Buffer(0);
  this._updateValueCallback = null;
};

util.inherits(SmartCharacteristic, BlenoCharacteristic);

SmartCharacteristic.prototype.onReadRequest = function(offset, callback) {
  console.log('SmartCharacteristic - onReadRequest: value = ' + this._value.toString("utf-8"));
  callback(this.RESULT_SUCCESS, this._value);
};

SmartCharacteristic.prototype.onWriteRequest = function(data, offset, withoutResponse, callback) {
  this._value = data;
    console.log('SmartCharacteristic - onWriteRequest: value = ' + this._value.toString("utf-8"));

  if (this._updateValueCallback) {
    console.log('SmartCharacteristic - onWriteRequest: notifying');

    this._updateValueCallback(this._value);
  }

  callback(this.RESULT_SUCCESS);
};

SmartCharacteristic.prototype.onSubscribe = function(maxValueSize, updateValueCallback) {
  console.log('SmartCharacteristic - onSubscribe');

  this._updateValueCallback = updateValueCallback;
};

SmartCharacteristic.prototype.onUnsubscribe = function() {
  console.log('SmartCharacteristic - onUnsubscribe');

  this._updateValueCallback = null;
};

module.exports = SmartCharacteristic;