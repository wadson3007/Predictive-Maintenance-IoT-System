#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp_now.h>

#define BUZZER 4
#define WIFI_LED 5
#define ALARM_LED 6
#define MQTT_LED 7
#define OLED_SDA 8
#define OLED_SCL 9
#define ACK_BUTTON 15

struct SensorData {
  float temperature;
  float vibration;
  float threshold;
  bool alarm;
};

Adafruit_SSD1306 display(128, 64, &Wire, -1);

SensorData received;

void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  memcpy(&received, incomingData, sizeof(received));
}

void setup() {
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
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}
