#include <WiFiNINA.h>
#include <ArduinoHttpClient.h>

// network credentials
char ssid[] = "aaaa";
char pass[] = "123@";

// Firebase Realtime Database details
const char* firebaseHost = "remotetrafficctrl-default-rtdb.firebaseio.com";
const int httpsPort = 443;

// Pins for LEDs
const int redPin = 2;
const int greenPin = 3;
const int bluePin = 4;

// Create WiFi and HTTP client
WiFiSSLClient wifiClient;
HttpClient client = HttpClient(wifiClient, firebaseHost, httpsPort);

void setup() {
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  // Connect to WiFi
  Serial.print("Connecting to WiFi");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected!");
}

void loop() {
  client.get("/leds.json"); // REST endpoint to get all LED states

  int statusCode = client.responseStatusCode();
  String response = client.responseBody();

  Serial.println("Status code: " + String(statusCode));
  Serial.println("Response: " + response);

  // Parse simple response manually
  bool redState = response.indexOf("\"red\":true") > 0;
  bool greenState = response.indexOf("\"green\":true") > 0;
  bool blueState = response.indexOf("\"blue\":true") > 0;

  digitalWrite(redPin, redState ? HIGH : LOW);
  digitalWrite(greenPin, greenState ? HIGH : LOW);
  digitalWrite(bluePin, blueState ? HIGH : LOW);

  delay(2000); // Poll every 2 seconds
}
