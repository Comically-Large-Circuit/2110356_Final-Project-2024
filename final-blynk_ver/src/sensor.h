#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// Constants for sensors
#define SEALEVELPRESSURE_HPA (1013.25)
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

// Function declarations
void initLightSensor(BH1750 &lightMeter);
float readLightSensor(BH1750 &lightMeter);
void initAirSensor(Adafruit_BME280 &bme);
void readAirSensor(Adafruit_BME280 &bme);
void initMoistureSensor(int sensorPin);
int readMoistureSensor(int sensorPin);
void initUltrasonicSensor(int trigPin, int echoPin);
float readUltrasonicSensor(int trigPin, int echoPin);

#endif
