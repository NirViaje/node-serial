#include <Arduino.h>
#include <Ticker.h>

//#include "SensorsActuators.h"
#include "Broad485.h"
#include "mqttClient.h"

DeviceAddress tempDeviceAddress; 
//TempSensors Tmp18B20();

Broad485Device BroadCurt;

Ticker mqttPub;

//() {
//  ;
//}

//void toggleBroadCurt() {
//  static bool isOpen = false;
//  if (isOpen) {
//    blinker.detach();
//    isOpen = false;
//  }
//  else {
//    blinker.attach(blinkerPace, blink);
//    isOpen = true;
//  }
//  digitalWrite(LED_PIN, LOW);  //make sure LED on on after toggling (pin LOW = led ON)
//}

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  WiFiSetup();
  mqttClientSetup();

  SensorsActuatorsSetup();
  
  // Grab a count of devices on the wire
  int numberOfDevices = sensors18B20.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  sensors18B20.getAddress(tempDeviceAddress, 0);
  Serial.print(" with outer sensor address: ");
  printAddress(tempDeviceAddress);
  
  sensors18B20.getAddress(tempDeviceAddress, 1);
  Serial.print("\tand with inner sensor address: ");
  printAddress(tempDeviceAddress);
}

void loop() {
  
  mqttLoop();

  if(mqttCurtainActions == curtON) {
    BroadCurt.setCurtainOpen();
    Serial.println("==========setCurtainOpen==========");
    mqttCurtainActions = curtVoid;
  } else if(mqttCurtainActions == curtOFF) {
    BroadCurt.setCurtainClosed();
    Serial.println("==========setCurtainClosed==========");
    mqttCurtainActions = curtVoid;
  } else if(mqttCurtainActions == curtSTOP) {
    BroadCurt.setCurtainStop();
    Serial.println("==========setCurtainSTOP==========");
    mqttCurtainActions = curtVoid;
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection Failed! Should be Rebooting...");
    delay(1000);
//    ESP.restart();
  }

  FastLEDloop();

  static uint32_t millisNow = millis();
  static uint32_t iTime = 0;
  if(millis() - millisNow > 1000) {
    Serial.println("millisNow: "+String(millisNow)+"\tmillis: "+String(millis())+"\tiTime: "+String(iTime));
    millisNow = millis();
  }
  iTime++;

}
