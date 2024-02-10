#include <DHT.h>
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

const int DHT_PIN = D4;
const int LED = D8;


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
  const char *host = "35.184.88.52";  // Replace with your server address
  const int httpsPort = 8080;

public:
  void sendData(float tem, int hum) {
    WiFiClient client;
    Serial.println("==========");
    Serial.print("connecting to ");
    Serial.println(host);

    if (!client.connect(host, httpsPort)) {
      Serial.println("connection failed");
      return;
    }

    String string_temperature = String(tem);
    String string_humidity = String(hum, DEC);

    String url = "/api/data?temperature=" + String(string_temperature) + "&humidity=" + String(string_humidity);


    Serial.print("requesting URL: ");
    Serial.println(url);

    // Make an HTTP GET request
    client.println("GET " + url + " HTTP/1.1");
    client.println("Host: " + String(host));
    client.println("Connection: close");
    client.println();

    Serial.println("GET request sent");

    // Wait for server response
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\r');
        Serial.println(line);
      }
    }

    // Close the connection
    client.stop();

    digitalWrite(LED, HIGH);  // Assuming you have an LED on pin ON_Board_LED
    delay(1000);
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
