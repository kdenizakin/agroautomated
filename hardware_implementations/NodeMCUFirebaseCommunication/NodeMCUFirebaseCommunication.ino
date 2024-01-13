// Import required libraries
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebSrv.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h> //Wifi modulünün headırı
#include <ArduinoJson.h>
#include <ESP8266Firebase.h>
#include <ESP8266WiFi.h>
#include <cstring>
#include <iostream>
using namespace std;

#define _SSID "SUPERONLINE_WiFi_1709"// WiFi name 
#define _PASSWORD "3cdSg3cyexbe"//WiFi Password 
#define REFERENCE_URL "agroautomated-8f55e-default-rtdb.firebaseio.com"  //Project reference url 
//#define FIREBASE_AUTH "H5EagCXkQeQLME8NcmKP7h6GUHq4gaTEHzeSCusi"
Firebase firebase(REFERENCE_URL);

SoftwareSerial NodeMCU(D6,D7); //RX ve TX pinleri

char sensorValues[100];
const char* received_String;
float received_distance;
int received_temperature;
int received_humidity;
int received_soil_moisture;

bool ledState = 0;
const int ledPin = 2;

void setup()
{
  Serial.begin(115200);
  NodeMCU.begin(4800);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP Address: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  digitalWrite(LED_BUILTIN, HIGH);
} 

void loop() 
{
  int i = 0;
  char *line_divided[20];
  //Now receive sensor data from the Arduino.
  if(NodeMCU.available()>0){ //eğer burada Arduinoya bir data transferi varsa condition satisfied.
    Serial.println("Data received from the Arduino UNO board!");
    String nodeMCUString = NodeMCU.readString(); // Read the String from Arduino
    nodeMCUString.toCharArray(sensorValues, 100); // Convert String to char array    
    line_divided[0] = strtok(sensorValues, "|"); // Distance (float)
    line_divided[1] = strtok(NULL, "|"); // Temperature (integer)
    line_divided[2] = strtok(NULL, "|"); // Humidity (integer)
    line_divided[3] = strtok(NULL, "|"); // Soil Water Level. (integer)

     // Write sensor data to the database.
    Serial.println("Now writing data to Firebase Realtime Database...");
    firebase.setString("sensor_data/information_string", "This data corresponds to analog sensor data.");
    firebase.setFloat("sensor_data/distance_float", atof(line_divided[0]));
    firebase.setInt("sensor_data/temperature_integer", atoi(line_divided[1]));
    firebase.setInt("sensor_data/humidity_integer", atoi(line_divided[2]));
    firebase.setInt("sensor_data/soil_moisture_integer", atoi(line_divided[3]));
    Serial.println("Writing data has been finished! ");
    Serial.println("Now reading the data...");
    NodeMCU.print(1);
    firebase.json(true);    
    String data = firebase.getString("sensor_data");  // Get data from the database.
    const size_t capacity = JSON_OBJECT_SIZE(3) + 50;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, data);
    received_String = doc["information_string"]; 
    received_distance = doc["distance_float"]; 
    received_temperature = doc["temperature_integer"];              
    received_humidity = doc["humidity_integer"];              
    received_soil_moisture = doc["soil_moisture_integer"];         
    // Print data
    Serial.print("Received String:\t");
    Serial.println(received_String);
    Serial.print("Received Distance:\t");
    Serial.println(received_distance);
    Serial.print("Received Temperature:\t");
    Serial.println(received_temperature);
    Serial.print("Received Humidity:\t");
    Serial.println(received_humidity);
    Serial.print("Received Soil Moisture:\t");
    Serial.println(received_soil_moisture);
    
  }



}