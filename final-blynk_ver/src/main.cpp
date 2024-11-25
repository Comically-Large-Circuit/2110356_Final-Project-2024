/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6DRQnlCVV"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "N2RLZVdZPFcRUmLivGbA-RGi4aY1ssoc"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

/* PIN
Light Pin & Air Pin: SDA-21 SCL-22
Moist Meter: 32
Ultrasonic Pin: trig-5 echo-18
*/

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "thamva";
char pass[] = "123456789";

// Light Meter
BH1750 lightMeter;

// Air Measure
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C

// Moist Meter
int _moisture, sensor_analog;
int sensor_pin = 32;

// Water pump button
const int button_pump = 34;
const int Pump_Output = 33;

unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 100;  // the debounce time; increase if the output flickers

int buttonState; // the current reading from the input pin
int lastButtonState = LOW;
int reading;
// Add a global variable to track pump status

void pourWater()
{
  unsigned long startMillis = millis();
  Serial.println("Pumping water ...");
  digitalWrite(Pump_Output, HIGH);
  while (millis() - startMillis < 5000)
    ;
  digitalWrite(Pump_Output, LOW);
  Serial.println("Water pumped");
}

void pumpControl()
{
  reading = digitalRead(button_pump);
  if (reading != lastButtonState)
  {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay)
  {
    if (reading != buttonState)
    {
      buttonState = reading;
      if (buttonState == HIGH)
      {
        Serial.println("Button pressed");
        pourWater();
      }
    }
  }
  lastButtonState = reading;
}

// Ultrasonic
const int trigPin = 5;
const int echoPin = 18;
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;

// Local IP Camera
String my_Local_IP;

BlynkTimer timer;

float printLightValues()
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

void printMoistMeter()
{
  sensor_analog = analogRead(sensor_pin);
  _moisture = (100 - ((sensor_analog / 4095.00) * 100));
  Serial.print("Moisture = ");
  Serial.print(_moisture); /* Print Temperature on the serial window */
  Serial.println("%");
}

// This function is called every time the Virtual Pin 0 state changes
static int timerID = -1;
BLYNK_WRITE(V9)
{
  int value = param.asInt();
  if (value == 1)
  {
    pourWater();               // Call the function to perform the action
    Blynk.virtualWrite(V9, 0); // Reset the virtual pin state to 0
  }
}

BLYNK_WRITE(V10)
{
  // Camera URL
  Blynk.virtualWrite(V10, "http://" + my_Local_IP + "/capture");
}

BLYNK_WRITE(V0)
{
  // Set incoming value from pin V0 to a variable
  int value = param.asInt();

  // Update state
  Blynk.virtualWrite(V1, value);
  if (value == 1)
  {
    // Start a timer to read sensor values periodically
    if (timerID != -1)
    {
      timer.deleteTimer(timerID);
    }
    timerID = timer.setInterval(1000L, []()
                                {
      printAirValues();
      printUltraSonicValues();
      printMoistMeter();
      Blynk.virtualWrite(V4, 30 - distanceCm);
      Blynk.virtualWrite(V5, printLightValues());
      Blynk.virtualWrite(V6, _moisture);
      Blynk.virtualWrite(V7, bme.readHumidity());
      Blynk.virtualWrite(V8, bme.readTemperature());
      Serial.println(""); });
  }
  else if (timerID != -1)
  {
    // Stop the timer if value is not 1
    timer.deleteTimer(timerID);
    timerID = -1;
  }
}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
  Blynk.setProperty(V10, "url", "http://" + my_Local_IP + "/stream");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(115200);
  // Initialize the I2C bus for Light measuring
  Wire.begin(21, 22); // SDA21, SCL=22
  // Pump
  pinMode(button_pump, INPUT);
  pinMode(Pump_Output, OUTPUT);

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

  // Moisture sensor setup
  pinMode(sensor_pin, INPUT);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer = BlynkTimer();
  // Camera Local IP
  my_Local_IP = WiFi.localIP().toString();
  Serial.println(my_Local_IP);
}

void loop()
{
  // printMoistMeter();
  Blynk.run();
  timer.run();
  pumpControl();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}