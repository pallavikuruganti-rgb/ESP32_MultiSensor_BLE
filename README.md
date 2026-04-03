# Implementation of Intelligent adaptable Multi sensor system

📌 **Overview**
This project implements an intelligent multi-sensor monitoring system using the ESP32 microcontroller. It integrates temperature, vibration, and galvanic skin response (GSR) measurements to detect environmental changes and identify the dominant variation among the sensed parameters. The processed data is transmitted wirelessly via Bluetooth Low Energy (BLE) to a mobile device.

🎯 **Features**
* **Real-time monitoring** of multiple sensors
* **Dominant variation detection** (Temperature / Moisture / Vibration)
* **BLE-based wireless data transmission**
* **Interrupt-based vibration detection** for fast response
* **Compact data format** for efficient communication

🧰 **Components Used**
* **ESP32** Development Board
* **MLX90614** Infrared Temperature Sensor (I2C)
* **SW420** Vibration Sensor (Digital Output)
* **GSR Measurement Setup** (Voltage Divider with Electrodes)
* **100kΩ Resistor**
* Breadboard and Jumper Wires

🔌 **Circuit Connections**

![Circuit Connection Diagram](./circuit%20connection%20diagram.jpeg)

### MLX90614 (I2C Temperature Sensor)
* **VCC** → 3.3V
* **GND** → GND
* **SDA** → GPIO21
* **SCL** → GPIO22

### SW420 (Vibration Sensor)
* **VCC** → VIN (5V)
* **GND** → GND
* **DO** → GPIO27

### GSR Setup (Voltage Divider)
* **One side** → 3.3V
* **Other side** → GND through 100kΩ resistor
* **Junction (Output)** → GPIO34

⚙️ **Working Principle**
The ESP32 reads data from three sources:
1. **Temperature** using MLX90614 via I2C.
2. 2. **Moisture/GSR** using analog input (implemented as a voltage divider where skin resistance variation changes the output voltage).
3. **Vibration** using interrupt-based digital input.

The system calculates the variation in sensor readings over time and determines the dominant parameter based on predefined thresholds.

📡 **BLE Data Transmission**
The ESP32 acts as a BLE server and sends sensor data periodically using notifications.

**Data Format:**
`Temperature, Moisture, Vibration, Dominant`

**Example:**
`30.6,1927,1,V`

**Where:**
* **Temperature** → °C
* **Moisture** → Analog value
* **Vibration** → 0 (No) / 1 (Yes)
* **Dominant** → T (Temp), M (Moisture), V (Vibration)

📱 **Mobile Interface**
The BLE data can be received using apps like **nRF Connect**:
1. Scan for device: `ESP32_SENSOR`
2. Connect
3. Enable notifications
4. View real-time data

🚀 **Future Improvements**
* Mobile app for visualization and control.
* Local data storage (SQLite / Room DB).
* Cloud integration using REST API or Firebase.
* Dashboard for analytics and alerts.

📂 **Repository Structure**
* `/code` → ESP32 Arduino code
* `/docs` → Report and circuit diagram
* `README.md` → Project documentation

✅ **Status**
* ✔ Sensors integrated
* ✔ Dominant variation detection implemented
* ✔ BLE communication working
* 🔄 Cloud integration (in progress)

👤 **Author**
Kuraganti Naga Pallavi