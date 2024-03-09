#include <WiFi.h>
#include "DHTesp.h"
#include "ThingSpeak.h"
#include <ESP32Servo.h>

const int DHT_PIN = 15;
const int LED_RED = 13;
const int LED_GREEN = 12;
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const int myChannelNumber = 2366097;
const char* myApiKey = "6JWVUC6RCUC5WA5V";
const char* server = "api.thingspeak.com";
Servo servo;
DHTesp dhtSensor;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(35, INPUT); 
  servo.attach(32);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("Wifi not connected");
  }
  Serial.println("Wifi connected !");
  Serial.println("Local IP: " + String(WiFi.localIP()));
  WiFi.mode(WIFI_STA);

  ThingSpeak.begin(client);
}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  ThingSpeak.setField(1,data.temperature);
  ThingSpeak.setField(2,data.humidity);
  

  if (data.temperature > 60 && data.temperature < 80) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
  }
  else
  {
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
  }
  
  int x = ThingSpeak.writeFields(myChannelNumber,myApiKey);
  
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  
  if(x == 200 || x == -401){
    Serial.println("Data pushed successfull");
  }else{
    Serial.println("Push error" + String(x));
  }
  Serial.println("---");

  delay(10000);

  
  int input = digitalRead(35); //Reads the value of the 8 pin
  Serial.println(input); //Prodcues if it is high or low on the terminal
  if(input == 1) { //Checks if the motion sensor has been tripped
    int position = 0; //Makes current position of servo as 0
      servo.write(100); //Makes the servo motor move by 100 degrees
      delay(1000);
    
  }
if(input == 0){ //If not trippped
  servo.write(0);//Returns the servo back to original position
        delay(1000);
}
  delay(1800);
}
