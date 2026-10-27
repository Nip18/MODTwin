MODTwin: IoT-Based Machine Health Monitoring

MODTwin is an ESP32-based prototype for monitoring machine operating conditions using sensor acquisition, rule-based health assessment, alerts, and cloud-based visualisation.

The project explores how low-cost embedded hardware and IoT connectivity can be used to build a foundation for condition monitoring and digital-twin systems.

Project Status

Current stage: Working prototype with simulated RPM and vibration inputs.

The current implementation reads temperature from a DHT11, generates RPM and vibration inputs for testing, calculates a basic health score, activates LED and buzzer alerts, and sends data to ThingSpeak.

Real shaft-speed measurement and reliable accelerometer-based vibration acquisition remain future improvements.

System Architecture

Sensors and test inputs → ESP32 → Health assessment → Status classification → LED/buzzer alerts → ThingSpeak dashboard

Hardware

ESP32 development board

DHT11 temperature and humidity sensor

Potentiometer for simulated RPM input

RGB LED

Buzzer

MPU6050 accelerometer/gyroscope module (integration incomplete)

Breadboard, jumper wires, and USB power/programming connection

Software and Libraries

Arduino C/C++

ESP32 Arduino framework

DHT sensor library

Adafruit MPU6050 and Adafruit Unified Sensor libraries

ThingSpeak Arduino library

Wi-Fi connectivity

Current Features

Temperature acquisition using the DHT11

Simulated RPM input using a potentiometer

Simulated vibration values for testing

Rule-based health score calculation

Four operating states: HEALTHY, WARNING, CRITICAL, and SHUTDOWN

RGB LED and buzzer status indication

Periodic data transmission to ThingSpeak

Serial Monitor output for debugging

Health Assessment

The current health score begins at 100 and applies penalties based on temperature and RPM thresholds.

The vibration penalty is currently disabled because the accelerometer integration has not been validated. The health score is a prototype rule-based indicator, not a validated industrial health or failure prediction model.

Repository Structure

main.ino — ESP32 firmware

secrets.example.h — Template for local Wi-Fi and API credentials

docs/ — Wiring diagrams, project photographs, and dashboard screenshots

The private secrets.h file is intentionally excluded from version control.

Setup

Install the ESP32 board package in Arduino IDE.

Install the required libraries listed above.

Copy secrets.example.h to secrets.h.

Enter your local Wi-Fi credentials and ThingSpeak write API key in secrets.h.

Open main.ino in Arduino IDE and select the appropriate ESP32 board and port.

Compile and upload the firmware.

Open Serial Monitor at 115200 baud.

Configure the ThingSpeak channel with numeric fields for temperature, RPM, health score, status code, and vibration.

Keep secrets.h private. Never publish Wi-Fi passwords or write API keys.

Current Limitations

RPM is simulated rather than measured from a rotating shaft.

Vibration is generated in software rather than measured from the MPU6050.

The MPU6050 integration is incomplete and has not been validated.

The health score uses basic thresholds and has not been validated against real machine faults.

The temperature fallback and sensor-error handling require improvement.

Dashboard status codes require a documented numeric mapping.

Planned Improvements

Integrate reliable accelerometer-based vibration measurement.

Measure actual motor RPM using a suitable sensor.

Improve sensor fault detection and health-score logic.

Add MQTT-based telemetry and a clearer dashboard.

Connect monitoring to a physical motor test platform.

Validate the system using repeatable experiments and recorded results.

Safety and Scope

MODTwin is an educational prototype. It is not certified for industrial safety, predictive maintenance decisions, or autonomous protection of production machinery.

## Project Evidence

### Physical Setup
![Hardware setup](docs/hardware-setup.jpg)

### Serial Monitor
![Serial output](docs/serial-monitor.png)

### ThingSpeak Dashboard
![ThingSpeak dashboard](docs/thingspeak-dashboard.png)

### Wiring Diagram
![Wiring diagram](docs/wiring-diagram.png)