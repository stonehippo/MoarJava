#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "MoarJava.h"
#include "WifiDefaultAuth.h"
#include "AIOKeys.h"

WiFiClientSecure client;

const int WIFI_TIMEOUT = 10000; // 10 seconds

const int FSR_PIN = A0; // use the  analog pin

int fsrReading;

Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Publish moarJava = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/MoarJava");

void setup() {

  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  if (!network_connect((char *)WLAN_SSID, (char *)WLAN_PASS)) {
    while(1);
  }
}

void loop() {
  MQTT_connect();

  // fsrReading = analogRead(FSR_PIN);
  fsrReading = analogRead(FSR_PIN);

  Serial.print("RC reading = ");
  Serial.println(fsrReading);

  moarJava.publish(fsrReading);

  delay(3000);
}

bool network_connect(char ssid[], char password[]) {
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


  WiFi.begin(ssid, password);

  long count = millis();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if ((millis() - count) > WIFI_TIMEOUT) {
      break;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();

    Serial.println("WiFi connected");
    Serial.println("IP address: "); Serial.println(WiFi.localIP());
    return true;
  }

  Serial.println("Wifi connection failed");
  WiFi.disconnect(true);
  return false;
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
     Serial.println(mqtt.connectErrorString(ret));
     Serial.println("Retrying MQTT connection in 5 seconds...");
     mqtt.disconnect();
     delay(5000);  // wait 5 seconds
     retries--;
     if (retries == 0) {
       // basically die and wait for WDT to reset me
       while (1);
     }
  }
  Serial.println("MQTT Connected!");
}
