# Plantae - Smart IoT Plant Monitoring System 🌱

This repository contains the source code and resources for **Plantae**, a smart plant monitoring system that leverages IoT to automate plant care. The project uses sensors, actuators, and a Blynk-based dashboard to monitor and manage environmental conditions for plants.

---

## Features

- 🌞 **Light Monitoring**: Tracks light intensity using a BH1750 sensor.
- 💧 **Soil Moisture Detection**: Monitors soil humidity with an FC-28 sensor.
- 🌡️ **Air Monitoring**: Measures temperature and humidity using a BME280 sensor.
- 🚰 **Water Level Management**: Monitors water levels with an ultrasonic sensor.
- 💦 **Automated Water Pump**: Controls a water pump for irrigation based on moisture levels.
- 📡 **Blynk Integration**: Remote monitoring and control using the Blynk app.
- 🎥 **Camera Integration**: Triggers a camera for plant image capture and monitoring.

---

## Project Structure

```plaintext
2110356_Final-Project-2024/
├── src/                 # Source code and libraries
├── data/                # Sensor data and logs
├── tests/               # Unit tests for system components
├── README.md            # Project documentation
```

---

## Hardware Setup

### Pin Configuration for ESP32

| GPIO Pin | Sensor                   | Type                |
| :------- | :----------------------- | :------------------ |
| GPIO 21  | BH1750                  | Light Sensor        |
| GPIO 22  | BH1750                  | Light Sensor        |
| GPIO 21  | BME280                  | Humidity Sensor     |
| GPIO 22  | BME280                  | Humidity Sensor     |
| GPIO 32  | FC-28                   | Soil Humidity Sensor |
| GPIO 5   | Ultrasonic (Trigger)    | Water Level Sensor  |
| GPIO 18  | Ultrasonic (Echo)       | Water Level Sensor  |
| GPIO 33  | Pump Control            | Actuator            |
| GPIO 34  | Pump Trigger Button     | Button              |

---

### Virtual Pins for Blynk

| Virtual Pin | Sensor        | Value              |
| :---------- | :------------ | :----------------- |
| V4          | Ultrasonic    | Water Level        |
| V5          | BH1750        | Light Level        |
| V6          | FC-28         | Soil Moisture      |
| V7          | BME280        | Humidity Level     |
| V8          | BME280        | Temperature        |
| V9          | Pump          | Pump State         |

---

## Software Setup

1. **Clone the Repository**:
   ```bash
   git clone https://github.com/username/2110356_Final-Project-2024.git
   cd 2110356_Final-Project-2024
   ```

2. **Install Dependencies**:
   - Install the required Arduino libraries:
     - Blynk
     - BH1750
     - Adafruit BME280
     - ArduinoJson
   - Use the Arduino IDE or PlatformIO to manage dependencies.

3. **Configure Blynk**:
   Update the following lines in your `src` files with your Blynk credentials:
   ```cpp
   #define BLYNK_TEMPLATE_ID "Your_Template_ID"
   #define BLYNK_TEMPLATE_NAME "Your_Project_Name"
   #define BLYNK_AUTH_TOKEN "Your_Auth_Token"
   ```

4. **Upload Code**:
   - Connect your ESP32 to your computer.
   - Compile and upload the code via the Arduino IDE or PlatformIO.

5. **Run the System**:
   - Ensure all hardware is correctly connected.
   - Power on the ESP32 and monitor the serial output for status updates.

---

## How It Works

- The ESP32 reads sensor data and sends it to the Blynk app in real time.
- An ultrasonic sensor monitors water levels and triggers alerts if levels are low.
- The water pump is automatically activated when soil moisture drops below a threshold.
- A button or the Blynk dashboard can be used to manually activate the pump.
- Sensor data is periodically sent to a Google Sheets script for logging and visualization.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

Feel free to copy and paste this into your repository! Let me know if you need further adjustments or explanations.
