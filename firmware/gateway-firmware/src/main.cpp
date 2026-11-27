#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>
#include <PubSubClient.h>
#include "secrets.h"

#define BUZZER 4
#define WIFI_LED 5
#define ALARM_LED 6
#define MQTT_LED 7
#define OLED_SDA 8
#define OLED_SCL 9
#define ACK_BUTTON 15
#define MQTT_SERVER "io.adafruit.com"
#define MQTT_PORT 1883

struct SensorData
{
  float temperature;
  float vibration;
  float threshold;
  bool alarm;
};

Adafruit_SSD1306 display(128, 64, &Wire, -1);

SensorData received;


WiFiClient espClient;
PubSubClient mqtt(espClient);

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len)
{
  memcpy(&received, incomingData, sizeof(received));
}

void connectMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Connecting to MQTT...");
    if (mqtt.connect("gateway-esp32", AIO_USERNAME, AIO_KEY))
    {
      Serial.println("connected");
      digitalWrite(MQTT_LED, HIGH);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.println(mqtt.state());
      delay(2000);
    }
  }
}

unsigned long lastBeep = 0;
const unsigned long beepInterval = 500;
bool beepOn = false;

bool acknowledged = false;

unsigned long lastDisplay = 0;
const unsigned long displayInterval = 500;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 10000;

void setup()
{
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(ALARM_LED, OUTPUT);
  pinMode(MQTT_LED, OUTPUT);
  pinMode(ACK_BUTTON, INPUT_PULLUP);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Predictive Maintenance");
  display.display();

  WiFi.mode(WIFI_STA);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  digitalWrite(WIFI_LED, HIGH);
  Serial.println("WiFi connected");

  mqtt.setServer(MQTT_SERVER, MQTT_PORT);

  connectMQTT();

  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  if (!mqtt.connected()) {
  connectMQTT();
}
mqtt.loop();

  if (received.alarm)
  {

    if (digitalRead(ACK_BUTTON) == LOW)
    {
      acknowledged = true;
    }

    if (!acknowledged)
    {
      if (millis() - lastBeep >= beepInterval)
      {
        lastBeep = millis();
        beepOn = !beepOn;
        if (beepOn)
        {
          tone(BUZZER, 2000);
          digitalWrite(ALARM_LED, HIGH);
        }
        else
        {
          noTone(BUZZER);
          digitalWrite(ALARM_LED, LOW);
        }
      }
    }
    else
    {
      noTone(BUZZER);
      digitalWrite(ALARM_LED, HIGH);
    }
  }
  else
  {
    digitalWrite(ALARM_LED, LOW);
    noTone(BUZZER);
    acknowledged = false;
  }

  if (millis() - lastDisplay >= displayInterval)
  {
    lastDisplay = millis();

    display.clearDisplay();
    display.setCursor(0, 0);

    display.println("Predictive Maintenance");

    display.print("Temp: ");
    display.print(received.temperature);
    display.println(" C");

    display.print("Vibration: ");
    display.println(received.vibration);

    if (received.alarm)
    {
      display.println("ALARM!");
    }
    else
    {
      display.println("OK");
    }
    display.display();
  }

  if (millis() - lastPublish >= publishInterval) {
    lastPublish = millis();

  char tempStr[10];
  snprintf(tempStr, sizeof(tempStr), "%.2f", received.temperature);
  mqtt.publish(AIO_USERNAME "/feeds/temperature", tempStr);

  char vibStr[10];
  snprintf(vibStr, sizeof(vibStr), "%.2f", received.vibration);
  mqtt.publish(AIO_USERNAME "/feeds/vibration", vibStr);


  char alarmStr[4];
  snprintf(alarmStr, sizeof(alarmStr), "%d", received.alarm);
  mqtt.publish(AIO_USERNAME "/feeds/alarm", alarmStr);
  }
}
