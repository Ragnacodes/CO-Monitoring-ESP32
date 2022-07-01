// Wifi and internet libraries
#include <WiFi.h>
#include <HTTPClient.h>
#include <Ethernet.h>
#include <EthernetClient.h>

// Define pins
#define ONBOARD_LED  2
#define SENSOR 33
//#define NORMAL_CONDITION_LIGHT 32
#define DANGEROUS_CONDITION_LIGHT 32

//#define LED

// global variables
float sensorValue, sensor_volt, RS_air, R0 = 0.34, ratio, RS_gas; 
HTTPClient http;


// ESP Serial
const char* DEVICE_SERIAL = "78e72d25-6b1e-445e-b565-18e5a320d7ef";
const char* DEVICE_ORG = "2a1f0c66-2c9f-4d1a-86b0-c4759efce4c1";

// Wifi information
bool isWifiConnected = false;

const char* WIFI_SSID = "Mostafa";
const char* WIFI_PASSWORD = "ntxa7145";


// Server information
const String BASE_URL = "http://188.121.110.10:8085/api/v1";
String url = BASE_URL + "/organizations/" + DEVICE_ORG + "/devices/" + DEVICE_SERIAL + "/metrics/co_density";

void setup() {
  // Setup serial
  Serial.begin(9600);
  Serial.println();

  // Setup pin modes
  pinMode(ONBOARD_LED,OUTPUT);
  pinMode(DANGEROUS_CONDITION_LIGHT,OUTPUT);
//  pinMode(NORMAL_CONDITION_LIGHT,OUTPUT);
  
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

  sensorValue = analogRead(SENSOR); 

//-----------------------------------------------/ 
 sensor_volt = ((float)sensorValue / 4096) * 5.0; 
 RS_gas = (5.0 - sensor_volt) / sensor_volt; // Depend on RL on yor module 
 ratio = RS_gas / R0; // ratio = RS/R0 
//------------------------------------------------------------/ 
 Serial.print("sensor_volt = "); 
 Serial.println(sensor_volt); 
 Serial.print("RS_ratio = "); 
 Serial.println(RS_gas); 
 Serial.print("Rs/R0 = "); 
 Serial.println(ratio); 
 Serial.print("\n\n"); 

  
  // HTTP Request
  http.begin(url);
  http.addHeader("Content-Type", "application/json");
//  String httpRequestData = "{\"density\":" + String((float)random(0, 1000)/10.00) + "}";
  String httpRequestData = "{\"density\":" + String(RS_gas) + "}";
  Serial.println("HTTP URL is " + url);
  Serial.println("HTTP Body is " + httpRequestData);
  int httpResponseCode = http.PUT(httpRequestData);
  Serial.print("The response code is: ");
  Serial.println(httpResponseCode);
  http.end();

  if(RS_gas<1)
  {
    digitalWrite(DANGEROUS_CONDITION_LIGHT, HIGH);
//    digitalWrite(NORMAL_CONDITION_LIGHT, HIGH);
  }
  else
  {
    digitalWrite(DANGEROUS_CONDITION_LIGHT, LOW);
//    digitalWrite(NORMAL_CONDITION_LIGHT, HIGH);
  }

  updateState();
  delay(1000);

}


void updateState() {
  digitalWrite(ONBOARD_LED, int(isWifiConnected));
}
