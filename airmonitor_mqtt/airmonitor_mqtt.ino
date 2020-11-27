#include <Arduino.h>

#include <WiFiManager.h>
#include "aliyun_iot.h"
#include "sensor.h"

const char *ssid = "bcore-zxm";
const char *password = "bcore6688";

void setup() {
    Serial.begin(115200);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
//    // 接入 WiFi 网络
//    WiFiManager wifiManager;
//    wifiManager.autoConnect("bcore-lab", "bcore6688");  //bcore-lab
//    Serial.println("Connected to internet.\n");

    // 初始化连接参数
    String productKey = F("a1YjdAyDSfF");
    String deviceName = F("iw3Ae0COUvn66mhfEK1t");
    String deviceSecret = F("868698aefb9054d7008aaa915569fe41");
    AliyunIoT_Init(productKey, deviceName, deviceSecret);

    // 建立连接
    AliyunIoT_Connect();
}

unsigned long postSensorDataTimer = 0;
#define POST_SENSOR_DATA_INTERVAL   60

void postSensorData() {
    int pm25 = sensor_get_pm25();
    int pm1 = sensor_get_pm1();
    int pm10 = sensor_get_pm10();
    float temperature = sensor_get_temperature();
    int humidity = sensor_get_humidity();
    String props = F("\"PM25Value\":");
    props += pm25;
    props += ",\"PM1\":";
    props += pm1;
    props += ",\"PM10\":";
    props += pm10;
    props += ",\"CurrentTemperature\":";
    props += temperature;
    props += ",\"RelativeHumidity\":";
    props += humidity;
    AliyunIoT_PostProperty(props);
}

void loop() {
    if (millis() - postSensorDataTimer > POST_SENSOR_DATA_INTERVAL*1000) {
        if (AliyunIoT_CheckConnection()) {
            Serial.println("Start sending sensor data...");
            postSensorData();
        }
        postSensorDataTimer = millis();
    }

    AliyunIoT_Loop();
    delay(1);
}
