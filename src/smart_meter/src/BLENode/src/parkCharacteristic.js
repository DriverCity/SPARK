var util = require('util');
var bleno = require('bleno');
var deasync = require('deasync');
var fs = require('fs');

var BlenoCharacteristic = bleno.Characteristic;

var ParkCharacteristic = function() {
  ParkCharacteristic.super_.call(this, {
    uuid: 'ec0e',
    properties: ['read', 'write', 'notify'],
    value: null
  });

  this._value = new Buffer(JSON.stringify("empty"), "utf-8");
  this._updateValueCallback = null;
};

util.inherits(ParkCharacteristic, BlenoCharacteristic);

ParkCharacteristic.prototype.onReadRequest = function(offset, callback) {
  console.log('ParkCharacteristic - onReadRequest - Received');

  // Synchronisation variables
  var sync = true;
  var data = null;

  // Fifo queue
  var fifoPathResponse = __dirname + '/../BLEResponseFifo';
  var outputFifo = fs.createReadStream(fifoPathResponse);

  var fifoPathInput = __dirname + '/../BLEInputFifo';
  var input = 'price\n';

  fs.open(fifoPathInput, 'w', (err, fd) => {
    if (err) {
      if (err.code === "EEXIST") {
        console.error('myfile already exists');
        return;
      } else {
        throw err;
      }
    }
    fs.writeSync(fd, input);
    fs.close(fd);
  });
  
  // Retrieve info from fifo
  outputFifo.on('data', function(result) {
    data = result;
    sync = false;
  });

  // Deasynch function for preserve callback
  while(sync) {
    require('deasync').sleep(100);
  }

  // Format data received
  data = (""+data).replace(/(\r\n|\n|\r)/gm,"");
  var str = data.split(";");

  // Create objects
  var dataReceived = {
    "price_per_hour":str[0],
    "limit":str[1],
    "resolution":str[2]
  }
  var bufferFormat = new Buffer(JSON.stringify(dataReceived), "utf-8");

  // Send callback
  callback(this.RESULT_SUCCESS, bufferFormat);
  console.log('ParkCharacteristic - onReadRequest - Response sended');
};


ParkCharacteristic.prototype.onWriteRequest = function(dataReceived, offset, withoutResponse, callback) {
  console.log('ParkCharacteristic - onWriteRequest - Received : ' + dataReceived.toString("utf-8"));

  // Synchronisation variables
  var sync = true;
  var data = null;

  // Fifo queue
  var fifoPathInput = __dirname + '/../BLEInputFifo';
  var input = dataReceived.toString("utf-8");

  var fifoPathResponse = __dirname + '/../BLEResponseFifo';
  var outputFifo = fs.createReadStream(fifoPathResponse);

  fs.open(fifoPathInput, 'w', (err, fd) => {
    if (err) {
      if (err.code === "EEXIST") {
        console.error('myfile already exists');
        return;
      } else {
        throw err;
      }
    }
    fs.writeSync(fd, input);
    fs.close(fd);
  });

  // Retrieve info from fifo
  outputFifo.on('data', function(result) {
    data = result;
    sync = false;
  });

  // Deasynch function for preserve callback
  while(sync) {
    require('deasync').sleep(100);
  }

  data = (""+data).replace(/(\r\n|\n|\r)/gm,"");
  console.log(data);

  if(data == "OK\0") {
    console.log("Success");
    callback(this.RESULT_SUCCESS);
  } else {
    console.log("Error");
    callback(this.RESULT_UNLIKELY_ERROR);
  }
};

module.exports = ParkCharacteristic;
