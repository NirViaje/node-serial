#include <WiFi.h>
#include <PubSubClient.h>

#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>

#include "SensorsActuators.h"
#include "WebPages.h"

enum CurtainActions {curtVoid, curtON, curtOFF, curtSTOP};

CurtainActions mqttCurtainActions = curtVoid;

const char *ssid = "bcore-zxm";
const char *password = "bcore6688";

//IPAddress server(47, 115, 63, 71);
unsigned long postSensorDataTimer = 0;
#define POST_SENSOR_DATA_INTERVAL   1

int chKeepAlive = 30;

static String mqttBroker;
static IPAddress mqttBrokerIP;
static String mqttClientID;
static String mqttUsername;
static String mqttPassword;
static String mqttPostPropertyTopic;

// Aliyun IoT
static String productKey;
static String deviceName;
static String deviceSecret;

static WiFiClient mqttWiFiClient;
WebServer server(80);

const char* host = "5d";

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    mqttCurtainActions = curtON;
    Serial.println("setCurtainOpen.");
    //    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else if ((char)payload[0] == '0') {
    mqttCurtainActions = curtOFF;
    Serial.println("setCurtainClosed.");
    //    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  } else {
    mqttCurtainActions = curtSTOP;
    Serial.println("setCurtainStop.");
  }
}

PubSubClient mqttClient("abi.bsbcore.com", 1883, mqttCallback, mqttWiFiClient); //server
//PubSubClient mqttClient(mqttWiFiClient);

//PubSubClient *pMqttClient;

void webUpdateServer() {
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", loginIndex);
  });
  server.on("/serverIndex", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}

void WiFiSetup() {

  Serial.print("MAC Address of this device:  ");
  Serial.println(WiFi.macAddress());

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

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");

  webUpdateServer();
}

void mqttClientSetup() {

  // Note - the default maximum packet size is 128 bytes. If the
  // combined length of clientId, username and password exceed this use the
  // following to increase the buffer size:
  // client.setBufferSize(255);

  mqttClient.setBufferSize(2048);
  mqttClient.setKeepAlive(chKeepAlive);

  String host = mqttBroker;
  WiFi.hostByName(host.c_str(), mqttBrokerIP);

  char result = mqttClient.connect("abi.bsbcore.com", "bsbcore", "broad888");  //mqttBrokerIP

  String log = F("MQTTclient.connect result: ");
  log += result;
  log += F("\nMQTTclient.state(): ");
  log += mqttClient.state();

  Serial.println(log);

  if (result) {
    Serial.println("Connected to MQTT.");
    //      break;
  } else {
    String log = F("Connect to MQTT failed with state ");
    log += mqttClient.state();
    Serial.println(log);
  }

  if (result) { //client.connect("abi.bsbcore.com", "bsbcore", "broad888")) {
    mqttClient.publish("outTopic", "hello world");
    mqttClient.subscribe("inTopic");
  }
}

bool mqtt_Connect(PubSubClient *pMqttClient) {
  ;
}
bool mqtt_Connect() {

  //    AliyunMQTTclient.setServer("a1YjdAyDSfF.iot-as-mqtt.cn-shanghai.aliyuncs.com", 1883);
  //    mqttClient.setServer(mqttBrokerIP, 1883);

  mqttClient.setBufferSize(2048);
  mqttClient.setKeepAlive(chKeepAlive);

  Serial.println("Start connecting to mqtt...");

  bool result = false;
  for (byte i = 0; i < 3; ++i) {
    result = mqttClient.connect("abi.bsbcore.com", "bsbcore", "broad888");
    //        result = AliyunMQTTclient.connect(
    ////                                          "2020|securemode=3,signmethod=hmacmd5|",
    ////                                          "iw3Ae0COUvn66mhfEK1t&a1YjdAyDSfF",
    ////                                          "2aa65ab192b6ed219e754ce64524f0c3");
    //                                          "01234567890123456789012",  //mqttClientID.c_str(),
    //                                          mqttUsername.c_str(),
    //                                          mqttPassword.c_str());

    String log = F("MQTTclient.connect result: ");
    log += result;
    log += F("\nMQTTclient.state(): ");
    log += mqttClient.state();
    Serial.println(log);
    if (result) {
      Serial.println("Connected to MQTT.");
//      mqttClient.publish("outTopic", "hello world");
      mqttClient.subscribe("inTopic");
      break;
    } else {
      String log = F("Connect to MQTT failed with state ");
      log += mqttClient.state();
      Serial.println(log);
    }
    delay(500);
  }

  return result;
}
//void mqtt_Disconnect() {
//  mqttClient.disconnect();
//}
bool mqtt_CheckConnection() {
  if (!mqttClient.connected()) {
    mqttClient.disconnect();
    Serial.println("MQTT connection lost, reconnecting...");
    mqtt_Connect();
  }

  return mqttClient.connected();
}

String mqtt_BuildPayload(String& props) {
  String payload = F("{\"timestampINms\":\"");
  payload += millis();
  payload += F("\",\"version\":\"1.0\",\"method\":\"thing.event.property.post\",\"params\":{");
  payload += props;
  payload += F("}}");
  return payload;
}
bool mqtt_PostProperty(String& props) {

  // topic
  if (mqttPostPropertyTopic.length() == 0) {
    //        String topic = F("/sys/%pk%/%dn%/thing/event/property/post");
    //        topic.replace("%pk%", productKey);
    //        topic.replace("%dn%", deviceName);
    String topic = F("/private/bcore-lab/SmartCurtain[B5-3D-CC]Test/post");
    mqttPostPropertyTopic = topic;
  }

  bool result = false;
  if (mqtt_CheckConnection()) {
    String payload = mqtt_BuildPayload(props);
    result = mqttClient.publish(mqttPostPropertyTopic.c_str(), payload.c_str(), 0);
    Serial.println("Post PropertyTopic and property payload:");
    Serial.println(mqttPostPropertyTopic + "\n" + payload);
  }

  return result;
}
void postSensorData() {

  sensors18B20.requestTemperatures();
  float RoomTemperature = sensors18B20.getTempCByIndex(0);
  //  float temperatureF = sensors18B20.getTempFByIndex(0);
  Serial.print(RoomTemperature);
  Serial.println("ºC");
  
  float WindowTemperature = sensors18B20.getTempCByIndex(1);
  //  float temperatureF = sensors18B20.getTempFByIndex(0);
  Serial.print(WindowTemperature);
  Serial.println("ºC");

  int ambientLight = analogRead(pinLDR);

  //    int pm25 = sensor_get_pm25();
  //    int pm1 = sensor_get_pm1();
  //    int pm10 = sensor_get_pm10();
  //    float temperature = sensor_get_temperature();
  //    int humidity = sensor_get_humidity();
  //    String props = F("\"PM25Value\":");
  //    props += pm25;
  //    props += ",\"PM1\":";
  //    props += pm1;
  //    props += ",\"PM10\":";
  //    props += pm10;
  //    props += ",\"CurrentTemperature\":";

  String props = F("\"RoomTemperature\":");
  props += RoomTemperature;
  props += ",\"WindowTemperature\":";
  props += WindowTemperature;
  props += ",\"ambientLight\":";
  props += ambientLight;
  props += ",\"DigitalPIR\":";
  props += digitalRead(pinDigitalPIR);
  props += ",\"AnalogLGPIR\":";
  props += analogRead(pinLowGainPIR);
  props += ",\"AnalogHGPIR\":";
  props += analogRead(pinHighGainPIR);
  props += ",\"CurtainStatus\":";
  props += "Invalid";
  //    props += ",\"RelativeHumidity\":";
  //    props += humidity;
  mqtt_PostProperty(props);
}

void mqttLoop() {
  if (millis() - postSensorDataTimer > POST_SENSOR_DATA_INTERVAL * 1000) {
    if (mqtt_CheckConnection()) {
      Serial.println("Start sending sensor data...");
      postSensorData();
    }
    postSensorDataTimer = millis();
  }

  mqttClient.loop();
  delay(1);

  server.handleClient();
  delay(1);
}
