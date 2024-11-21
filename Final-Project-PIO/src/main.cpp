#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* PIN
Light Pin & Air Pin: SDA-21 SCL-22
Moist Meter: 15
Ultrasonic Pin: trig-5 echo-18
*/

// Light Meter
BH1750 lightMeter;

// Air Measure
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
unsigned long delayTime;

// Moist Meter
int _moisture, sensor_analog;
int sensor_pin = 15;

// Ultrasonic
const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;

void printLightValues()
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
}

void printAirValues()
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
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
}

void printMoistMeter()
{
  sensor_analog = analogRead(sensor_pin);
  _moisture = (100 - ((sensor_analog / 4095.00) * 100));
  Serial.print("Moisture = ");
  Serial.print(_moisture); /* Print Temperature on the serial window */
  Serial.println("%");
}

void printUltraSonicValues()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
  distanceCm = duration * SOUND_SPEED / 2;
  // Convert to inches
  distanceInch = distanceCm * CM_TO_INCH;
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  Serial.print("Distance (inch): ");
  Serial.println(distanceInch);
}

void checkPump()
{
  sensor_analog = analogRead(sensor_pin);
  _moisture = (100 - ((sensor_analog / 4095.00) * 100));
  if (_moisture > 25)
  {
    digitalWrite(25, HIGH);
  }
  else
  {
    digitalWrite(25, LOW);
  }
}

void setup()
{
  Serial.begin(9600);

  // Initialize the I2C bus for Light measuring
  Wire.begin(21, 22); // SDA21, SCL=22

  // setup for air measuring
  bool status;
  status = bme.begin(0x76);

  // Light measurre init Log
  if (lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
  {
    // Serial.println(F("BH1750 initialized"));
  }
  else
  {
    Serial.println(F("Error initializing BH1750"));
    while (1)
      ;
  }

  // Air measure error Log
  if (!status)
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  // Ultrasonic Setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  pinMode(25, OUTPUT);
}

void loop()
{

  // Print Light Value
  printLightValues();

  // Print Air Value
  printAirValues();

  // Moist Meter
  printMoistMeter();

  // Ultrasonic
  printUltraSonicValues();

  checkPump();

  Serial.println();

  // digitalWrite(25,HIGH);
  delay(2000);
}
