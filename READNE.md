# Predictive Maintenance IoT System for Industrial Motors

A two-node IoT system that monitors industrial motors in real time to predict failures before they happen. A **sensor node** measures temperature and vibration directly on the motor, and sends the data wirelessly to a **gateway** that displays status, triggers alarms, and pushes the readings to the cloud

Built with custom PCBs (KiCad), 3D-printed enclosures (Onshape), and ESP32-S3 microcontrollers

## Why I made it

This project started when I was asked to do an IoT project based on some industrial project, so I developed a basic version of this complete project, I made it on a weekend using only one breadboard and one ESP32 DevKit module

The reason behind the two versions of this project are that on big industrial companies they spend a lot of money on keeping workers near the equipment to be sure that all is working well and my first idea was what would happen if we could cut those additional payments, so I came up with the idea of a project that could monitor the changes on the motors and make an alarm when things are getting bad

Im not saying that it could replace all the workers that are there, because the motors still need someone to operate them but I think this project could cut a little bit of the physical and mental strain on staff

## How it works

The system has two nodes that communicate wirelessly:

### Sensor Node (On the motor)

Mounted directly on the motor, its constantly cheking the two signals that reveal a failure

- **Temperature** a MAX6675 module with a K-type thermocouple reads the motors temperature
- **Vibration** a GY-25 module measures the motors vibration/tilt
- **Threshold** a trimmer potentiometer lets you set the vibration alarm threshold by hand
- **Status LEDs** 4 LEDs show the temperature state at a glance (blue = cold, green = normal, yellow = warning, red = critical)
- **Calibration button** sets the reference "normal" state for the motor being monitored

The sensor node sends its readings to the gateway using **ESP-NOW**, a fast, low-power wireless protocol that doesnt need a WiFi router

### Gateway (Control point)

Placed where an operator can see it, the gateway receives the data and acts on it:

- **OLED display** shows live temperature, vibration, and system status
- **Buzzer** sounds an audible alarm when a reading crosses a dangerous threshold
- **Status LEDs** 3 LEDs indicate WiFi connection, active alarm, and MQTT/cloud status
- **ACK button** lets the operator acknowledge and silence an alarm

The gateway forwards every reading to the cloud over **MQTT**, so the motor data can be logged and monitored remotely

## Pinout

### Sensor Node (ESP32-S3-WROOM-1)

**MAX6675** (temperature):

- SCK: GPIO12
- SO: GPIO13
- CS: GPIO10

**GY-25** (vibration):

- TX: GPIO18
- RX: GPIO17

**Potentiometer** (threshold): 

- GPIO4

**Calibration button**:

- GPIO15

**LEDs**:

- Blue (cold): GPIO8
- Green (normal): GPIO7
- Yellow (warning): GPIO6
- Red (critical): GPIO5

**USB-C**:

- D-: GPIO19
- D+: GPIO20

### Gateway (ESP32-S3-WROOM-1)

**OLED display** (I2C):

- SDA: GPIO8
- SCL: GPIO9

**Buzzer**:

- GPIO4

**ACK button**:

- GPIO15

**LEDs**:

- WiFi (green): GPIO5
- Alarm (red): GPIO6
- MQTT (blue): GPIO7

**USB-C**:

- D-: GPIO19
- D+: GPIO20
- 
