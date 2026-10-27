#include <Wire.h>
#include <DHT.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#include "secrets.h"

/********* WiFi *********/
unsigned long channelID = 3443183;

WiFiClient client;

// ---------------- PINS ----------------

#define DHTPIN 27
#define DHTTYPE DHT11

#define POT_PIN 34

#define RED_PIN 26
#define GREEN_PIN 32
#define BLUE_PIN 33

#define BUZZER_PIN 25

// ---------------- OBJECTS ----------------

DHT dht(DHTPIN, DHTTYPE);
Adafruit_MPU6050 mpu;

// ---------------- VARIABLES ----------------

float temperature = 0;

float vibration = 0;

int rpm = 0;

int health = 100;

String status = "HEALTHY";

unsigned long previousMillis = 0;

// ---------------- SETUP ----------------

void setup()
{
    Serial.begin(115200);

    
    WiFi.begin(ssid, password);

    Serial.print("Connecting to WiFi");

    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 20)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("WiFi Connected!");
        Serial.print("IP Address: ");
        Serial.println(WiFi.localIP());

        ThingSpeak.begin(client);
    }
    else
    {
        Serial.println("WiFi connection FAILED!");
        Serial.print("WiFi status code: ");
        Serial.println(WiFi.status());
    }
    ThingSpeak.begin(client);

    Wire.begin(21,22);

    dht.begin();

    pinMode(RED_PIN,OUTPUT);
    pinMode(GREEN_PIN,OUTPUT);
    pinMode(BLUE_PIN,OUTPUT);

    pinMode(BUZZER_PIN,OUTPUT);

    digitalWrite(BUZZER_PIN,LOW);

    /*if(!mpu.begin())
    {
        Serial.println("MPU6050 NOT FOUND");

        while(true);
    }*/

    Serial.println("================================");
    Serial.println("MODTwin Started");
    Serial.println("================================");
}

// ---------------- SENSOR FUNCTIONS ----------------

void readTemperature()
{
    temperature = dht.readTemperature();

    if(isnan(temperature))
    {
        temperature = 25;
    }
}

void readRPM()
{
    int value = analogRead(POT_PIN);

    rpm = map(value,0,4095,1000,5000);
}

void readVibration()
{
    /*sensors_event_t accel,gyro,temp;

    mpu.getEvent(&accel,&gyro,&temp);

    vibration = sqrt(
        accel.acceleration.x*accel.acceleration.x +
        accel.acceleration.y*accel.acceleration.y +
        accel.acceleration.z*accel.acceleration.z
    );*/
    vibration = random(95, 131) / 10.0;

}

// ---------------- LED FUNCTION ----------------

void setColor(bool r,bool g,bool b)
{
    digitalWrite(RED_PIN,r);
    digitalWrite(GREEN_PIN,g);
    digitalWrite(BLUE_PIN,b);
}
// ---------------- HEALTH CALCULATION ----------------

void calculateHealth()
{
    health = 100;

    // Temperature Penalty
    if (temperature > 30)
        health -= (temperature - 30) * 2;

    // RPM Penalty
    if (rpm > 3500)
        health -= (rpm - 3500) / 100;

    // Vibration Penalty
    // Around 9.8 m/s² is normal because of gravity.
    /*if (vibration > 10.5)
        health -= (vibration - 10.5) * 15;*/

    if (health > 100)
        health = 100;

    if (health < 0)
        health = 0;
}

// ---------------- DECISION ENGINE ----------------

void updateMachine()
{
    if (health >= 90)
    {
        status = "HEALTHY";

        setColor(LOW, HIGH, LOW);   // Green

        digitalWrite(BUZZER_PIN, LOW);
    }

    else if (health >= 70)
    {
        status = "WARNING";

        setColor(HIGH, HIGH, LOW);  // Yellow

        digitalWrite(BUZZER_PIN, LOW);
    }

    else if (health >= 40)
    {
        status = "CRITICAL";

        setColor(HIGH, LOW, LOW);   // Red

        digitalWrite(BUZZER_PIN, HIGH);
    }

    else
    {
        status = "SHUTDOWN";

        setColor(HIGH, LOW, HIGH);  // Purple

        digitalWrite(BUZZER_PIN, HIGH);
    }
}

// ---------------- SERIAL OUTPUT ----------------

void printData()
{
    Serial.println("------------------------------------");

    Serial.print("Temperature : ");
    Serial.print(temperature);
    Serial.println(" C");

    Serial.print("RPM : ");
    Serial.println(rpm);

    Serial.print("Vibration : ");
    Serial.println(vibration);

    Serial.print("Health : ");
    Serial.print(health);
    Serial.println("%");

    Serial.print("Status : ");
    Serial.println(status);

    Serial.println("------------------------------------");
}

// ---------------- LOOP ----------------

void loop()
{         
    readTemperature();

    readRPM();

    readVibration();

    calculateHealth();

    updateMachine();

    if (millis() - previousMillis > 15000)
    {
        previousMillis = millis();

        printData();
        ThingSpeak.setField(1, temperature);
        ThingSpeak.setField(2, rpm);
        ThingSpeak.setField(3, health);
        int statusCode = 0;
        if (status == "HEALTHY") statusCode = 1;
        else if (status == "WARNING") statusCode = 2;
        else if (status == "CRITICAL") statusCode = 3;
        else if (status == "SHUTDOWN") statusCode = 4;
        ThingSpeak.setField(4, statusCode);
        ThingSpeak.setField(5, vibration);

        int x = ThingSpeak.writeFields(channelID, writeAPIKey);

        Serial.print("ThingSpeak Response: ");
        Serial.println(x);
    }
}