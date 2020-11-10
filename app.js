const express = require('express');
const app = express();

var SerialPort = require("serialport");
var WebSocketServer = require('ws').Server;
const Readline = require('@serialport/parser-readline');

var SERVER_PORT = 8081;               // port number for the webSocket server
var wss = new WebSocketServer({port: SERVER_PORT}); // the webSocket server
var connections = new Array;          // list of connections to the server

wss.on('connection', handleConnection);

function handleConnection(client) {
 console.log("New Connection"); // you have a new client
 connections.push(client); // add this client to the connections array

 client.on('close', function() { // when a client closes its connection
 console.log("connection closed"); // print it out
 var position = connections.indexOf(client); // get the client's position in the array
 connections.splice(position, 1); // and delete it from the array
 });
}

function broadcast(data) {
 for (myConnection in connections) {   // iterate over the array of connections
  connections[myConnection].send(data); // send the data to each connection
 }
}

var sp = new SerialPort("COM5", {  
    baudrRate: 9600 //115200
    // parser: serialport.parsers.raw
});
sp.close(function (err) {
    if ( err ) {
        return console.log('failed to close: ',err.message);
    } else {
        return console.log('close serial port successful: ',err.message);
    }
});
// var sp = new SerialPort(portName, { //parsers are 'raw','readline','byteLength','byteDelimiter'
//   //parser: serialport.parsers.readline("\n")
//   parser: serialport.parsers.raw
//   //parser: serialport.parsers.byteLength(13)
// });
sp.open(function (error) {
    if ( error ) {
      console.log('failed to open: '+error);
    } else {
      console.log('open');
    //   sp.on('data', sendSerialData);
    }
    let parser = sp.pipe(new Readline());   //raw, byteLength(13)
    parser.on('data', sendSerialData);
    // sp.on('data', sendSerialData);
});
function sendSerialData(data) {
    // if there are webSocket connections, send the serial data
    // to all of them:
    console.log('ssd',String(data));
    if (connections.length > 0) {
        broadcast(String(data));
    }
}

// SerialPort.list().then(
//     ports => ports.forEach(console.log),
//     err => console.error(err)
// )

var port = 3000;
// var arduinoCOMPort = "COM5";    ///dev/tty.SLAB_USBtoUART";

// var arduinoSerialPort = new SerialPort(arduinoCOMPort, {  
//     baudrRate: 115200
// });

// // arduinoSerialPort.isOpen;

// arduinoSerialPort.close(function (err) {
//     if ( err ) {
//         return console.log('failed to close: ',err.message);
//     } else {
//         // console.log('open');
//         // //接受串口数据，并打印到终端
//         // arduinoSerialPort.on('data', function(data) {
//         // console.log('数据接收: ' + data);
//         // });
//         return console.log('close serial port successful: ',err.message);
//     }
// });

// /// https://serialport.io/docs/api-parsers-overview
// const Readline = require('@serialport/parser-readline');

// let parser = arduinoSerialPort.pipe(new Readline());
// parser.on('data', console.log);

// arduinoSerialPort.on('open',function() {
//     console.log('Serial Port ' + arduinoCOMPort + ' is opened.');

//     console.log('open');
//     //接受串口数据，并打印到终端
//     arduinoSerialPort.on('data', function(data) {
//         console.log('数据接收: ' + data);   //.toString('utf8'));   //ascii/hex
//     });
// });

// ////////////////////////
// var serialport = new SerialPort(arduinoCOMPort, {
//     baudRate: 9600,//這數值必須與arduino 那邊設定對應
// });

// serialport.close(function (err) {
//     if ( err ) {
//         return console.log('failed to close: ',err.message);
//     } else {
//         // console.log('open');
//         // //接受串口数据，并打印到终端
//         // arduinoSerialPort.on('data', function(data) {
//         // console.log('数据接收: ' + data);
//         // });
//         return console.log('close serial port successful: ',err.message);
//     }
// });

// const parsers = SerialPort.parsers;//解析器讀取 console log
// const parser = new parsers.Readline({
//     delimiter: '\r\n', //字串使用換行切割符號
// });

// serialport.pipe(parser);//輸出到解析器 (重點)
// parser.on('data', function (inputstr){
//     // //寫入資料庫
//     // var date = new Date();
//     // var hour = date.getHours();
//     // hour = (hour < 10 ? "0" : "") + hour;
//     // var min = date.getMinutes();
//     // min = (min < 10 ? "0" : "") + min;
//     // var sec = date.getSeconds();
//     // sec = (sec < 10 ? "0" : "") + sec;
//     // var year = date.getFullYear();
//     // var month = date.getMonth() + 1;
//     // month = (month < 10 ? "0" : "") + month;
//     // var day = date.getDate();
//     // day = (day < 10 ? "0" : "") + day;
    
//     // //這邊簡單呼叫時間函式寫入.不再另外載入 moment.js
//     // var queryStr = 'INSERT INTO `light_record` SET ';
//     // queryStr += ' `val` = "'+inputstr+'", ';
//     // queryStr += ' `state` = "1", ';
//     // queryStr += ' `inputTime` = "'+year+"-"+month+'-'+day+' '+hour+':'+min+':'+sec+'" ';
//     // queryStr += 'ON DUPLICATE KEY UPDATE val = "'+inputstr+'" ';
//     //console.log(queryStr);
//     // z2.mdb.query(queryStr, function(err, rows) {
//     //     if (err)
//     //         throw err;
//     // });
//     console.log("Write "+inputstr);
// });

// //串口打开
// arduinoSerialPort.open(function (err) {
//     if ( err ) {
//         return console.log('failed to open: ',err.message);
//     } else {
//         console.log('open');
//         //接受串口数据，并打印到终端
//         port.on('data', function(data) {
//         console.log('数据接收: ' + data);
//         });
//     }
// });

app.get('/', function (req, res) {
    return res.send('Working');
})

app.get('/:action', function (req, res) {
    
   var action = req.params.action || req.param('action');
    if(action == 'led'){
        arduinoSerialPort.write("w");
        return res.send('Led light is on!');
    } 
    if(action == 'off') {
        arduinoSerialPort.write("t");
        return res.send("Led light is off!");
    }
    return res.send('Action: ' + action);
});

app.listen(port, function () {
  console.log('Example app listening on port http://0.0.0.0:' + port + '!');
});

// var SerialPort = require("serialport");
// //设置串口号，波特率，关闭自动开启
// var port = new SerialPort("COM10", {
//   baudrate: 9600,
//   autoOpen:false
// });  


//https://zhuanlan.zhihu.com/p/98050314
'use strict';
const { Transform } = require('stream');

class ConcatParser extends Transform {
    constructor(options = {}) {
        super(options);
        try {
            if (typeof options.boundary === 'undefined') {
                throw new TypeError('"boundary" is not a bufferable object');
            }

            if (options.boundary.length === 0) {
                throw new TypeError('"boundary" has a 0 or undefined length');
            }

            this.includeBoundary = typeof options.includeBoundary !== 'undefined' ? options.includeBoundary : true;
            this.interval = typeof options.interval !== 'undefined' ? options.interval : 3000;
            this.maxBufferSize = typeof options.maxBufferSize !== 'undefined' ? options.maxBufferSize : 65535;
            this.intervalID = -1;
            this.boundary = Buffer.from(options.boundary);
            this.buffer = Buffer.alloc(0);

        } catch (error) {
           throw new Error('Init concatparser error');
        }
    }

    _transform(chunk, encoding, cb) {

        clearTimeout(this.intervalID);
        let data = Buffer.concat([this.buffer, chunk]),
            dataLength = data.length,
            position;

        if (dataLength >= this.maxBufferSize) {
            this.buffer = data.slice(0, this.maxBufferSize);
            data = Buffer.alloc(0);
            this.emitData();
        } else if ((position = data.indexOf(this.boundary)) !== -1) {
            this.buffer = data.slice(0, position + (this.includeBoundary ? this.boundary.length : 0));
            data = Buffer.alloc(0);
            this.emitData();
        }
        this.buffer = data;
        this.intervalID = setTimeout(this.emitData.bind(this), this.interval);
        cb();
    }

    emitData() {
        clearTimeout(this.intervalID);
        if (this.buffer.length > 0) {
            this.push(this.buffer);
        }
        this.buffer = Buffer.alloc(0);
    }
    _flush(cb) {
        this.emitData();
        cb();
    }
}

module.exports = ConcatParser;


// CA4A844242B0426546B525224353765B464FF
// CA4A84428ED0B56D6A700D263B03DF6F734FF
// CA4A8442C3218E7EBA174A92EBF29F222
// CA4E8442BC2EDB6A4BB227C8DA34A532B021F3
// CA4E8442DD87D8F72FB5704BCEE4B47636A7F5
// CA4E8442428C7635D0F61572CA5A5EC46EC4EE
// CA5A84425276A07DA72F336852A0B52125B1E7
// CA5A844295C7E75F425E6CAF23F6365B5F2FF
// CA5A8442D2E18442E86EE9537E

// CA428442DAA637F13D42842628133B2F227FF
// CA428442D6A82CD267CC23C853A223D33637F6
// CA4284425F43FA2F762521C89AC3E5B57425CE
// CA4284427A4B8A4

// //42FEB36656E676524DCCE2C53BDC6FF

// CA428442D2DC603DA3673685A6466763635F4
// CA4284421CD388565667527A3760AD8071F5FF
// CA4284424B22E14425B57A4229A5D0266462F0
// CA42844262D6BCEA83AAD4A2ACB64E6B6BFB1
// CA428442775AE2CBF58426485BB424A765A5FA
// CA428442E1AFC2B4C69D42E1C542E5CD7EFA
// CA428442C26EEB29FF4F4348D235E4CB2723C6
// CA42844243D6C1CCBBB727F28C495ECF29D2FF
// CA428442DF35AADE43D3F236C71AF7361FF
// CA428442525A3

// CA4A844288FBA9765A32B28CECFB1C4410FE
// CA4A84424E8B2DDE4067297BEA4A23E2F122
// CA5A8442D76D764E4B02F43F726F61C276EF2
// CA5A8442A9D72FE685222C8D3A08486A467CE
// CA5A8442CA58D7C3D33436685273F7A2B323F0
// CA5A8442DA66ADE7AB2D2348CA2061E0AD60CE
// CA5A8442374AA14A4EBD42DDD0F696DB8622
// CA5A844242735C73A1B43DC342A0B1B7A725E7
// CA5A8442D61A1E1CC661

// 0F000FF0000F0FF300FF600FF000300
// CA428442DCC3B33446F5152A27271B1B135FF7F00200000000000000000806000FF3F000C0FF0000000F0FF390000000F870000E0FFFFFF50000000FFFFFF70000081FFFFFFFFD000000FF3F000000FFFF3B0100FF600FF46000F00CA5A8442C4F3735A9C3215627DA79C4CF6B6FFCA5A840C3FB4AA424712D227F7663D27E7FFCA38840FF56F474AEF9CA771A125A7F4E7CA3884422323A6DEC57B624CDBE032F0E222FFFF60000CA5A840DAFEFF969634A62FCB3A54A6ACAFECA52840E94AEE5EA352126ABDAB5A7687E69CA5284042DE864F77562EE85BD3C3A4A524FFCA52840C370E2AFDC805A4A6EE5E87AC54EFDCA281003743C3D8B6360D231B770A723A5FFCA52840435BA53DDF2257CA98A810E4ED3F1CA52104270742F71D3871805F42F7273576F5CA5284042B0FF2DE3589F72E384C2BC61ADFFCA52104278742F31F1BF340E537B520A7E0E5CA528442D3B5273727D22184B243FFAE4ADF6CA2884087525757B42D240275A72AFC85A3BCA52840F90827F879C9D4A7B5E66DA74ADF6CA28840EBB1F73DF9A1270E0A2E1AD21A7F9CA52104268CCD676243208CAE4A5E86ACD19CA2810426F5E617D4A62278A72FB3E52A42FFCA28840D27025C87ABFC707A3FE5F52567F7CA5210428F5F4A4925BF6F2DFCC63AFA123FFCA2884042A5CCF534943648CBB3A2375A7AF3CA52104243C23527B560250D2E5A725A537F7CA288404A3212711A5B2A88CB20B5F2B13DB0CA5284426BB588A53D4E4E60E4A260B770F2FFCA521042324A4652317D4A52CAA7C8FA5EC5CACA288401BCBA5F567622DC3CB63BF25F5AFB7CA52840BB5B2173FA634681ACB25F72774FFCA288404B76628CA374274BA53736A72637E5CA528404F7A7AA67A474248CFDA7131A737E5CA28104272C865E52DD735AA32D73FFE9D9F9CA28840B0F3A36EC2A2426124EC5729ED3CA528407B2227B4295BAB62E51A7EECC61FF
// CA5A8442F34261A3F064354842F086F1E9C9D
// CA38844298CD264A7FE26852D3E48A4EA
// CA5A8409C25C26B7AC1E2734DF6727F7EFE
// CA5A1042E1E1F5AB2B6B1172E16E7BFAE2FBFA
// CA388442F667F5C2A2E3270B2E2A36FBD2FF
// CA5A8442CA2192A65FC0956AE9856BD6F6E5E6
// CA52104248DA8BB7B55762DAA51B521A27CF
// CA5284426B2B7FFEEB2A20885A747027F733F2
// CA528442CB37CA5062BF30E0B5A76064A6FF
// CA521042CAB05D49AD6827D262203794FBFDFF
// CA5210424B9B325EB5D495626133F286F64CFF
// CA2884427120A35A4C5E260D22727342123FF
// CA5284057B9F3725AF22DAF7A5431ABFAFFF
// CA5284424375E12FF2E0314B6EC64DC37B88FE
// CA528442DC2A4633BD75370334DAF67527B1
// CA528442734B4B23626F5648CB62333DE246
// CACA528405A5B5376ABCB05A6574E4E74AF4FF
// CA52840CC4E2B0E5E7956575E8A3B95CFE
// CA2884422DE45B4A423142AA5A86328416DC
// CA52104253A3F0DAA41E635A76CEE1F6A322
// CA5210425EB17DC1E6772BC85BB424B514B3F1
// CA528406FDAE252677C4A626B2868DB6229F3
// CA5284426ACA635E2F7624AA5B66435D231FE
// CA528442E34B709C5ACB4302473C7E75A34
// CA28844280528F5EB3E325C36476CAC36777F4
// CA528442C2F0772DB567887BDDD171A14AFF
// CA2884425FCF66AE273275A64A56320F0EDFF
// CA528442D604A57ED2763FE5E5D3969DFAFF
// CA528442CBE0F43522E54842A5B74AD222169E
// CA5284424FB05218F1712743444BE7F47425F7
// CA52844297D12DDB4BF474E33E0E1E2B1A0FF
// CA528442DA264EDC67CA42DBC562CDC4AE3
// CA528442C7DA1BDBD64E42DA608C30866A0FF
// CA5284423F9EC474767F62FEA594DAB73378FE
// CA5284427B77E952778342D324D237DA35D2
// CA52844242FBF36E68F62CA3485B7E46DFE
// CA528442328DC33EC96CE4A8D48428A973FF
// CA52844236869E4A65F552486246B360B7C2
// CA5284425A5BD6D6E4B125487653C355B67FE
// CA528442AF79485DEE7464258B16F28C20FE1

// 000F8FFFF000010FF
// CA4A84053E6D486E462424A2AC3FE98565FF
// CA4A8402143A4F047B02B8C04654C2F5CBFF
// CA4A8405E56E959D6C36723312434F734BFE
// CA4A840791E66F5C48A4F09C874A5966CCC2
// CA28104242567A7FDA8B5A886DFE60239C60FF
// CA4A1042B34AC4215C416625A537B424864FE
// CA4A840FC60117E9FF4E726A7C4E8D68DAEF
// CA2810426EA8FED27E4815AD65EBC353B4FE
// CA4A104282CFCB5E6A7E260CA4B22F3A035B0
// CA2810427043522EDB362FADAA47B82D267FF
// CA2884421FB2B2D7E8FAE4E3686E21C75F9
// CA4A8442EE697A1CE7422DED29FB3A1D77AFE7

// 0FF
// CA4A8404BCBC6266ACA17276C5A44EC0E8F8
// CA4A104276B18E437D201A72F752F4D3DA
// CA4A84063F29AE7BD6442A47AF671681FA
// CA4A84426816E4ED47E1352B3A0E96F45BFF

// 00
// 21436587 64010640 209E EF73 F00404000803F50A020006000540003C014036ED80BEF2FA89BE5E576BEF15D30BE4685A0BDF0759A3C90BEC63DC419E3E30DFC3E61ACF3D9A6D453D032283ADB58DABCA34B1BD8894BABC23433BBCC5D380BBE7C5BBA987AEB8000060005400000140DC82B3BC7817BDBCCC3BBBBCC841B0BC8CAB9EBCDCF388BC7DC862BCF4E833BCA5D8BC7A7DC4BBBE7185BB1AAC28BB319EC2BAE26F47BA6CDCAEB9703BF9B842C35B8E69EBDB6721D3B50000FFFF
// 2143658764010640205EBA293F00404000803F50A020006000540001E014060A5ABBC9C9E4BD33AD5DBE5391A1BE1036C4BE1047CBBEFA6B0BEB4306CBE20BE95BDF873C73D8AD87D3E2F12B43E6138CA3E1BD7C13E2845A13EB491623E040E73D08AAC3AF463D5BD447D45BE60005400000140A8AC33EB861B3EC0ABF3EE698103E6A4EE3EA5793E361413EA0AAED3DD174D53D7E7ABA3DB0BA9D3D8636803D58CF453D906ED3D64D6B13C2895263C08E3139C8B34BCA0216EBCAC989CBCFFFF
// 2143658764010640205EBA293F00404000803F50A02000600054000280140B3F4D1BDCD408BE82E71DBEB33D23BEF3A213BEF613DCBDA0E957BD40DC493CFC339E3D62CD63E49BF2B3EADB83A3E3214363E7EC8243EA1A9F3E1E9EFB3D9CE0E23D14CBCC3DF052A63DB8E3313D60005400000140B9627BBE74A778BE915872BE4E8868BE9A5A5BBE5854BBE40D037BE41422BE9C38ABE2066E1BDD85ACBD0BD6ABD6019FABC04B89BB8093AD3CD0C2383D286F893DB0B5B13DA87BD43DC82BF13DFFFF
// 2143658764010640205EBA293F00404000803F50A02000600054000280140B3F4D1BDCD408BE82E71DBEB33D23BEF3A213BEF613DCBDA0E957BD40DC493CFC339E3D62CD63E49BF2B3EADB83A3E3214363E7EC8243EA1A9F3E1E9EFB3D9CE0E23D14CBCC3DF052A63DB8E3313D60005400000140B9627BBE74A778BE915872BE4E8868BE9A5A5BBE5854BBE40D037BE41422BE9C38ABE2066E1BDD85ACBD0BD6ABD6019FABC04B89BB8093AD3CD0C2383D286F893DB0B5B13DA87BD43DC82BF13DFFFF
// 2143658764010640205EBA293F00404000803F50A020006000540003C0140969183BD181A4ABD2C1086BCC8CBDE3CD22A8F3DD412CC3D94DDDB3DEBD9B83D56D9593D4010EC3B5A86BBD38FF71BD15F27EBDEF363ABD2318CBCDF78323CBCD7DA3C8F90B53C68138CBB266B3CBD60005400000140BECE9E3D808A03DF47BA23D581FA63DACDCAA3D3890B03DE87B73DE83BE3DB837C53D584BCC3D10DDD23DC883D83D78D1DC3D056DF3D20A2DF3DE849DD3D38E7D73D381DCF3D109DC23D302BB23DFFFF
// 2143658764010640205EBA293F00404000803F50A020006000540003C0140CC53F3BC4287DCBC345C77BCF8CB843B226DD23C7C7303D1814E3DC6EE333D162AD23C808DE43A22719CBC13C9F0BC5C24D8BC2A15EBC50285E3A7AE3FB3BF88BF3A5C618BBC262327BD739074BD60005400000140B07CB03DB84EB93D8871C03D68DEC53DA498C93DDCADCB3DE835CC3D4C52CB3D642DC93D2CF9C53D78EDC13D3646BD3DCC41B83DBB1FB33D6D1FAE3DA47EA93D4D77A53DC93CA23D7CF99F3D8ECD9E3DFFFF
// 2143658764010640205EBA293F00404000803F50A020006000540003C0140D2D9513C719B823C3AE8593CAF42BE3BA07350BB3AD11DBCBA7135BCB710FCBB5C72DFBAB3FB883BEF743CB850193C4DCE3CC263E83B72B4AE3B8479493BA0145DBA201EF3BBE72885BC7069CCBC60005400000140E0FB61BDC06551BDF0173EBDE01B28BD2088FBD205E9BC2082AEBC01C60BC8015B9BB03E313B80EC383CC0D0A33C6052EB3C206193DD0863B3D20B55C3D20217C3D58B28C3DB8129A3D50BA63DFFFF
// 2143658764010640205EBA293F00404000803F50A020006000540004301402C7DCD3C8042883C9663FE3B8093243A1CDC5EBBF42487BBE28526BB0B58EB94C87AC3A486CE13AC8AE9A3A0728B82E61DBBA6B0B1BBA6F517BC243048BC6CF54DBC2D419BC4C3734BB6675B63B600054000001403C483EBDA07944BDC0544BBDC8BA52BDF0815ABD487962BD06C6ABD402372BD306779BD8080BD94DB82BDA82B85BD38D686BDF0C087BDE8D087BD40EB86BDC8F684BDF8DC81BD50147BBD90DC6FBDFFFF
// 2143658764010640205EBA293F00404000803F50A02000600054000490140C8D29BDA86B45BC1CC8A03C128E453D22238E3D565EA33DD9689D3DCB14753DF812FC3C106F99BB7A201DBDC797EBDBCA890BDE3281BDBB5B31BD622394BCB09FB23B040B63C5BCF83CAA4FFA3C60005400000140209FEEBC50F9F9BCB8272BDEADD6BDFC29BBD3916FBD90AC12BD28F515BD7FF518BDCEB11BBDA7301EBD867E20BD50AF22BDB0DD24BD3A2927BDCB429BD2CA02CBD40D30BDAC1634BD4CD138BDFFFF
// 2143658764010640205EBA293F00404000803F50A020006000540004801402BD687BDE611A8BD5991AABDF6C85BDA4CCECBCA8E08F3C8A77D3D4A39BC3DEA53CD3D934BB33D799F7A3D5E9203DE050C93B18F446BC9C6FD7BCFE7725BD676C62BDDEDD89BD3E4491BDE0DB7EBD6000540000014080FD7CBB0CFCBA0488BA0F8A7390141B3A030A2390A40BA0FE6BA80E25FBBC0D7B0BB409AF9BB50D623BC90554CBC03275BCA0B58EBCD8CA2BC383DB4BCE04C5BC604BD4BCA01DE2BCFFFF
// 2143658764010640205EBA293F00404000803F50A02000600054000460140303260BDBC1B9CBD6228A6BD3E4687BD30E69BDD89A23CEE53453DF369E3D43BB63D40E2A93D72BC813D57E5193D58585D3CC0D149BB804629BC18852EBCE0DF1BBC3CE154BC2CA2CBBCD6636BD60005400000140C0D0C8BC8016ECBC68B5BD502611BD382D1ABDA0620BDD0AE22BD383822BD48CB1EBD30A518BDF81410BD68775BD2066F2BCA069D7BCA0DABABC209E9DBC809980BC805D49BC206C15BC80DACDBBFFFF
// 2143658764010640204E62103F00404000803F50A020006000540004B0140345F843C5C30153B1AB888BBA03526BB9647913B7837453CF5EB803C8E7533C90D37E3B669F8BCA9879BBCA8B8BFBC14459BBC22ECEBBE7762C3CCA6BCE3CA63303DAEEDC83CF8A6923B76CEC9BC60005400000140901A6D3D181A6C3D585693D60D2633DE07A5C3D38FC523DC857473D6893393DD8BA293DC8E2173DE02B43DA08ADD3C40DCAF3C80D67F3C0E81C3C05A623B0422DBB80EECBC0636ABC80EDA0BCFFFF
// 2143658764010640204E62103F00404000803F50A02000600054000490140DC85143CA17E7B3C4E1DD23CF628143DDEB52A3D2D73193DFC3CAF3C105B9FBBB2CD12BD1F7C82BDD054A5BD5695A7BDD8FA87BDCA6C1DBDF06AA9BBF497CE3C172F3C3DE7975A3DEB0443D9DCF83D600054000001400DDB93A306A8E3B8081F73B981A343C50736F3CB07A963C23E4B53C2289D53C3FFDF43CEFEA93DC3D5183D84D273D6063343DCAAB403D78BE4B3DC075553DE0AD5D3DCC44643D01B693DB0146C3DFFFF
// 2143658764010640204E62103F00404000803F50A02000600054000440140C095F7BAB037F9BBA8A843BC326B6CBC6C647BBC8F777DBCD0E77EBCD24F82BC141985BCA9C281BC845B62BC14531EBC80A16ABBA0FB643BF078253CC41B703CF852833CFB8E683CE8BF2D3C5C4F23C6000540000014064E4743C4D14B3CD42A223C7024F13B0619F3BA0DE203B0463839A0B8FCBA40DF79BB401FB2BBC871DDBB80BCFDBB58FE8BC18AFCBCB4A19BC184EFFBBB064DDBBF0B2ADBBA04F61BB80CF9BBAFFFF
// 2143658764010640204E62103F00404000803F50A02000600054000440140C47CE5BC86C11BD8EABF0BC28A5BBBCE0FC6CBC1CB4C4BB80A3833A6082B73BACB9F73BAC4FFA3B2840F13B1B2463C8DAE2D3CFEEB633CBEBB893CEEB8943C81668E3C742B6E3CF2BA233C85F8C3B60005400000140D0CFBE3CA08ACF3C6032DE3CC0BCEA3C4021F53C2056FD3CB8A713D588033D43043D3CB533D7C1223D889BFE3C10E0F63CF0AED3C443CE13CE49AD33C2C52C43C2C90B33CA885A13CC2678E3CFFFF
// 2143658764010640204E62103F00404000803F50A020006000540004701404AC7863C7DB4EF3C91CF163D51B6163D48B8E93CDE1C5E3C5C2093BB7456AABC5B2FFDBC36DD3BD1EBAC8BCC6624BC8841EF3B1F8ABB3CC0DF53DD432C3D6A3DF3CC2DF5E3C90B83DBB1AC092BC600054000001401C4C7BDB8701BDC0AFF4BC301EE4BC804ED1BC3069BCBC09EA5BC10258DBC807F66BC807030BC807EF1BB02D80BB0FCE4B9805F453B406ED13BC04C1E3C407F513C70E5803C3063973CC010AC3CFFFF
// 2143658764010640204E62103F00404000803F50A02000600054000460140F21538BCE0B877BBD463A13B6C83553CA0C39E3CCD22BF3CA97C73CF02BB53C9558893C12CF153C059DB3978498BC3F8B85BCFCC4BCBC1878E5BC178CF8BCAFE3ECBCB1A0BBBC5C2D4ABC20FACB3A60005400000140F791EBC70F83EBC76E61BC3B2682BC6F1A94BCA221A6BC14F8B7BC4657C9BCFCF4D9BC1885E9BC14BDF7BC682B2BD34897BDB8DBBBDB0AFBD3C211BD9CB311BD941511BD2824FBDB0DFBBDFFFF
// 2143658764010640204E62103F00404000803F50A0200060005400000140C7318BC060ABA90AAF3CB9AE823C90B29B3CD8CE8C3C33F6393C8A06B3BB0F565BB502A0BC8916ABC23B4C3BBDCE413BBD852223A9818893AC45CCBA2CDFD4BB90A043BC1FF97EBC8A07EBC600054000001402E373B3B4DA9523B72D6633B84506F3B466753BA828763B4479713B5CF673BC082563B8583F3B687213BD03F63A078993A09FAD39A03E2ABA10F5E8BAD05B48BBD42893BBBA1CC7BBF0D3FFBBFFFF
// 2143658764010640204E62103F00404000803F50A0200060005400000140CA2D3F3DC411533DDA603A3DDE7FF33C90C543C5CE563BCF86AFDBC08B20BD79AD19BD74C7E0BCDCDC4CBC060363B932E683C5A8E9A3C78C5853CACB373C78FA4BBC6D133BCC5B480BC598875BC6000540000014098B4AEBC6480A8BCC2B4A0BCDE7797BCEF68CBC276281BC88E869BCE0D04FBCF3FA34BC0E719BCDD2AFEBB7ACABB4B5E98BB13354BB8395FEBAF4454EBAC8B36939E9408E3A9395EC3A94B71C3BFFFF
// 2143658764010640204E62103F00404000803F50A0200060005400030140AEA2ADBBCC113B4E6253C53228A3CA3F7A73CE014A63C1DDD853C14DA1D3C40ACC23A88C4DEBBE4AE6ABC145AAABC8EAFD6BC6A14F7BC49AF0BDC22E7BC9ACE9CBC38C58CBB9C9D6A3CFEB463D60005400000140C03C83B0597B3B80F2C43A0F762BA405A54BBA033B7BB103D1BC08525BCC0F747BCB01E68BCD8C482BCB0E98FBCC8519BBC18D8A4BC1060ACBCC0D7B1BC439B5BCB488B6BC4CD4B5BCD02FB3BCFFFF
// 2143658764010640204E62103F00404000803F50A0200060005400050140D210F3CEA439F3BC0389B9461EA4BB30A33BC2EA111BC38343BCDC6CDBBD6FC90BB10663DBB68BC5BAC0C14C394A2C1E3BE43E953B5CE6AC3B7F415F3B9A4478BA8446C8BB9EE71DBCE0611CBC600054000001401CAEA73CD2F5AB3C908DAF3C4472B23C097B43C38E7B53C404AB63CF8A5B53CC4DFB33C44DDB03CB885AC3C88C3A63C58859F3C48BF963C486D8C3CC095803C2095663C704D493C7095293C30B673CFFFF