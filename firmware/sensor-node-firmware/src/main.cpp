#include <Arduino.h>
#include <max6675.h>

#define GY_RX 17
#define GY_TX 18

#define POT 4

#define RED_LED 5
#define YELLOW_LED 6
#define GREEN_LED 7
#define BLUE_LED 8

#define MAX_CS 10
#define MAX_SCK 12
#define MAX_SO 13

#define CALIBRATION_BUTTON 15

MAX6675 thermocouple(MAX_SCK, MAX_CS, MAX_SO);

unsigned long lastTempRead = 0;
const unsigned long tempReadInterval = 500;

float pitch = 0, roll = 0, yaw = 0;

struct SensorData {
  float temperature;
  float vibration;
  float threshold;
  bool alarm;
};

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);

  pinMode(CALIBRATION_BUTTON, INPUT_PULLUP);
  

  Serial2.begin(115200, SERIAL_8N1, GY_TX, GY_RX);

  digitalWrite(BLUE_LED, HIGH);
  delay(300);
  digitalWrite(BLUE_LED, LOW);

  delay(300);
  digitalWrite(RED_LED, HIGH);
  delay(300);
  digitalWrite(RED_LED, LOW);

  digitalWrite(YELLOW_LED, HIGH);
  delay(300);
  digitalWrite(YELLOW_LED, LOW);

  digitalWrite(GREEN_LED, HIGH);
  delay(300);
  digitalWrite(GREEN_LED, LOW);

}

void loop() {
  if (millis() - lastTempRead >= tempReadInterval) {
    lastTempRead = millis();

    float temp = thermocouple.readCelsius();

    digitalWrite(BLUE_LED, LOW);
    digitalWrite(RED_LED, LOW);
    digitalWrite(YELLOW_LED, LOW);
    digitalWrite(GREEN_LED, LOW);

    if (temp <= 15) {
      digitalWrite(BLUE_LED, HIGH);
      } else if (temp > 15 && temp <= 45) {
      digitalWrite(GREEN_LED, HIGH);
      } else if (temp > 45 && temp <= 60) {
      digitalWrite(YELLOW_LED, HIGH);
      } else if (temp > 60) {
      digitalWrite(RED_LED, HIGH);
      }
  }

  int raw = analogRead(POT);
  int threshold = map(raw, 0, 4095, 0, 100);

  if (Serial2.available() >= 8) {
  if (Serial2.read() == 0xAA) {
    byte datos[7];
    Serial2.readBytes(datos, 7);
    if (datos[6] == 0x55) {
      pitch = (int16_t)(datos[0] << 8 | datos[1]) / 100.0;
      roll  = (int16_t)(datos[2] << 8 | datos[3]) / 100.0;
      yaw   = (int16_t)(datos[4] << 8 | datos[5]) / 100.0;
    }
  }
  }
}
