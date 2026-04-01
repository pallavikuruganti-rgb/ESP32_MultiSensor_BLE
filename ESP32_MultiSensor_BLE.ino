#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define VIB_PIN 27
#define MOISTURE_PIN 34

// BLE
BLECharacteristic *pCharacteristic;

// Vibration interrupt
volatile bool vibrationDetected = false;
volatile unsigned long lastInterruptTime = 0;

// Previous values
float prevTemp = 0;
int prevMoisture = 0;

void IRAM_ATTR detectVibration() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastInterruptTime > 100) {
    vibrationDetected = true;
    lastInterruptTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(VIB_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(VIB_PIN), detectVibration, CHANGE);

  mlx.begin();

  // ===== BLE SETUP =====
  BLEDevice::init("ESP32_SENSOR");

  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService("1234");

  pCharacteristic = pService->createCharacteristic(
                      "5678",
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->addDescriptor(new BLE2902());

  pService->start();
  BLEDevice::getAdvertising()->start();

  Serial.println("Multi-Sensor Monitoring Started");
  Serial.println("BLE Started");

  // Initial readings
  prevTemp = mlx.readObjectTempC();
  prevMoisture = analogRead(MOISTURE_PIN);
}

void loop() {

  // 🔹 Read sensors
  float currentTemp = mlx.readObjectTempC();
  int currentMoisture = analogRead(MOISTURE_PIN);

  // 🔹 Calculate changes
  float tempChange = abs(currentTemp - prevTemp);
  int moistureChange = abs(currentMoisture - prevMoisture);

  String dominant = "N";

  // 🔹 Print values
  Serial.print("Temp: ");
  Serial.print(currentTemp);
  Serial.print(" | Moisture: ");
  Serial.print(currentMoisture);
  Serial.print(" | Vibration: ");
  Serial.println(vibrationDetected ? "YES" : "NO");

  // 🔹 Dominant detection (UNCHANGED)
  if (vibrationDetected) {
    Serial.println(">>> Dominant: VIBRATION <<<");
    dominant = "V";
  }
  else if (tempChange > 0.5 && tempChange > moistureChange) {
    Serial.println(">>> Dominant: TEMPERATURE <<<");
    dominant = "T";
  }
  else if (moistureChange > 50 && moistureChange > tempChange) {
    Serial.println(">>> Dominant: MOISTURE <<<");
    dominant = "M";
  }
  else {
    Serial.println("No significant change");
  }

  Serial.println("-----------------------------");

  // 🔹 COMPACT DATA FORMAT (FIXED)
  String data = "";
  data += String(currentTemp,1);
  data += ",";
  data += String(currentMoisture);
  data += ",";
  data += String(vibrationDetected ? 1 : 0);
  data += ",";
  data += dominant;

  // 🔹 SEND VIA BLE
  pCharacteristic->setValue(data.c_str());
  pCharacteristic->notify();

  // Debug
  Serial.println("BLE Sent: " + data);

  // 🔹 RESET vibration AFTER sending
  vibrationDetected = false;

  // 🔹 Update previous values
  prevTemp = currentTemp;
  prevMoisture = currentMoisture;

  delay(1000);
}