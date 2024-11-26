#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Arduino.h>

#define SEALEVELPRESSURE_HPA (1013.25)
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

void initLightSensor(BH1750 &lightMeter)
{
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
  {
    // Serial.println(F("BH1750 initialized"));
  }
  else
  {
    Serial.println(F("Error initializing BH1750"));
    while (1);
  }
}

float readLightSensor(BH1750 &lightMeter)
{
  float lux = lightMeter.readLightLevel();
  if (lux >= 0)
  {
    Serial.print("Light: ");
    Serial.print(lux);
    Serial.println(" lx");
  }
  else
  {
    Serial.println("Error reading light level");
  }
  return lux;
}

void initAirSensor(Adafruit_BME280 &bme)
{
  bool status;
  status = bme.begin(0x76);
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
}

float readAirSensor(Adafruit_BME280 &bme)
{
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");

  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
}

void initUltrasonic(int trigPin, int echoPin)
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input
}

float readUltrasonic(int trigPin, int echoPin)
{
  long duration;
  float distanceCm;
  float distanceInch;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED / 2;
  distanceInch = distanceCm * CM_TO_INCH;

  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm ");
  Serial.print(distanceInch);
  Serial.println(" inch");
  return distanceCm;
}

void initMoistureSensor(int sensor_pin)
{
  pinMode(sensor_pin, INPUT);
}

int readMoistureSensor(int sensor_pin)
{
  int sensor_analog = analogRead(sensor_pin);
  Serial.print("Moisture: ");
  Serial.println(sensor_analog);
  return sensor_analog;
}

float printUltraSonicValues(int trigPin, int echoPin)
{
  return readUltrasonic(trigPin, echoPin);
}

