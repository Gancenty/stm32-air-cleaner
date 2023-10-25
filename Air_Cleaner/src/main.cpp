#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char *ssid = "wifi";
const char *password = "12345678";
const char *mqtt_server = "iot-06z00dfugqh7j96.mqtt.iothub.aliyuncs.com";
const int mqtt_port = 1883;
const char *mqtt_user = "ID0001&iljsG281vSb";
const char *mqtt_pass = "92a3927f54a46a36df802c5a684bfe34c91e0c7ddbe6711b0b8e7df14b067278";
const char *mqtt_clientId = "iljsG281vSb.ID0001|securemode=2,signmethod=hmacsha256,timestamp=1683440363914|";
const char *mqtt_sub_topic = "/broadcast/iljsG281vSb/ID0001";
const char *mqtt_pub_topic = "/broadcast/iljsG281vSb/APP";

#define MODE_STA_FUN  0X00

#define HUMI_STA_FUN  0X01
#define FAN_STA_FUN   0X02
#define VIOLED_STA_FUN 0X03

#define TEMP_DATA_FUN 0X04
#define HUMI_DATA_FUN 0X05
#define PM25_DATA_FUN 0X06
#define AIR_DATA_FUN 0X07

#define VIOLED_OFFTIME_FUN 0X08
#define RTC_TIME_FUN    	 0X09

#define P_TAIL 0XFF

typedef struct
{
  float temperature;
  float humidity;
  float pm25;
  float air_quality;
  uint8_t fan_sta;
  uint8_t violed_sta;
  uint8_t humi_sta;
  uint16_t violed_offtime;
  uint8_t mode;

  uint8_t all_updata;
} AIR_STA;

WiFiClient espClient;
PubSubClient client(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

#define BUFFER_SIZE (512)
uint8_t UART_RX[BUFFER_SIZE];
uint16_t rxlen = 0;
uint8_t UART_TX[BUFFER_SIZE];
uint16_t txlen = 0;
AIR_STA air;
StaticJsonDocument<256> doc;
String mqtt_upload_json;
uint32_t last_rtc_update=0;

void rtc_time_send(void)
{
  uint32_t now = timeClient.getEpochTime();
  UART_TX[0]=RTC_TIME_FUN;
  UART_TX[1]=now>>24;
  UART_TX[2]=now>>16;
  UART_TX[3]=now>>8;
  UART_TX[4]=now>>0;
  UART_TX[5]=P_TAIL;
  txlen=6;
  Serial.write(UART_TX,txlen);
  last_rtc_update=millis();
}
void setup_wifi()
{
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
void setup_time(void)
{
  timeClient.begin();
  timeClient.setTimeOffset(8 * 3600);
  while (timeClient.update() == false)
  {
    delay(250);
    digitalWrite(LED_BUILTIN, LOW);
    delay(250);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  rtc_time_send();
}
void setup_mqtt()
{
  // Loop until we're reconnected
  while (client.connected() == false)
  {
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    if (client.connect(mqtt_clientId, mqtt_user, mqtt_pass))
    {
      client.subscribe(mqtt_sub_topic);
    }
    else
    {
      delay(5000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length)
{
  if (strcmp((const char *)topic, (const char *)mqtt_sub_topic) != 0)
    return;
  char *input = (char *)payload;
  size_t inputLength = length;

  StaticJsonDocument<96> doc;

  DeserializationError error = deserializeJson(doc, input, inputLength);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }
  if (doc.containsKey("violed_sta"))
  {
    UART_TX[0] = VIOLED_STA_FUN;
    air.violed_sta = doc["violed_sta"];
    UART_TX[1] = air.violed_sta;
    UART_TX[2] = P_TAIL;
    txlen = 3;
  }
  if (doc.containsKey("humi_sta"))
  {
    UART_TX[0] = HUMI_STA_FUN;
    air.humi_sta = doc["humi_sta"];
    UART_TX[1] = air.humi_sta;
    UART_TX[2] = P_TAIL;
    txlen = 3;
  }
  if (doc.containsKey("fan_sta"))
  {
    UART_TX[0] = FAN_STA_FUN;
    air.fan_sta = doc["fan_sta"];
    UART_TX[1] = air.fan_sta;
    UART_TX[2] = P_TAIL;
    txlen = 3;
  }
  if (doc.containsKey("violed_offtime"))
  {
    UART_TX[0] = VIOLED_OFFTIME_FUN;
    air.violed_offtime = doc["violed_offtime"];
    UART_TX[1] = air.violed_offtime<<24;
    UART_TX[2] = air.violed_offtime<<16;
    UART_TX[3] = air.violed_offtime<<8;
    UART_TX[4] = air.violed_offtime<<0;
    UART_TX[5] = P_TAIL;
    txlen = 6;
  }
  if (doc.containsKey("mode"))
  {
    UART_TX[0] = MODE_STA_FUN;
    air.mode = doc["mode"];
    UART_TX[1] = air.mode;
    UART_TX[2] = P_TAIL;
    txlen = 3;
  }
  Serial.write(UART_TX, txlen);
}
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  setup_time();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  setup_mqtt();
}

void loop()
{
  if (!client.connected())
  {
    setup_mqtt();
  }
  while (Serial.available())
  {
    uint8_t data = Serial.read();
    if (data == P_TAIL)
    {
      air.all_updata++;
      switch (UART_RX[0])
      {
      case HUMI_STA_FUN:
        air.humi_sta = UART_RX[1];
        doc["humi_sta"] = air.humi_sta;
        break;
      case FAN_STA_FUN:
        air.fan_sta = UART_RX[1];
        doc["fan_sta"] = air.fan_sta;
        break;
      case VIOLED_STA_FUN:
        air.violed_sta = UART_RX[1];
        doc["violed_sta"] = air.violed_sta;
        break;
      case TEMP_DATA_FUN:
        air.temperature = (float)((uint16_t)UART_RX[1] << 8 | UART_RX[2]) / 100;
        doc["temperature"] = String(air.temperature, 1);
        break;
      case HUMI_DATA_FUN:
        air.humidity = (float)((uint16_t)UART_RX[1] << 8 | UART_RX[2]) / 100;
        doc["humidity"] = String(air.humidity, 1);
        break;
      case PM25_DATA_FUN:
        air.pm25 = (float)((uint16_t)UART_RX[1] << 8 | UART_RX[2]) / 100;
        doc["pm25"] = String(air.pm25, 2);
        break;
      case AIR_DATA_FUN:
        air.air_quality = (float)((uint16_t)UART_RX[1] << 8 | UART_RX[2]) / 100;
        doc["air_quality"] = String(air.air_quality, 2);
        break;
      case MODE_STA_FUN:
        air.mode = UART_RX[1];
        doc["mode"] = air.mode;
        break;
      case VIOLED_OFFTIME_FUN:
        air.violed_offtime = (uint32_t)(UART_RX[1]<<24)|(uint32_t)(UART_RX[2]<<16)|(uint32_t)(UART_RX[3]<<8)|(uint32_t)(UART_RX[4]<<0);
        doc["violed_offtime"] = air.violed_offtime;
        break;
      default:
        break;
      }
      rxlen = 0;
      if (air.all_updata == 9)
      {
        air.all_updata = 0;
        serializeJson(doc, mqtt_upload_json);
        client.publish(mqtt_pub_topic, mqtt_upload_json.c_str());
        doc.clear();
        mqtt_upload_json.clear();
      }
    }
    else
    {
      UART_RX[rxlen++] = data;
    }
  }
  if(millis() - last_rtc_update >=1000*10)
  {
    rtc_time_send();
    last_rtc_update=millis();
  }
  client.loop();
  timeClient.update();
}