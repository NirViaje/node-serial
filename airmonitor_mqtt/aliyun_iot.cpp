#include "aliyun_iot.h"

#include <ArduinoCrypto.h>
#include <PubSubClient.h>
#include <WiFi.h>

static WiFiClient mqttWiFiClient;
static PubSubClient AliyunMQTTclient(mqttWiFiClient);

static String productKey;
static String deviceName;
static String deviceSecret;
static String mqttBroker;
static IPAddress mqttBrokerIP;
static String mqttClientID;
static String mqttUsername;
static String mqttPassword;
static String mqttPostPropertyTopic;

char chKeepAlive = 60;

void AliyunIoT_Init(String& pk, String& dn, String& ds) {
    productKey = pk;
    deviceName = dn;
    deviceSecret = ds;

    // 获取 MQTT 服务器地址
    // a1YjdAyDSfF.iot-as-mqtt.cn-shanghai.aliyuncs.com
    mqttBroker = F("%pk%.iot-as-mqtt.cn-shanghai.aliyuncs.com");
    mqttBroker.replace("%pk%", pk);
    mqttBroker = F("abi.bsbcore.com");
    String host = mqttBroker;
    host.toLowerCase();
    WiFi.hostByName(host.c_str(), mqttBrokerIP);

    // 生成 MQTT ClientID
    String timestamp = F("");
    timestamp += millis();
//    mqttClientID = F("%dn%&%pk%|securemode=3,signmethod=hmacsha256,timestamp=%timestamp%|");
    mqttClientID = F("2020|securemode=3,signmethod=hmacsha256|");
    mqttClientID.replace("%pk%", pk);
    mqttClientID.replace("%dn%", dn);
    mqttClientID.replace("%timestamp%", timestamp);

    // 生成 MQTT Username
    //  2020|securemode=3,signmethod=hmacmd5|
    //  iw3Ae0COUvn66mhfEK1t&a1YjdAyDSfF
    //https://www.freeformatter.com/hmac-generator.html
    //  clientId2020deviceNameiw3Ae0COUvn66mhfEK1tproductKeya1YjdAyDSfF
    //  2aa65ab192b6ed219e754ce64524f0c3
    mqttUsername = dn;
    mqttUsername += "&";
    mqttUsername += pk;

    // 生成 MQTT Password
    String signcontent = F("clientId");
    signcontent += dn + F("&") + pk + F("deviceName") + dn + F("productKey") + pk + F("timestamp") + timestamp;
    mqttPassword = AliyunIoT_GetSign(signcontent, ds);

    mqttClientID = "abi";
    mqttUsername = "bsbcore";
    mqttPassword = "broad888";  //broad123";  //""

    String log = F("MQTT options:\n");
    log += "Broker: " + mqttBroker + "\n";
    log += "BrokerIP: ";
    log += String(mqttBrokerIP[0]) + "." + String(mqttBrokerIP[1]) + "." + String(mqttBrokerIP[2]) + "." + String(mqttBrokerIP[3]) + F("\n");
    log += "ClientID: " + mqttClientID + "\n";
    log += "Username: " + mqttUsername + "\n";
    log += "signcontent: " + signcontent + "\n";
    log += "Password: " + mqttPassword + "\n";
    Serial.println(log);
}

//GetSign:
//clientIdiw3Ae0COUvn66mhfEK1t&a1YjdAyDSfFdeviceNameiw3Ae0COUvn66mhfEK1tproductKeya1YjdAyDSfFtimestamp1645
//7514cb3c1733d59c496ef00f6a8c73720ddfb41ca36ac6aa2f16f0fee24ebb8f
//
//Aliyun IoT options:
//Broker: a1YjdAyDSfF.iot-as-mqtt.cn-shanghai.aliyuncs.com
//BrokerIP: 101.133.195.226
//ClientID: iw3Ae0COUvn66mhfEK1t&a1YjdAyDSfF|securemode=3,signmethod=hmacsha256,timestamp=1645|
//Username: iw3Ae0COUvn66mhfEK1t&a1YjdAyDSfF
//Password: 7514cb3c1733d59c496ef00f6a8c73720ddfb41ca36ac6aa2f16f0fee24ebb8f

String AliyunIoT_GetSign(String& signcontent, String& ds) {
    SHA256HMAC hmac((const byte *)ds.c_str(), ds.length());
    hmac.doUpdate((const byte *)signcontent.c_str(), signcontent.length());
    byte authCode[SHA256HMAC_SIZE];
    hmac.doFinal(authCode);
    String sign = F("");
    for (byte i = 0; i < SHA256HMAC_SIZE; ++i) {
        if (authCode[i] < 0x10) {
            sign += F("0");
        }
        sign += String(authCode[i], HEX);
    }
    String log = F("GetSign:\n");
    log += signcontent + F("\n");
    log += sign + F("\n");
    Serial.println(log);
    return sign;
}

void AliyunIoT_MQTTCallback(char* c_topic, byte* b_payload, unsigned int length) {

}

bool AliyunIoT_Connect() {
    AliyunMQTTclient.setBufferSize(2048);
    AliyunMQTTclient.setKeepAlive(chKeepAlive);
    Serial.println("chKeepAlive: " + String(chKeepAlive, DEC));
//    chKeepAlive %= 60;
//    chKeepAlive++;
//    AliyunMQTTclient.setServer("a1YjdAyDSfF.iot-as-mqtt.cn-shanghai.aliyuncs.com", 1883);
    AliyunMQTTclient.setServer(mqttBrokerIP, 1883);
//    AliyunMQTTclient.setCallback(AliyunIoT_MQTTCallback);

    Serial.println("Start connecting to Aliyun IoT...");

    bool result = false;
    for (byte i = 0; i < 3; ++i) {
//      result = AliyunMQTTclient.connect("abi.bsbcore.com", "bsbcore", "broad888");
        result = AliyunMQTTclient.connect(
//                                          "2020|securemode=3,signmethod=hmacmd5|",
//                                          "iw3Ae0COUvn66mhfEK1t&a1YjdAyDSfF",
//                                          "2aa65ab192b6ed219e754ce64524f0c3");
//                                          "2020|securemode=3,signmeth",  //mqttClientID.c_str(),
                                          "01234567890123456789012",  //mqttClientID.c_str(),
                                          mqttUsername.c_str(),
                                          mqttPassword.c_str());

//        Serial.println("mqttClientID: " + String(mqttClientID));
//        Serial.println("mqttUsername: " + String(mqttUsername));
//        Serial.println("mqttPassword: " + String(mqttPassword));
        String log = F("MQTTclient.connect result: ");
            log += result;
            log += F("\nMQTTclient.state(): ");
            log += AliyunMQTTclient.state();
            Serial.println(log);
        if (result) {
            Serial.println("Connected to MQTT.");
            break;
        } else {
            String log = F("Connect to MQTT failed with state ");
            log += AliyunMQTTclient.state();
            Serial.println(log);
        }
        delay(500);
    }

    return result;
}

void AliyunIoT_Disconnect() {
  AliyunMQTTclient.disconnect();
}

bool AliyunIoT_CheckConnection() {
    if (!AliyunMQTTclient.connected()) {
        AliyunMQTTclient.disconnect();
        Serial.println("MQTT connection lost, reconnecting...");
        AliyunIoT_Connect();
    }

    return AliyunMQTTclient.connected();
}

String AliyunIoT_BuildPayload(String& props) {
    String payload = F("{\"id\":\"");
    payload += millis();
    payload += F("\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{");
    payload += props;
    payload += F("}}");
    return payload;
}

void AliyunIoT_Loop() {
    AliyunMQTTclient.loop();
}

bool AliyunIoT_PostProperty(String& props) {
    // 根据 Alink 协议，生成上报数据所使用的 topic
    if (mqttPostPropertyTopic.length() == 0) {
        String topic = F("/sys/%pk%/%dn%/thing/event/property/post");
        topic.replace("%pk%", productKey);
        topic.replace("%dn%", deviceName);
        mqttPostPropertyTopic = topic;
    }

    bool result = false;
    if (AliyunIoT_CheckConnection()) {
        String payload = AliyunIoT_BuildPayload(props);
        result = AliyunMQTTclient.publish(mqttPostPropertyTopic.c_str(), payload.c_str(), 0);
        Serial.println("Post PropertyTopic and property payload:");
        Serial.println(mqttPostPropertyTopic + "\n" + payload);
    }

    return result;
}
