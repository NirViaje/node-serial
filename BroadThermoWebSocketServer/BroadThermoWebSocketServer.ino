#include <WiFi.h>
#include <WebSocketServer.h>


#define WIFI_SSID "bcore-lab" // change with your own wifi ssid
#define WIFI_PASS "bcore6688" // change with your own wifi password

#define PORT 80

WiFiServer server(PORT);
WebSocketServer webSocketServer;

String dataToSend; // update this with the value you wish to send to your client

long microNow = 0;
char pinIO = 26;
byte dataByte = 0;
String dataBlock = "";

void addDataBlockln(String str) {
  dataBlock += str + ", ";  //"\n";
}
void timingBits() {

  // 5.00ms for low group, 5.10ms for high group
  uint32_t microHigh = 0;
  uint32_t microLow = 0;
  uint32_t microHighLow = 0;
  bool bSync = false;
  
  char bitsCounter = 0;
  int bytesCounter = 0;
    
//  microNow = micros();

  const uint32_t RailHigh = 0;
  const uint32_t HighLowRail = 0;
  const uint32_t RailSumDelta = 0;

  const uint32_t bitHigh = 0;
  const uint32_t bitShort = 0;
  const uint32_t bitLong = 0;

  const uint32_t bitDelta = 0;
  
  while(!bSync) 
  {
    
    if(digitalRead(pinIO)) {
      while(digitalRead(pinIO)) {
        ;
      }
  //    microHigh = micros() - microNow;
      addDataBlockln("H " + String(micros()));
      microNow = micros();
    }
//    Serial.println("|_ ! C, I: " + String(++iFall) + ", " + String(microInterval));
    
    while(!digitalRead(pinIO)) {
      if(micros() - microNow > 5000 && dataBlock != "") {
        dataToSend = dataBlock; // + "]";
        dataBlock = "";
        return; //return data integrity
      }
    }
    addDataBlockln("L " + String(micros()));
    microNow = micros();
  }
}
void syncBits() {
//  pinMode(pinIO, INPUT);  //INPUT_PULLUP

  // 5.00ms for low group, 5.10ms for high group
  uint32_t microHigh = 0;
  uint32_t microLow = 0;
  uint32_t microHighLow = 0;
  bool bSync = false;
  
  char bitsCounter = 0;
  int bytesCounter = 0;
    
  microNow = micros();

  const uint32_t RailHigh = 0;
  const uint32_t HighLowRail = 0;
  const uint32_t RailSumDelta = 0;

  const uint32_t bitHigh = 0;
  const uint32_t bitShort = 0;
  const uint32_t bitLong = 0;

  const uint32_t bitDelta = 0;
  
  while(!bSync) 
  {
    
    while(digitalRead(pinIO)) {
      ;
    }
    microHigh = micros() - microNow;
    microNow = micros();
//    Serial.println("|_ ! C, I: " + String(++iFall) + ", " + String(microInterval));
    
    while(!digitalRead(pinIO)) {
      ;
//      if(micros() - microNow > 5000) {
//        dataToSend = dataBlock;
//        dataBlock = "";
//        return; //return data integrity
//      }
    }
    microLow = micros() - microNow;
    microNow = micros();
    
//    Serial.println(String(microHigh) + ", " + String(microLow)); //"_| ! C, I, N: " + String(++iRise) + ", " + 

    microHighLow = microHigh + microLow;
    if(microHigh > 440 && microHigh < 565) {
    
      if(microLow > 240 && microLow < 360) {
        // short bit
//        Serial.write("0");  //ln("_"); //SYNC_SHORT_BIT!");
        dataByte <<= 1;
        bitsCounter++;
//        return SYNC_SHORT_BIT;
      } else if(microLow > 1000 && microLow < 1200) {
        // long bit
//        Serial.write("1");  //ln("__"); //SYNC_LONG_BIT!");
        dataByte <<= 1;
        dataByte |= 0x01;
        bitsCounter++;
//        return SYNC_LONG_BIT;
      } else {
//        Serial.println
        addDataBlockln("ls T.O.: " + String(microLow));
      }
      
      if(bitsCounter == 8) {

        addDataBlockln(String(dataByte, HEX));  //BIN
        dataByte = 0;
        bitsCounter = 0;
        
//        Serial.println
//        dataToSend = ("B" + String(bytesCounter));
        bytesCounter++; //21?
        if(bytesCounter == 20) {  //bytesCounter = 0;
          Serial.println("dataBlock"+dataBlock);
          dataToSend = dataBlock;
          dataBlock = "";
          return;
        }
      }
    } else if(microHigh > 3200 && microLow > 1500) {
      if(microHighLow < 5050) {
        bitsCounter = 0;
        bytesCounter = 0;
//        Serial.println
        addDataBlockln("\nH.R.: " + String(microHigh) + ", " + String(microLow)); // + ", " + String(microHighLow));  //SYNC_HIGH_RAIL
//        return SYNC_HIGH_RAIL;
      }
      else if(microHighLow < 5300) {
        bitsCounter = 0;
        bytesCounter = 0;
//        Serial.println
        addDataBlockln("\nL.R.: " + String(microHigh) + ", " + String(microLow));  //SYNC_Low_RAIL
//        return SYNC_HIGH_RAIL;
      } else {
//        Serial.println
        addDataBlockln("SYNC T.O.: " + String(microHighLow));
      }
    } else {
      addDataBlockln("T.O.: " + String(microHigh) + ", " + String(microLow)); //, mH&L
//      Serial.println(dataToSend); //Out of any timing
    }
//    Serial.println(micros() - microNow);
    
//    char bSyncStatus = 0;//syncBit();
//    if(bSyncStatus == SYNC_LOW_RAIL) {
//      bSync = true;
////      Serial.println("We start with SYNC_LOW_RAIL!");
//    }
//    else if(bSyncStatus == SYNC_HIGH_RAIL) {
//      bSync = true;
////      Serial.println("We start with SYNC_HIGHT_RAIL!");
//    }
  }
}

void setup()
{
  Serial.begin(115200);

  pinMode(pinIO, INPUT);  //INPUT_PULLUP
  
  // try to connect
  if(connect() == 0) { return ; }

}

void loop() {
 webSocketLoop(); 
}

/*
  * Connect to the wifi (credential harcoded in the defines)
  */ 
char connect()
{
  WiFi.begin(WIFI_SSID, WIFI_PASS); 

  Serial.println("Waiting for wifi");
  int timeout_s = 30;
  while (WiFi.status() != WL_CONNECTED && timeout_s-- > 0) {
      delay(1000);
      Serial.print(".");
  }
  
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.println("unable to connect, check your credentials");
    return 0;
  }
  else
  {
    Serial.println("Connected to the WiFi network");
    Serial.println(WiFi.localIP());
    return 1;
  }
}

void webSocketLoop()
{
  Serial.println("Starting the server");
  server.begin();
  delay(100);

  Serial.println("server running");
  while(true)
  {
    WiFiClient client = server.available();
    
    if (client.connected() && webSocketServer.handshake(client)) 
    {
      String data;
      int timingNow = micros();
      int overTime = 0;
      while (client.connected()) 
      {
//        Serial.println("X " + String(micros() - timingNow));
        overTime = micros() - timingNow;
//        timingNow = micros();
        timingBits();
//        syncBits();
//        Serial.println
        timingNow = micros();
        dataToSend += "X " + String(overTime);
        
//        dataToSend = "Hello to you too..."+String(millis());

//        //getData
//        data = webSocketServer.getData();
//        if (data.length() > 0) 
//        {
//          onDataReceived(data);
//        }
        
//        delay(10); // Delay needed for receiving the data correctly
//        data = "";
        
        if(dataToSend.length() > 0)
        {
          webSocketServer.sendData(dataToSend);
         
        }
        delay(10); // Delay needed for sending the data correctly
        dataToSend = "";
      }
 
      Serial.println("The client disconnected");
      delay(100);
    }
    
    delay(100);
  }
}

void onDataReceived(String &data)
{
  Serial.println(data);

  if(data == "HELLO")
  {
    dataToSend = "Hello to you too...";
  }
  else if(data == "BYE")
  {
    dataToSend = "I am sad to see you leave";
  }
  else if(data == "WAZZUP")
  {
    dataToSend = "Life socks!";
  }
}
