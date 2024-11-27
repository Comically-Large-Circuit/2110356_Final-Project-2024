/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "TMPL6SVHsqtvX"
#define BLYNK_TEMPLATE_NAME "Plantae"
#define BLYNK_AUTH_TOKEN "TKyUxAz6PQaSKeUSOeuB6yXqr9wpm3MK"
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <webserver.h>

#include <webserverhandler.h>
#include <sensor.h>
#include <pump.h>

/* PIN
Light Pin & Air Pin: SDA-21 SCL-22
Moist Meter: 32
Ultrasonic Pin: trig-5 echo-18
*/

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "thamva";
char pass[] = "123456789";

// Water pump button
const int button_pump = 34;
const int Pump_Output = 33;
unsigned long pump_duration = 5000;


// Local IP Camera
String my_Local_IP;

BlynkTimer timer;

// Light Meter
BH1750 lightMeter;
// Air Measure
Adafruit_BME280 bme; // I2C
// Moist Meter
// int _moisture;
int sensor_pin = 32;
// Ultrasonic
const int trigPin = 5;
const int echoPin = 18;


// This function is called every time the Virtual Pin 0 state changes
static int timerID = -1;
BLYNK_WRITE(V9)
{
  int value = param.asInt();
  if (value == 1)
  {
    startPump();              // Call the function to perform the action
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
      readAirSensor(bme);
      // Serial.println(30-readUltrasonicSensor(trigPin, echoPin));
      // Serial.println(readMoistureSensor(sensor_pin));
      Blynk.virtualWrite(V4, readUltrasonicSensor(trigPin, echoPin));
      Blynk.virtualWrite(V5, readLightSensor(lightMeter));
      Blynk.virtualWrite(V6, readMoistureSensor(sensor_pin));
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
  my_Local_IP = WiFi.localIP().toString();
  Serial.println("BLYNK_CONNECTED called");
  Serial.print("Setting V10 URL: ");
  Serial.println("http://" + my_Local_IP + "/capture");
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
  Blynk.setProperty(V10, "url", "http://" + my_Local_IP + "/capture");
}

// // This function sends Arduino's uptime every second to Virtual Pin 2.
// void myTimerEvent()
// {
//   // You can send any value at any time.
//   // Please don't send more that 10 values per second.
//   Blynk.virtualWrite(V2, millis() / 1000);
// }




void setup()
{
  // Debug console
  Serial.begin(115200);
  // sensor init
  Wire.begin(21, 22); // SDA21, SCL=22
  initLightSensor(lightMeter);
  initAirSensor(bme);
  initMoistureSensor(sensor_pin);
  initUltrasonicSensor(trigPin, echoPin);


  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  // You can also specify server:
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer = BlynkTimer();
  // Camera Local IP
  my_Local_IP = WiFi.localIP().toString();
  Serial.println(my_Local_IP);
  
  setupWebServer();
  initPump(button_pump, Pump_Output);
}

void loop()
{
  // printMoistMeter();
  Blynk.run();
  timer.run();
  handleWebServer();
  pumpControl(button_pump, Pump_Output, pump_duration);

 
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}