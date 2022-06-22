// Wifi and internet libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

// Define pins
#define ONBOARD_LED  2
#define sensor 33
//#define LED

// global variables
float sensorValue; 
HTTPClient http;


// ESP Serial
const char* DEVICE_SERIAL = "78e72d25-6b1e-445e-b565-18e5a320d7ef";
const char* DEVICE_ORG = "2a1f0c66-2c9f-4d1a-86b0-c4759efce4c1";

// Wifi information
bool isWifiConnected = false;
//const char* WIFI_SSID = "Mike Wazowski";
//const char* WIFI_PASSWORD = "6e3003547ebdad";

const char* WIFI_SSID = "Mostafa";
const char* WIFI_PASSWORD = "ntxa7145";

//const char* WIFI_SSID = "Galaxy";
//const char* WIFI_PASSWORD = "uihe4654";


// Server information
const String BASE_URL = "http://188.121.110.10:8085/api/v1";
String url = BASE_URL + "/organizations/" + DEVICE_ORG + "/devices/" + DEVICE_SERIAL + "/metrics/co_density";
void setup() {
  // Setup serial
  Serial.begin(9600);
  Serial.println();

  // Setup pin modes
  pinMode(ONBOARD_LED,OUTPUT);
//  pinMode(LED, OUTPUT);

  // Setup Wifi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  isWifiConnected = true;
}

void loop() {
  sensorValue = analogRead(sensor); 
  Serial.println(sensorValue);

  updateState();
  delay(1000);
  
  // HTTP Request
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
//  String httpRequestData = "{\"density\":" + String((float)random(0, 1000)/10.00) + "}";
  String httpRequestData = "{\"density\":" + String(sensorValue) + "}";
  Serial.println("HTTP URL is " + url);
  Serial.println("HTTP Body is " + httpRequestData);
  int httpResponseCode = http.PUT(httpRequestData);
  Serial.print("The response code is: ");
  Serial.println(httpResponseCode);
  http.end();
}


void updateState() {
  digitalWrite(ONBOARD_LED, int(isWifiConnected));
}
