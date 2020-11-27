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

  pinMode(pinLDR, INPUT_PULLUP);
  sensors.begin();
  
  // Grab a count of devices on the wire
  int numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  sensors.getAddress(tempDeviceAddress, 0);
  Serial.print(" with address: ");
  printAddress(tempDeviceAddress);
  
}

void loop() {
  
//  mqttClient.loop();

//  Serial.printf("\tanalogRead.pinLDR: %d,\t", analogRead(pinLDR));

  mqttLoop();

  oneWireBus;

//  sensors.requestTemperatures(); 
//  float temperatureC = sensors.getTempCByIndex(0);
////  float temperatureF = sensors.getTempFByIndex(0);
//  Serial.print(temperatureC);
//  Serial.println("ºC");
  
//  Serial.println("==========setCurtainOpen==========");
  BroadCurt.setCurtainOpen();
  delay(1000);

//  Serial.println("==========setCurtainClosed==========");
  BroadCurt.setCurtainClosed();
  delay(1000);

}
