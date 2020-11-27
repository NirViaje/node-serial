
//const char BYTES_NUM 2
//const char HOST_ADDR 3
//const char TARGET_ADDR 4
//const char CUR_CTR 7
//const char CUR_TMP 8
//const char CHECK_SUM 9

//ON
// F7F807000519810500ABFD
//OFF
// F7F807000519810A00B0FD
//STOP
// F7F807000519810F00B5FD

HardwareSerial Serial485(2); //pin16/17, RXD/TXD

const int bytesLength = 11; //uint8_tx

enum BroadCurtainBytesSetting {Head0, Head1, BYTES_NUM, 
                              HOST_ADDR, TARGET_ADDR, TYPE_CODE, FUNC_CODE, 
                              CURT_CTR, CURT_TMP, CHECK_SUM, END_CODE};

class Broad485Device {
  public:
  
    uint8_t theBroadBytesFrame[bytesLength] = {0xF7, 0xF8, 0x07, 
                                        0x00, 0x05, 
                                        0x19, 0x81, 
                                        0x05, 0x00, 
                                        0xAB, 0xFD};
    uint8_t theCurtainOpenFrame[bytesLength] = {0xF7, 0xF8, 0x07, 
                                        0x00, 0x05, 0x19, 
                                        0x81, 0x05, 0x00, 
                                        0xAB, 0xFD};
    uint8_t theCurtainCloseFrame[bytesLength] = {0xF7, 0xF8, 0x07, 
                                        0x00, 0x05, 0x19, 
                                        0x81, 0x0A, 0x00, 
                                        0xB0, 0xFD};
    uint8_t theCurtainStopFrame[bytesLength] = {0xF7, 0xF8, 0x07, 
                                        0x00, 0x05, 0x19, 
                                        0x81, 0x0F, 0x00, 
                                        0xB5, 0xFD};
    
    enum CurtainActions {curtVoid, curtON, curtOFF, curtSTOP};
    enum CurtainType {curtInner, curtOuter};  //motor1/2
    
    Broad485Device() {
      Serial485.begin(9600);
      while (!Serial485) {
        ; // wait for serial port to connect. Needed for native USB port only
      }
//      pinMode(PIN, INPUT_PULLUP);
//      attachInterrupt(PIN, std::bind(&Button::isr,this), FALLING);
    };
    ~Broad485Device() {
//      detachInterrupt(PIN);
    }
    
    bool setCurtainAddr(uint8_t addr) {
      if(addr > 0x0F) { //0x50
        return -1;
      }
      
      ;
    }
    
    void setCurtainOpen() {
      setCurtainCtrByte(curtOuter, curtON); //Motor2
      Serial485.write(theCurtainOpenFrame, bytesLength); //ON
//      Serial.write(theBroadBytesFrame, bytesLength);
    }
    void setCurtainClosed() {
      setCurtainCtrByte(curtOuter, curtOFF);
      printHexArray(theBroadBytesFrame, bytesLength);
      Serial.println(theBroadBytesFrame[CURT_CTR], BIN);
      Serial485.write(theBroadBytesFrame, bytesLength);
    }
    void setCurtainStop() {
      setCurtainCtrByte(curtInner, curtSTOP);
      Serial485.write(theCurtainStopFrame, bytesLength);
    }
    void setCurtainAction(uint8_t whichCurtain, uint8_t Action) {
      setCurtainCtrByte(whichCurtain, Action);
      Serial485.write(theBroadBytesFrame, bytesLength);
    }

    void setBroadFrameByte() {
      ;
    }
    void setCurtainCtrByte(bool whichCurtain, uint8_t Action){ //enum CurtainActions

      uint8_t byteMask = 0x03 << (!whichCurtain << 1); //00000011b << 0 or 2
      byteMask += 0xF0;

      theBroadBytesFrame[CURT_CTR] &= byteMask;
      theBroadBytesFrame[CURT_CTR] |= (Action << (whichCurtain << 1));
//      Serial.println(String(theBroadBytesFrame[CURT_CTR], BIN)+", "+String(theBroadBytesFrame[CURT_CTR], HEX)+" # += (Action");

      checksumSend2Curtain();
      
//      Serial.println("theBroadBytesFrame[CURT_CTR] = " + String(theBroadBytesFrame[CURT_CTR], BIN));
    }
    void setCurtainTmpByte();
    
    uint8_t checksumSend2Curtain() {
      uint8_t checksum = 0;

      for(int i = BYTES_NUM; i < CHECK_SUM; i++) {
        checksum += theBroadBytesFrame[i];
//        Serial.printf("i, checksum, [BYTES_NUM+i]: %d, 0x%02X, 0x%02X\n", i, checksum, theBroadBytesFrame[i]);
      }
      theBroadBytesFrame[CHECK_SUM] = checksum; //
      
      return checksum;
    }

    void printHex(uint8_t num) {
      char hexCar[2];
    
      sprintf(hexCar, "%02X", num);
      Serial.print(hexCar); //return String
    }
    void printHexArray(uint8_t b[], int arrayLength) {
      for(int i = 0; i < arrayLength; i++){ //sizeof(b)
        printHex(b[i]);
      }
      Serial.println(); //return String
    }

    void getCurtainDataParsed() {
      ;
    }

//    struct CurtainCtrByte {
//      uint8_t a:8;
//      uint8_t b:2;
//      uint8_t c:6;
//    }data; 
};


//struct sBytesBroadFrame {
//  ;
//} bytesBroadFrame;
