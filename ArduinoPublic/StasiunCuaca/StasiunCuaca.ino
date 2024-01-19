// https://www.electroniclinic.com/google-spreadsheet-or-google-sheets-with-esp8266-nodemcu-for-data-logging/
// 
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "DHT.h"
#include "WifiCredentials.h"

#define DHTTYPE DHT11 // type of the temperature sensor
const int DHTPin = 2; //--> The pin used for the DHT11 sensor is Pin D3 = GPIO5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router


//----------------------------------------Host & httpsPort
const char* host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

String GAS_ID = gas_id; //--> spreadsheet script ID

unsigned long startTime = 0;
unsigned long duration = 0.5 * 60 * 1000; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(500);

  dht.begin();  //--> Start reading DHT11 sensors
  delay(500);

  pinMode(D6, INPUT);
  
  WiFi.begin(ssid, password); //--> Connect to your WiFi router
  Serial.println("");
    
  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  //----------------------------------------Wait for connection
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(100);
    digitalWrite(ON_Board_LED, HIGH);
    delay(1000);
    //----------------------------------------
  }
  //----------------------------------------
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.
  Serial.println("");
  Serial.print("Successfully connected to : ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  //----------------------------------------

  client.setInsecure();
}

void loop() {
  if (millis() - startTime < duration) {
    digitalWrite(ON_Board_LED, LOW);
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (it's a very slow sensor)
    int h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    int sensorValue = analogRead(A0);
    float l = sensorValue; 

    int i = digitalRead(D6);
  
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println("Failed to read from DHT sensor !");
      delay(500);
      return;
    }
    String Temp = "Temperature : " + String(t) + " °C";
    String Humi = "Humidity : " + String(h) + " %";
    String Ldr = "Ldr : " + String(l) ;
    String isIndoor = "isIndoor : " + String(i) ;
    Serial.println(Temp);
    Serial.println(Humi);
    Serial.println(Ldr);
    Serial.println(isIndoor);
  
    sendData(t, h, l, i); // Calls the sendData Subroutine
    ESP.deepSleep(1800e6); // Sleep for 30 minutes
  } else {
    // If 2 minutes have passed, you can do something else here or go to sleep.
    // For example:
    ESP.deepSleep(1800e6); // Sleep for 30 minutes
  }
}

// Subroutine for sending data to Google Sheets
void sendData(float tem, int hum, int ldr, int isIndoor) {
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);
  
  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  String string_temperature =  String(tem);
  // String string_temperature =  String(tem, DEC); 
  String string_humidity =  String(hum, DEC); 
  String string_ldr =  String(ldr);
  String string_isIndoor =  String(isIndoor);
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature + "&humidity=" + string_humidity + "&ldr=" + string_ldr+ "&isIndoor=" + string_isIndoor;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
         "Host: " + host + "\r\n" +
         "User-Agent: BuildFailureDetectorESP8266\r\n" +
         "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  digitalWrite(ON_Board_LED, HIGH);
  //----------------------------------------
} 
//==============================================================================