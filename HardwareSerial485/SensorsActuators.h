#include <OneWire.h>
#include <DallasTemperature.h>

const int oneWireBus = 23;
const int pinLDR = 33;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

//Inner, outer
class TempSensors {

  public:
    TempSensors() {
      sensors.begin();
    }
    void Temp18B20() {
      ;
    }
    
    void TempThermoRes() {
      ;
    }
};

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++){
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}

//Inner, outer
class ambientLight {

  public:
    void ambientLDR();
};

void linearHall() {
  ;
}

void motionPIR() {
  ;
}

void BroadAC() {
  ;
}

//RMT, effects
//Change the interactive animation, change the theme
// https://www.digikey.com/en/maker/projects/neoanim-using-bitmaps-to-animate-neopixels-on-circuit-playground/a0c3e653a739480e9d2d0dcc7422d284
// https://adrianotiger.github.io/Neopixel-Effect-Generator/
class ws281xEffects {
  public:
  
    void ws281x() {
      ;
    }
};
