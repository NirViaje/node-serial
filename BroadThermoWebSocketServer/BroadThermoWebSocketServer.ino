#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketServer.h>


#define WIFI_SSID "bcore-lab" // change with your own wifi ssid
#define WIFI_PASS "bcore6688" // change with your own wifi password

#define PORT 80

WiFiServer server(PORT);
WebSocketServer webSocketServer;

#define TimingPriority 12

TaskHandle_t TaskTiming;

String dataToSend; // update this with the value you wish to send to your client

uint32_t microNow = 0;  //uint64_t
char pinIO = 26;
byte dataByte = 0;
String dataBlock = "";

void addDataBlockln(String str) {
  dataBlock += str + ", ";  //"\n";
}
void IRAM_ATTR isr() {  //interrBits() {
//    addDataBlockln(
    dataToSend += "C ";// + String(micros()) + ", ";
//  if(digitalRead(pinIO))
//    addDataBlockln(micros());//"L " + String(micros()));
//  else
//    addDataBlockln(micros());//"H " + String(micros()));

//  microNow = micros();
}
void timingBits() { //void *pvParameters) {

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

  uint32_t bitDelta = 0;  //const 

//  Serial.println("timingBits: " + String(xPortGetCoreID()));
  
  while(false) //!bSync) 
  {
    uint32_t intervalNow = micros() - microNow;
//    microNow = micros();
    if(intervalNow > 2) {
      dataToSend = String(micros()) + ", " + String(intervalNow) + ", " + String(microHigh - microLow) + ", " + String(bitDelta);// + ", " + String(micros() - microNow) + "\n";
      microLow = micros();
      bitDelta = 0;
    }
    else {
      microHigh = micros();
      bitDelta++;
    }
    microNow = micros();
  }

  while(true)
  {
    
    if(digitalRead(pinIO)) {
      while(digitalRead(pinIO)) {
        ;
      }
  //    microHigh = micros() - microNow;
      addDataBlockln("H " + String(micros()));
      microNow = micros();
    } //else {
//      addDataBlockln("Y " + String(micros()));
//      microNow = micros();
//    }
//    Serial.println("|_ ! C, I: " + String(++iFall) + ", " + String(microInterval));
    
    while(!digitalRead(pinIO)) {
      if(micros() - microNow > 5000 && dataBlock != "") {
        dataToSend += dataBlock; // + "]";
        dataBlock = "";
//        vTaskDelete(TaskTiming);  //
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

  attachInterrupt(pinIO, isr, CHANGE);

//  xTaskCreatePinnedToCore(
//             timingBits, /* Task function. */ //webSocketLoop, //
//             "TaskTiming",   /* name of task. */
//             10000,     /* Stack size of task */
//             NULL,      /* parameter of the task */
//             TimingPriority,         /* priority of the task */
//             &TaskTiming,    /* Task handle to keep track of created task */
//             0);        /* pin task to core 0 */

//  Serial.println("uxTaskPriorityGet: " + String(uxTaskPriorityGet(TaskTiming)));
}

void loop() {
//  timingBits();
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

void webSocketLoop()  //void *pvParameters)
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
//        dataToSend += "Y " + String(micros()) + ", ";
        overTime = micros() - timingNow;
//        timingNow = micros();
//        timingBits();
//        syncBits();
//        Serial.println
//        dataToSend += String(digitalRead(pinIO));
        timingNow = micros();
//        dataToSend += "X " + String(overTime);
        
//        dataToSend = "Hello to you too..."+String(millis());

//        //getData
//        data = webSocketServer.getData();
//        if (data.length() > 0) 
//        {
//          onDataReceived(data);
//        }
        
//        delay(10); // Delay needed for receiving the data correctly
//        data = "";
        
        if(dataToSend.length() > 0)// && micros() - microNow > 20000)
        {
          webSocketServer.sendData(dataToSend);
          Serial.println(dataToSend);
          
//          xTaskCreatePinnedToCore(
//                     timingBits, /* Task function. */
//                     "TaskTiming",   /* name of task. */
//                     10000,     /* Stack size of task */
//                     NULL,      /* parameter of the task */
//                     TimingPriority,         /* priority of the task */
//                     &TaskTiming,    /* Task handle to keep track of created task */
//                     0);        /* pin task to core 0 */
          delay(10); // Delay needed for sending the data correctly
          dataToSend = "";
          microNow = 0;
        }
//        delay(100);
//        Serial.println("webSocketLoop: " + String(xPortGetCoreID()));
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
