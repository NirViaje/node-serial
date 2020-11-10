var SerialPort = require('serialport');
var port = new SerialPort('COM5');

//发hex
var senddata = [0x01,0x02];
//发字符串
//senddata = 'test data';

function writeport()
{
    port.write(senddata, function (err) {
        if (err) {
            return console.log('Error on write: ', err.message);
        }
        console.log('send: ' + senddata);
    });
}

port.on('open', function () {
    writeport();
});

// open errors will be emitted as an error event
port.on('error', function (err) {
    console.log('Error: ', err.message);
})

setInterval(function () {
    writeport();
}, 5000);


port.on('data', function (data) {
    //收hex
    console.log('recv: ' + data.toString('hex'));
    //收字符串
    //console.log('recv: ' + data.toString('ascii'));
  });