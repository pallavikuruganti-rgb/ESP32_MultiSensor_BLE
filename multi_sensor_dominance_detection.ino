#include <Wire.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#define VIB_PIN 27
#define MOISTURE_PIN 34

// Vibration interrupt
volatile bool vibrationDetected = false;
volatile unsigned long lastInterruptTime = 0;

// Previous values
float prevTemp = 0;
int prevMoisture = 0;

void IRAM_ATTR detectVibration() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastInterruptTime > 100) { // debounce
    vibrationDetected = true;
    lastInterruptTime = currentTime;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(VIB_PIN, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(VIB_PIN), detectVibration, CHANGE);

  mlx.begin();

  Serial.println("Multi-Sensor Monitoring Started");

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

  // 🔹 Print values
  Serial.print("Temp: ");
  Serial.print(currentTemp);
  Serial.print(" | Moisture: ");
  Serial.print(currentMoisture);
  Serial.print(" | Vibration: ");
  Serial.println(vibrationDetected ? "YES" : "NO");

  // 🔹 Dominant detection
  if (vibrationDetected) {
    Serial.println(">>> Dominant: VIBRATION <<<");
    vibrationDetected = false;
  }
  else if (tempChange > 0.5 && tempChange > moistureChange) {
    Serial.println(">>> Dominant: TEMPERATURE <<<");
  }
  else if (moistureChange > 50 && moistureChange > tempChange) {
    Serial.println(">>> Dominant: MOISTURE <<<");
  }
  else {
    Serial.println("No significant change");
  }

  Serial.println("-----------------------------");

  // 🔹 Update previous values
  prevTemp = currentTemp;
  prevMoisture = currentMoisture;

  delay(1000);
}