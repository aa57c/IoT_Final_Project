#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 3       // Pin for DHT sensor
#define DHTTYPE DHT11  // DHT11 sensor
#define BUTTON_PIN 5   // Pin for emergency button

DHT dht(DHTPIN, DHTTYPE);

const int gasPin = A0;      // Analog pin for gas sensor
const int buzzer = 4;       // Digital pin for buzzer
const int collisionPin = 2; // Pin for the collision sensor

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Initialize LCD

unsigned long buzzerStartTime = 0;  // Timestamp for buzzer
const unsigned long buzzerDuration = 10000; // Buzzer duration in milliseconds
bool buzzerActive = false;          // State of the buzzer

void setup() {
  Serial.begin(9600);  // Initialize Serial Monitor
  dht.begin();         // Start DHT sensor
  pinMode(gasPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(collisionPin, INPUT_PULLUP); // Enable internal pull-up resistor for collision sensor
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Enable internal pull-up resistor for emergency button

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read button state
  int buttonState = digitalRead(BUTTON_PIN);

  bool emergencyAlert = false;
  bool collisionAlert = false;

  // Check for emergency button press
  if (buttonState == LOW) { // Button is pressed
    emergencyAlert = true;
    delay(500); // Debounce delay to prevent multiple messages
  }

  // Read temperature and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read gas sensor
  int gasValue = analogRead(gasPin);
  float gasConcentration = map(gasValue, 0, 1023, 0, 100); // Map to arbitrary range for ppm

  // Read collision sensor
  int collisionDetected = digitalRead(collisionPin);
  buzzerActive = false;

  // Check if the collision sensor is triggered
  if (collisionDetected == LOW) {
    collisionAlert = true;

    // Activate buzzer
    digitalWrite(buzzer, HIGH);

    // Display collision alert on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("COLLISION ALERT!");
    lcd.setCursor(0, 1);
    lcd.print("Impact Detected");

    // Keep the message and buzzer active for 5 seconds
    unsigned long startTime = millis();
    while (millis() - startTime < 5000) {
      // Wait for 5 seconds, keeping buzzer on
    }

    // Turn off the buzzer after the delay
    digitalWrite(buzzer, LOW);

    // Clear the LCD for other messages
    lcd.clear();
  }
  else{
    collisionAlert = false;
  }

  if (gasConcentration > 20) { // Hazard thresholds
    // Hazard Alert
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HAZARD ALERT!");
    lcd.setCursor(0, 1);
    lcd.print("Gas: ");
    lcd.print(gasConcentration);
    lcd.print("ppm");
    delay(2000); // Keep alert on screen for 2 seconds
    lcd.clear();
    return; // Skip to next iteration
  } else if (temperature > 30) {
    // Hazard Alert
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("HAZARD ALERT!");
    lcd.setCursor(0, 1);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    delay(2000); // Keep alert on screen for 2 seconds
    lcd.clear();
    return; // Skip to next iteration
  } else if (!buzzerActive) {
    // Normal state
    digitalWrite(buzzer, LOW);
  }

  // Flipping between screens
  static bool displayTemperatureAndGas = true; // Toggle flag for screens

  if (displayTemperatureAndGas) {
    // Display Temperature and Gas concentration
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Gas: ");
    lcd.print(gasConcentration);
    lcd.print("ppm");
  } else {
    // Display Humidity
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }

  // Toggle the screen
  displayTemperatureAndGas = !displayTemperatureAndGas;

  // Send sensor data over Serial to ESP32
  Serial.print(gasConcentration);
  Serial.print(",");
  Serial.print(temperature);
  Serial.print(",");
  Serial.print(humidity);
  Serial.print(",");
  Serial.print(collisionAlert ? "true" : "false");
  Serial.print(",");
  Serial.println(emergencyAlert ? "true" : "false");

  delay(3000); // Small delay between screen updates
}
