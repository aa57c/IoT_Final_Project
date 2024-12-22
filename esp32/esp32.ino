#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "TomorrowLand";        // Replace with your Wi-Fi SSID
const char *password = "fsai1507";        // Replace with your Wi-Fi password
const char *serverURL = "http://192.168.0.110:5000/api/send-sensor-data";  // Backend API endpoint

void setup() {
  Serial.begin(115200);  // Initialize Serial communication for debugging (ESP32 to PC)
  Serial1.begin(9600, SERIAL_8N1, 16, 17);  // Initialize Serial1 for communication with Arduino (RX = GPIO 16, TX = GPIO 17)

  WiFi.begin(ssid, password);

  // Wait for the ESP32 to connect to Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void sendDataToBackend(String temp, String humid, String gas, String collision, String emergency) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient Http;
    Http.begin(serverURL);

    // Set headers for JSON
    Http.addHeader("Content-Type", "application/json");

    // Prepare the JSON payload
    String payload = "{\"temp\": \"" + temp +
                 "\", \"humidity\": \"" + humid +
                 "\", \"gas_level\": \"" + gas +
                 "\", \"collision_alert\": \"" + collision +
                 "\", \"emergency_alert\": \"" + emergency + 
                 "\"}";

    Serial.println("Sending Payload: " + payload);

    // Send POST request
    int httpResponseCode = Http.POST(payload);
    if (httpResponseCode > 0) {
      Serial.println("Data sent successfully!");
      Serial.println("HTTP Response code: " + String(httpResponseCode));

      // Log server response
      String response = Http.getString();
      Serial.println("Server Response: " + response);
    } else {
      Serial.println("Error sending data: " + String(httpResponseCode));
    }

    // Close the connection
    Http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void loop() {
  // Check if Arduino is sending data over Serial1 (using GPIO 16 for RX, GPIO 17 for TX)
  if (Serial1.available()) {
    String sensorData = Serial1.readStringUntil('\n'); // Read all the data sent from Arduino

    sensorData.trim(); // Remove any trailing newline characters

    // Print the received data to the Serial Monitor for debugging
    Serial.println("Received data: " + sensorData);

    // Split the data by commas (temperature, humidity, gas level, collision, emergency)
    int firstComma = sensorData.indexOf(',');
    int secondComma = sensorData.indexOf(',', firstComma + 1);
    int thirdComma = sensorData.indexOf(',', secondComma + 1);
    int fourthComma = sensorData.indexOf(',', thirdComma + 1);

    String gasLevelStr = sensorData.substring(0, firstComma);
    String tempStr = sensorData.substring(firstComma + 1, secondComma);
    String humidityStr = sensorData.substring(secondComma + 1, thirdComma);
    String collisionStr = sensorData.substring(thirdComma + 1, fourthComma);
    String emergencyStr = sensorData.substring(fourthComma + 1);

    // Remove any trailing spaces in collision and emergency data
    collisionStr.trim();
    emergencyStr.trim();

    Serial.println("Data: " + tempStr + "," + humidityStr + "," + gasLevelStr + "," + collisionStr + "," + emergencyStr);

    // Send data to the backend via POST request
    sendDataToBackend(tempStr, humidityStr, gasLevelStr, collisionStr, emergencyStr);
  }

  delay(2000);  // Adjust delay as needed
}