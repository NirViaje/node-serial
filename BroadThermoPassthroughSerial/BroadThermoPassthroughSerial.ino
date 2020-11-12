/*
  SerialPassthrough sketch

  Some boards, like the Arduino 101, the MKR1000, Zero, or the Micro, have one
  hardware serial port attached to Digital pins 0-1, and a separate USB serial
  port attached to the IDE Serial Monitor. This means that the "serial
  passthrough" which is possible with the Arduino UNO (commonly used to interact
  with devices/shields that require configuration via serial AT commands) will
  not work by default.

  This sketch allows you to emulate the serial passthrough behaviour. Any text
  you type in the IDE Serial monitor will be written out to the serial port on
  Digital pins 0 and 1, and vice-versa.

  On the 101, MKR1000, Zero, and Micro, "Serial" refers to the USB Serial port
  attached to the Serial Monitor, and "Serial1" refers to the hardware serial
  port attached to pins 0 and 1. This sketch will emulate Serial passthrough
  using those two Serial ports on the boards mentioned above, but you can change
  these names to connect any two serial ports on a board that has multiple ports.

  created 23 May 2016
  by Erik Nyquist
*/
#include <M5StickC.h>

#define SYNC_SHORT_BIT 0
#define SYNC_LONG_BIT 1
#define SYNC_LOW_RAIL 2
#define SYNC_HIGH_RAIL 3
#define SYNC_ERROR -1

long microNow = 0;
char pinIO = 26;
int iRise = 0;
int iFall = 0;

bool pinStatusPrev = LOW;
bool pinStatusCurr = LOW;
bool bRiseFall = LOW;

byte the8bits = 0;

char clockBit() {
  long SyncInterval = 0;
  long microInterval = 0;
  
  microNow = micros();
  while(digitalRead(pinIO)) {
    ;
  }
  microInterval = micros() - microNow;
  if(microInterval > 100) iFall++;
  if(microInterval > 400) {
    SyncInterval = microInterval;
    
    // 1.6ms low after 3.4ms high
    microNow = micros();
    while(!digitalRead(pinIO)) {
      ;
    }
    microInterval = micros() - microNow;
    iRise++;
    if(microInterval > 400) {
      SyncInterval += microInterval;
      // SyncInterval > 5.00 or 5.10ms
      if(SyncInterval > 850 && SyncInterval < 1300) {
        Serial.println("We start with SYNC_HIGH_RAIL!");
        return SYNC_HIGH_RAIL;
      }
      else if(SyncInterval > 4800 && SyncInterval < 5050) {
        Serial.println("We start with SYNC_LOW_RAIL!");
        return SYNC_LOW_RAIL;
      }
      else {
        Serial.println("SYNC_ERROR with microInterval, SyncInterval: " + String(microInterval) +", " + String(SyncInterval) + "us");
        return SYNC_ERROR;
      }
    }
    else {
      Serial.println("SYNC_ERROR with microInterval: " + String(microInterval) + "us");
      return SYNC_ERROR;
    }
  }
}
char syncBit() {
  long SyncInterval = 0;
  long microInterval = 0;
  
  //////////////////////////////////microNow = micros();
  while(digitalRead(pinIO)) {
    ;
  }
  microInterval = micros() - microNow;
  microNow = micros();
  if(microInterval > 100) iFall++;
  if(microInterval > 3200) {
    SyncInterval = microInterval;
    
    // 1.6ms low after 3.4ms high
    while(!digitalRead(pinIO)) {
      ;
    }
    microInterval = micros() - microNow;
    microNow = micros();
    iRise++;
    if(microInterval > 1500) {
      SyncInterval += microInterval;
      // SyncInterval > 5.00 or 5.10ms
      if(SyncInterval > 5050 && SyncInterval < 5300) {
        Serial.println("SYNC_HIGH_RAIL!");
        return SYNC_HIGH_RAIL;
      }
      else if(SyncInterval > 4800 && SyncInterval < 5050) {
        Serial.println("SYNC_LOW_RAIL!");
        return SYNC_LOW_RAIL;
      }
      else {
        Serial.println("SYNC_ERROR w m, S: " + String(microInterval) +", " + String(SyncInterval));
        return SYNC_ERROR;
      }
    }
    else {
      Serial.println("SYNC_ERROR w m: " + String(microInterval));
      return SYNC_ERROR;
    }
  }
  else if(microInterval > 480 && microInterval < 520) {
    SyncInterval = microInterval;
    
    // 1.6ms low after 3.4ms high
    while(!digitalRead(pinIO)) {
      ;
    }
    microInterval = micros() - microNow;
    microNow = micros();
    iRise++;
    if(microInterval > 1000 && microInterval < 1200) {
      SyncInterval += microInterval;
      // long bit
      return SYNC_LONG_BIT;
    }
    else if(microInterval > 280 && microInterval < 320) {
      SyncInterval += microInterval;
      // short bit
      return SYNC_SHORT_BIT;
    }
  }
  else {
    Serial.println("SYNC_ERROR with microInterval: " + String(microInterval) + "us");
    return SYNC_ERROR;
  }
}

void setup() {
  Serial.begin(115200);
//  Serial1.begin(9600);

  M5Setup();
  M5.Lcd.drawCentreString("SYNC_BIT/RAIL",0,0,2);
  M5.Lcd.drawCentreString("iRise/Fall: " + String(iRise) + ", " + String(iFall),0,20,2);
//  M5.Lcd.drawChar("SYNC_BIT/",0,0,2);

  pinMode(pinIO, INPUT);  //INPUT_PULLUP

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


  while(!bSync) {
    
    while(digitalRead(pinIO)) {
      ;
    }
    microHigh = micros() - microNow;
    microNow = micros();
  }
  while(!bSync) {
    
    while(digitalRead(pinIO)) {
      ;
    }
    microHigh = micros() - microNow;
    microNow = micros();
//    Serial.println("|_ ! C, I: " + String(++iFall) + ", " + String(microInterval));
    
    while(!digitalRead(pinIO)) {
      ;
    }
    microLow = micros() - microNow;
    microNow = micros();
    
//    Serial.println(String(microHigh) + ", " + String(microLow)); //"_| ! C, I, N: " + String(++iRise) + ", " + 

    microHighLow = microHigh + microLow;
    if(microHigh > 440 && microHigh < 565) {
    
      if(microLow > 240 && microLow < 360) {
        // short bit
        Serial.write("0");  //ln("_"); //SYNC_SHORT_BIT!");
        bitsCounter++;
//        return SYNC_SHORT_BIT;
      } else if(microLow > 1000 && microLow < 1200) {
        // long bit
        Serial.write("1");  //ln("__"); //SYNC_LONG_BIT!");
        bitsCounter++;
//        return SYNC_LONG_BIT;
      } else {
        Serial.println("ls T.O.: " + String(microLow));
      }
      
      if(bitsCounter == 8) {
        bitsCounter = 0;
        Serial.println("B" + String(bytesCounter));
        bytesCounter++;
      }
    } else if(microHigh > 3200 && microLow > 1500) {
      if(microHighLow < 5050) {
        bitsCounter = 0;
        bytesCounter = 0;
        Serial.println("\nH.R.: " + String(microHigh) + ", " + String(microLow) + ", " + String(microHighLow));  //SYNC_HIGH_RAIL
//        return SYNC_HIGH_RAIL;
      }
      else if(microHighLow < 5300) {
        bitsCounter = 0;
        bytesCounter = 0;
        Serial.println("\nL.R.: " + String(microHigh) + ", " + String(microLow));  //SYNC_Low_RAIL
//        return SYNC_HIGH_RAIL;
      } else {
        Serial.println("SYNC T.O.: " + String(microHighLow));
      }
    } else {
      Serial.println("T.O., mH&L: " + String(microHigh) + ", " + String(microLow)); //Out of any timing
    }
//    Serial.println(micros() - microNow);
    
    //
//    clockBit();
//    M5.Lcd.drawCentreString("iRise/Fall: " + String(iRise) + ", " + String(iFall),0,20,2);
    
    char bSyncStatus = 0;//syncBit();
    if(bSyncStatus == SYNC_LOW_RAIL) {
      bSync = true;
//      Serial.println("We start with SYNC_LOW_RAIL!");
    }
    else if(bSyncStatus == SYNC_HIGH_RAIL) {
      bSync = true;
//      Serial.println("We start with SYNC_HIGHT_RAIL!");
    }
  }
}

int bitsCounter = 0;
void loop() {

  char bSyncStatus = syncBit();
  if(bSyncStatus == SYNC_SHORT_BIT) {
    the8bits << 1;
    bitsCounter++;
  } else if(bSyncStatus == SYNC_LONG_BIT) {
    the8bits << 1;
    the8bits |= 0x01;
    bitsCounter++;
  } else if(bSyncStatus == SYNC_HIGH_RAIL || bSyncStatus == SYNC_LOW_RAIL) {
    ; //syncBit
    M5.Lcd.drawCentreString("iRise/Fall: " + String(iRise) + ", " + String(iFall),0,20,2);
  } else {
//    Serial.println("Neither SYNC_SHORT/LONG_BIT or SYNC_HIGH/LOW_RAIL, maybe we got trouble, bitsCounter: " + String(bitsCounter));
    bitsCounter = 0;
    M5.Lcd.drawCentreString("iRise/Fall: " + String(iRise) + ", " + String(iFall),0,20,2);
  }

  if(bitsCounter == 8) {
    Serial.println("the8bits: " + String(the8bits, BIN));
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.drawCentreString(String(the8bits, BIN),0,0,2);
    bitsCounter = 0;
  }
  
//  else if(syncBit() == SYNC_LOW_RAIL) {
//    Serial.println(SYNC_LOW_RAIL);
//  } else if(syncBit() == SYNC_HIGHT_RAIL) {
//    ;
//  }

//  pinStatusCurr = digitalRead(pinIO);
//  if(pinStatusCurr != pinStatusPrev) {
//    ;
//  }
  
//  if (Serial.available()) {      // If anything comes in Serial (USB),
//    Serial1.write(Serial.read());   // read it and send it out Serial1 (pins 0 & 1)
//  }
//
//  if (Serial1.available()) {     // If anything comes in Serial1 (pins 0 & 1)
//    Serial.write(Serial1.read());   // read it and send it out Serial (USB)
//  }
}
