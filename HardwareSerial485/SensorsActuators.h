#include <OneWire.h>
#include <DallasTemperature.h>

#include "FastLED.h"

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

const int oneWireBus = 23;
const int pinLDR = 33;

const int pinLowGainPIR = 32;
const int pinHighGainPIR = 35;  //
const int pinDigitalPIR = 34;

const int ws2812DI = 18;  //
const int ws2812BI = 19;

#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    200
#define BRIGHTNESS  255

CRGB leds[NUM_LEDS];

OneWire oneWire(oneWireBus);
DallasTemperature sensors18B20(&oneWire);

void WS2812Setup();

//Inner, outer
class TempSensors {

  public:
    TempSensors() {
      sensors18B20.begin();
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

void SensorsActuatorsSetup() {
  
  pinMode(pinLDR, INPUT_PULLUP);
  sensors18B20.begin();

  pinMode(pinLowGainPIR, INPUT);
  pinMode(pinHighGainPIR, INPUT);
  pinMode(pinDigitalPIR, INPUT_PULLUP);

  WS2812Setup();
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

void WS2812Setup() {
  delay(300); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,ws2812DI,COLOR_ORDER>(leds, NUM_LEDS)
    .setCorrection(TypicalLEDStrip)
    .setDither(BRIGHTNESS < 255);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}
// This function draws rainbows with an ever-changing,
// widely-varying set of parameters.
void pride() {
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 1, 3000);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < NUM_LEDS; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = analogRead(pinHighGainPIR) >> 4;  //hue16 / 256;

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    CRGB newcolor = CHSV( hue8, sat8, bri8);
    
    uint16_t pixelnumber = i;
    pixelnumber = (NUM_LEDS-1) - pixelnumber;
    
    nblend( leds[pixelnumber], newcolor, 64);
  }
}
void FastLEDloop() {
  pride();
  FastLED.show();  
}
