
#include <Adafruit_NeoPixel.h>

class Strip
{
public:
  uint8_t   effect;
  uint8_t   effects;
  uint16_t  effStep;
  unsigned long effStart;
  Adafruit_NeoPixel strip;
  Strip(uint16_t leds, uint8_t pin, uint8_t toteffects, uint16_t striptype) : strip(leds, pin, striptype) {
    effect = -1;
    effects = toteffects;
    Reset();
  }
  void Reset(){
    effStep = 0;
    effect = (effect + 1) % effects;
    effStart = millis();
  }
};

struct Loop
{
  uint8_t currentChild;
  uint8_t childs;
  bool timeBased;
  uint16_t cycles;
  uint16_t currentTime;
  Loop(uint8_t totchilds, bool timebased, uint16_t tottime) {currentTime=0;currentChild=0;childs=totchilds;timeBased=timebased;cycles=tottime;}
};

Strip strip_0(60, 8, 60, NEO_GRB + NEO_KHZ800);
Strip strip_1(60, 9, 60, NEO_GRB + NEO_KHZ800);
Strip strip_2(60, 10, 60, NEO_GRB + NEO_KHZ800);
Strip strip_3(60, 11, 60, NEO_GRB + NEO_KHZ800);
Strip strip_4(60, 12, 60, NEO_GRB + NEO_KHZ800);
Strip strip_5(60, 13, 60, NEO_GRB + NEO_KHZ800);
Strip strip_6(60, 14, 60, NEO_GRB + NEO_KHZ800);
Strip strip_7(60, 15, 60, NEO_GRB + NEO_KHZ800);
Strip strip_8(60, 16, 60, NEO_GRB + NEO_KHZ800);
struct Loop strip0loop0(2, false, 1);
struct Loop strip1loop0(0, false, 1);
struct Loop strip2loop0(0, false, 1);
struct Loop strip3loop0(1, false, 1);
struct Loop strip4loop0(2, false, 1);
struct Loop strip4loop00(3, false, 1);
struct Loop strip5loop0(1, false, 1);
struct Loop strip6loop0(0, false, 1);
struct Loop strip7loop0(0, false, 1);
struct Loop strip8loop0(2, false, 1);

//[GLOBAL_VARIABLES]

void setup() {

  //Your setup here:

  strip_0.strip.begin();
  strip_1.strip.begin();
  strip_2.strip.begin();
  strip_3.strip.begin();
  strip_4.strip.begin();
  strip_5.strip.begin();
  strip_6.strip.begin();
  strip_7.strip.begin();
  strip_8.strip.begin();
}

void loop() {

  //Your code here:

  strips_loop();
}

void strips_loop() {
  if(strip0_loop0() & 0x01)
    strip_0.strip.show();
  if(strip1_loop0() & 0x01)
    strip_1.strip.show();
  if(strip2_loop0() & 0x01)
    strip_2.strip.show();
  if(strip3_loop0() & 0x01)
    strip_3.strip.show();
  if(strip4_loop0() & 0x01)
    strip_4.strip.show();
  if(strip5_loop0() & 0x01)
    strip_5.strip.show();
  if(strip6_loop0() & 0x01)
    strip_6.strip.show();
  if(strip7_loop0() & 0x01)
    strip_7.strip.show();
  if(strip8_loop0() & 0x01)
    strip_8.strip.show();
}

uint8_t strip0_loop0() {
  uint8_t ret = 0x00;
  switch(strip0loop0.currentChild) {
    case 0: 
           ret = strip0_loop0_eff0();break;
    case 1: 
           ret = strip0_loop0_eff1();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip0loop0.currentChild + 1 >= strip0loop0.childs) {
      strip0loop0.currentChild = 0;
      if(++strip0loop0.currentTime >= strip0loop0.cycles) {strip0loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip0loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip0_loop0_eff0() {
    // Strip ID: 0 - Effect: Move - LEDS: 60
    // Steps: 60 - Delay: 50
    // Colors: 0 ()
    // Options: toLeft=true, rotate=true, 
  if(millis() - strip_0.effStart < 50 * (strip_0.effStep)) return 0x00;
  uint32_t c = strip_0.strip.getPixelColor(0);
  for(uint16_t j=0;j<59;j++) 
    strip_0.strip.setPixelColor(j, strip_0.strip.getPixelColor(j+1)); 
  strip_0.strip.setPixelColor(59, c); 
  if(strip_0.effStep >= 60) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip0_loop0_eff1() {
    // Strip ID: 0 - Effect: Rainbow - LEDS: 60
    // Steps: 60 - Delay: 20
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=60, toLeft=true, 
  if(millis() - strip_0.effStart < 20 * (strip_0.effStep)) return 0x00;
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<60;j++) {
    ind = strip_0.effStep + j * 1;
    switch((int)((ind % 60) / 20)) {
      case 0: factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
              factor2 = (float)((int)(ind - 0) % 60) / 20;
              strip_0.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
              factor2 = (float)((int)(ind - 20) % 60) / 20;
              strip_0.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
              factor2 = (float)((int)(ind - 40) % 60) / 20;
              strip_0.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              break;
    }
  }
  if(strip_0.effStep >= 60) {strip_0.Reset(); return 0x03; }
  else strip_0.effStep++;
  return 0x01;
}

uint8_t strip1_loop0() {
  uint8_t ret = 0x00;
  switch(strip1loop0.currentChild) {
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip1loop0.currentChild + 1 >= strip1loop0.childs) {
      strip1loop0.currentChild = 0;
      if(++strip1loop0.currentTime >= strip1loop0.cycles) {strip1loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip1loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip2_loop0() {
  uint8_t ret = 0x00;
  switch(strip2loop0.currentChild) {
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip2loop0.currentChild + 1 >= strip2loop0.childs) {
      strip2loop0.currentChild = 0;
      if(++strip2loop0.currentTime >= strip2loop0.cycles) {strip2loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip2loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip3_loop0() {
  uint8_t ret = 0x00;
  switch(strip3loop0.currentChild) {
    case 0: 
           ret = strip3_loop0_eff0();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip3loop0.currentChild + 1 >= strip3loop0.childs) {
      strip3loop0.currentChild = 0;
      if(++strip3loop0.currentTime >= strip3loop0.cycles) {strip3loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip3loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip3_loop0_eff0() {
    // Strip ID: 3 - Effect: Fade - LEDS: 60
    // Steps: 20 - Delay: 5
    // Colors: 2 (0.0.0, 255.255.255)
    // Options: duration=100, every=1, 
  if(millis() - strip_3.effStart < 5 * (strip_3.effStep)) return 0x00;
  uint8_t r,g,b;
  double e;
  e = (strip_3.effStep * 5) / (double)100;
  r = ( e ) * 255 + 0 * ( 1.0 - e );
  g = ( e ) * 255 + 0 * ( 1.0 - e );
  b = ( e ) * 255 + 0 * ( 1.0 - e );
  for(uint16_t j=0;j<60;j++) {
    if((j % 1) == 0)
      strip_3.strip.setPixelColor(j, r, g, b);
    else
      strip_3.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_3.effStep >= 20) {strip_3.Reset(); return 0x03; }
  else strip_3.effStep++;
  return 0x01;
}

uint8_t strip4_loop0() {
  uint8_t ret = 0x00;
  switch(strip4loop0.currentChild) {
    case 0: 
           ret = strip4_loop0_eff0();break;
    case 1: 
           ret = strip4_loop00();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip4loop0.currentChild + 1 >= strip4loop0.childs) {
      strip4loop0.currentChild = 0;
      if(++strip4loop0.currentTime >= strip4loop0.cycles) {strip4loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip4loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip4_loop0_eff0() {
    // Strip ID: 4 - Effect: Move - LEDS: 60
    // Steps: 60 - Delay: 50
    // Colors: 0 ()
    // Options: toLeft=true, rotate=true, 
  if(millis() - strip_4.effStart < 50 * (strip_4.effStep)) return 0x00;
  uint32_t c = strip_4.strip.getPixelColor(0);
  for(uint16_t j=0;j<59;j++) 
    strip_4.strip.setPixelColor(j, strip_4.strip.getPixelColor(j+1)); 
  strip_4.strip.setPixelColor(59, c); 
  if(strip_4.effStep >= 60) {strip_4.Reset(); return 0x03; }
  else strip_4.effStep++;
  return 0x01;
}

uint8_t strip4_loop00() {
  uint8_t ret = 0x00;
  switch(strip4loop00.currentChild) {
    case 0: 
           ret = strip4_loop00_eff0();break;
    case 1: 
           ret = strip4_loop00_eff1();break;
    case 2: 
           ret = strip4_loop00_eff2();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip4loop00.currentChild + 1 >= strip4loop00.childs) {
      strip4loop00.currentChild = 0;
      if(++strip4loop00.currentTime >= strip4loop00.cycles) {strip4loop00.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip4loop00.currentChild++;
    }
  };
  return ret;
}

uint8_t strip4_loop00_eff0() {
    // Strip ID: 4 - Effect: Blink - LEDS: 60
    // Steps: 36 - Delay: 5
    // Colors: 2 (66.121.172, 138.255.255)
    // Options: timeBegin=50, timeToOn=10, timeOn=10, timeToOff=10, timeOver=100, every=1, 
  if(millis() - strip_4.effStart < 5 * (strip_4.effStep)) return 0x00;
  uint8_t e,r,g,b;
  if(strip_4.effStep < 10) {
    for(uint16_t j=0;j<60;j++) 
      strip_4.strip.setPixelColor(j, 66, 121, 172);
  }
  else if(strip_4.effStep  < 12) {
    e = (strip_4.effStep * 5) - 50;
    r = 138 * ( e / 10 ) + 66 * ( 1.0 - e / 10 );
    g = 255 * ( e / 10 ) + 121 * ( 1.0 - e / 10 );
    b = 255 * ( e / 10 ) + 172 * ( 1.0 - e / 10 );
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_4.strip.setPixelColor(j, r, g, b);
      else strip_4.strip.setPixelColor(j, 66, 121, 172);
  }
  else if(strip_4.effStep < 14) {
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_4.strip.setPixelColor(j, 138, 255, 255);
      else strip_4.strip.setPixelColor(j, 66, 121, 172);
  }
  else if(strip_4.effStep < 16) {
    e = (strip_4.effStep * 5) - 70;
    r = 66 * ( e / 10 ) + 138 * ( 1.0 - e / 10 );
    g = 121 * ( e / 10 ) + 255 * ( 1.0 - e / 10 );
    b = 172 * ( e / 10 ) + 255 * ( 1.0 - e / 10 );
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_4.strip.setPixelColor(j, r, g, b);
      else strip_4.strip.setPixelColor(j, 66, 121, 172);
  }
  else {
    for(uint16_t j=0;j<60;j++) 
      strip_4.strip.setPixelColor(j, 66, 121, 172);
  }
  if(strip_4.effStep >= 36) {strip_4.Reset(); return 0x03; }
  else strip_4.effStep++;
  return 0x01;
}

uint8_t strip4_loop00_eff1() {
    // Strip ID: 4 - Effect: Rainbow - LEDS: 60
    // Steps: 60 - Delay: 20
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=60, toLeft=true, 
  if(millis() - strip_4.effStart < 20 * (strip_4.effStep)) return 0x00;
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<60;j++) {
    ind = strip_4.effStep + j * 1;
    switch((int)((ind % 60) / 20)) {
      case 0: factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
              factor2 = (float)((int)(ind - 0) % 60) / 20;
              strip_4.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
              factor2 = (float)((int)(ind - 20) % 60) / 20;
              strip_4.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
              factor2 = (float)((int)(ind - 40) % 60) / 20;
              strip_4.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              break;
    }
  }
  if(strip_4.effStep >= 60) {strip_4.Reset(); return 0x03; }
  else strip_4.effStep++;
  return 0x01;
}

uint8_t strip4_loop00_eff2() {
    // Strip ID: 4 - Effect: Move - LEDS: 60
    // Steps: 60 - Delay: 50
    // Colors: 0 ()
    // Options: toLeft=false, rotate=true, 
  if(millis() - strip_4.effStart < 50 * (strip_4.effStep)) return 0x00;
  uint32_t c = strip_4.strip.getPixelColor(59);
  for(uint16_t j=60-1;j>0;j--) 
    strip_4.strip.setPixelColor(j, strip_4.strip.getPixelColor(j-1)); 
  strip_4.strip.setPixelColor(0, c); 
  if(strip_4.effStep >= 60) {strip_4.Reset(); return 0x03; }
  else strip_4.effStep++;
  return 0x01;
}

uint8_t strip5_loop0() {
  uint8_t ret = 0x00;
  switch(strip5loop0.currentChild) {
    case 0: 
           ret = strip5_loop0_eff0();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip5loop0.currentChild + 1 >= strip5loop0.childs) {
      strip5loop0.currentChild = 0;
      if(++strip5loop0.currentTime >= strip5loop0.cycles) {strip5loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip5loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip5_loop0_eff0() {
    // Strip ID: 5 - Effect: Fade - LEDS: 60
    // Steps: 20 - Delay: 5
    // Colors: 2 (0.0.0, 255.255.255)
    // Options: duration=100, every=1, 
  if(millis() - strip_5.effStart < 5 * (strip_5.effStep)) return 0x00;
  uint8_t r,g,b;
  double e;
  e = (strip_5.effStep * 5) / (double)100;
  r = ( e ) * 255 + 0 * ( 1.0 - e );
  g = ( e ) * 255 + 0 * ( 1.0 - e );
  b = ( e ) * 255 + 0 * ( 1.0 - e );
  for(uint16_t j=0;j<60;j++) {
    if((j % 1) == 0)
      strip_5.strip.setPixelColor(j, r, g, b);
    else
      strip_5.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_5.effStep >= 20) {strip_5.Reset(); return 0x03; }
  else strip_5.effStep++;
  return 0x01;
}

uint8_t strip6_loop0() {
  uint8_t ret = 0x00;
  switch(strip6loop0.currentChild) {
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip6loop0.currentChild + 1 >= strip6loop0.childs) {
      strip6loop0.currentChild = 0;
      if(++strip6loop0.currentTime >= strip6loop0.cycles) {strip6loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip6loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip7_loop0() {
  uint8_t ret = 0x00;
  switch(strip7loop0.currentChild) {
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip7loop0.currentChild + 1 >= strip7loop0.childs) {
      strip7loop0.currentChild = 0;
      if(++strip7loop0.currentTime >= strip7loop0.cycles) {strip7loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip7loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip8_loop0() {
  uint8_t ret = 0x00;
  switch(strip8loop0.currentChild) {
    case 0: 
           ret = strip8_loop0_eff0();break;
    case 1: 
           ret = strip8_loop0_eff1();break;
  }
  if(ret & 0x02) {
    ret &= 0xfd;
    if(strip8loop0.currentChild + 1 >= strip8loop0.childs) {
      strip8loop0.currentChild = 0;
      if(++strip8loop0.currentTime >= strip8loop0.cycles) {strip8loop0.currentTime = 0; ret |= 0x02;}
    }
    else {
      strip8loop0.currentChild++;
    }
  };
  return ret;
}

uint8_t strip8_loop0_eff0() {
    // Strip ID: 8 - Effect: Rainbow - LEDS: 60
    // Steps: 60 - Delay: 20
    // Colors: 3 (255.0.0, 0.255.0, 0.0.255)
    // Options: rainbowlen=60, toLeft=true, 
  if(millis() - strip_8.effStart < 20 * (strip_8.effStep)) return 0x00;
  float factor1, factor2;
  uint16_t ind;
  for(uint16_t j=0;j<60;j++) {
    ind = strip_8.effStep + j * 1;
    switch((int)((ind % 60) / 20)) {
      case 0: factor1 = 1.0 - ((float)(ind % 60 - 0 * 20) / 20);
              factor2 = (float)((int)(ind - 0) % 60) / 20;
              strip_8.strip.setPixelColor(j, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2);
              break;
      case 1: factor1 = 1.0 - ((float)(ind % 60 - 1 * 20) / 20);
              factor2 = (float)((int)(ind - 20) % 60) / 20;
              strip_8.strip.setPixelColor(j, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2, 0 * factor1 + 255 * factor2);
              break;
      case 2: factor1 = 1.0 - ((float)(ind % 60 - 2 * 20) / 20);
              factor2 = (float)((int)(ind - 40) % 60) / 20;
              strip_8.strip.setPixelColor(j, 0 * factor1 + 255 * factor2, 0 * factor1 + 0 * factor2, 255 * factor1 + 0 * factor2);
              break;
    }
  }
  if(strip_8.effStep >= 60) {strip_8.Reset(); return 0x03; }
  else strip_8.effStep++;
  return 0x01;
}

uint8_t strip8_loop0_eff1() {
    // Strip ID: 8 - Effect: Blink - LEDS: 60
    // Steps: 46 - Delay: 5
    // Colors: 2 (0.0.0, 255.255.255)
    // Options: timeBegin=50, timeToOn=39, timeOn=31, timeToOff=10, timeOver=100, every=1, 
  if(millis() - strip_8.effStart < 5 * (strip_8.effStep)) return 0x00;
  uint8_t e,r,g,b;
  if(strip_8.effStep < 10) {
    for(uint16_t j=0;j<60;j++) 
      strip_8.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_8.effStep  < 17.8) {
    e = (strip_8.effStep * 5) - 50;
    r = 255 * ( e / 39 ) + 0 * ( 1.0 - e / 39 );
    g = 255 * ( e / 39 ) + 0 * ( 1.0 - e / 39 );
    b = 255 * ( e / 39 ) + 0 * ( 1.0 - e / 39 );
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_8.strip.setPixelColor(j, r, g, b);
      else strip_8.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_8.effStep < 24) {
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_8.strip.setPixelColor(j, 255, 255, 255);
      else strip_8.strip.setPixelColor(j, 0, 0, 0);
  }
  else if(strip_8.effStep < 26) {
    e = (strip_8.effStep * 5) - 120;
    r = 0 * ( e / 10 ) + 255 * ( 1.0 - e / 10 );
    g = 0 * ( e / 10 ) + 255 * ( 1.0 - e / 10 );
    b = 0 * ( e / 10 ) + 255 * ( 1.0 - e / 10 );
    for(uint16_t j=0;j<60;j++) 
      if((j%1)==0) strip_8.strip.setPixelColor(j, r, g, b);
      else strip_8.strip.setPixelColor(j, 0, 0, 0);
  }
  else {
    for(uint16_t j=0;j<60;j++) 
      strip_8.strip.setPixelColor(j, 0, 0, 0);
  }
  if(strip_8.effStep >= 46) {strip_8.Reset(); return 0x03; }
  else strip_8.effStep++;
  return 0x01;
}
