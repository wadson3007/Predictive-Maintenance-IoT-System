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
  
}

void loop() {
  
}
