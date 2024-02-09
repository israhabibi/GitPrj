#include <DHT.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int DHT_PIN = D4;

class DHTSensor {
private:
  DHT dht;

public:
  DHTSensor(int pin) : dht(pin, DHT22) {
    dht.begin();
  }

  float getTemperature() {
    return dht.readTemperature();
  }

  float getHumidity() {
    return dht.readHumidity();
  }
};

class WiFiConnector {
private:
  WiFiManager wifiManager;

public:
  void connect() {
    wifiManager.autoConnect("AutoConnectAP");
    Serial.println("Connected to WiFi");
  }
};

class DataSender {
private:
  const char *host = "script.google.com";  // Replace with your server address
  const int httpsPort = 443;
  String GAS_ID = "AKfycbxHSrSE57pGVARXhy-2Af526M9v8fXluioKWHzgcSMsT0WjpUpRcXy5jn4gqiO9Nent8A";

public:
  void sendData(float tem, int hum) {
    WiFiClientSecure client;
    Serial.println("==========");
    Serial.print("connecting to ");
    Serial.println(host);
    client.setInsecure();

    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }

    String string_temperature = String(tem);
    String string_humidity = String(hum, DEC);

    String url = "/macros/s/" + GAS_ID + "/exec?temperature=" + string_temperature +
                "&humidity=" + string_humidity;

    Serial.print("requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "User-Agent: ESP8266\r\n" +
                 "Connection: close\r\n\r\n");

    Serial.println("request sent");

    Serial.println("closing connection");
    Serial.println("==========");
    Serial.println();
    // digitalWrite(ON_Board_LED, HIGH);  // Assuming you have an LED on pin ON_Board_LED
  }
};

class DataDisplay {
public:
  static void displayData(float temperature, int humidity) {
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
  }
};


DHTSensor dhtSensor(DHT_PIN);
WiFiConnector wifiConnector;
DataSender dataSender;

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi using WiFiManager
  wifiConnector.connect();
}

void loop() {
  // Read sensor data
  float temperature = dhtSensor.getTemperature();
  int humidity = dhtSensor.getHumidity();

  // Display data on Serial Monitor
  DataDisplay::displayData(temperature, humidity);

  // Send data
  dataSender.sendData(temperature, humidity);

  delay(300000);  // Adjust the delay according to your needs
}
