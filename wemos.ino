#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WIFI_SSID "<Your Wifi Name>"
#define WIFI_PASS "<Your WiFi Password>"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "<Your Adafruit IO Username>"
#define MQTT_PASS "<Your Adafruit IO Key>"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

// I named my adafruit feed "openclose"
Adafruit_MQTT_Subscribe openclose = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/openclose");

boolean curtainOpen = false;

// Motor -------------------------

#define SLEEP 16 //6 actually enable
#define STEP 13 //4
#define DIR 0 //2

#define STEPS_PER_ROTATION 1200

//End of motor--------------------


void setup()
{
  Serial.begin(9600);

  //Connect to WiFi
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println("Connected");

  //Subscribe to the openclose topic
  mqtt.subscribe(&openclose);

  // Motor -------------------

  pinMode(SLEEP, OUTPUT);
  pinMode(STEP, OUTPUT);
  pinMode(DIR, OUTPUT);

  //end motor ---------------


}

void loop()
{

  //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &openclose)
    {
      //Print the new value to the serial monitor
      Serial.print("openclose: ");
      Serial.println((char*) openclose.lastread);

      //If the switch is triggered and the curtain is closed, open it
      //Otherwise, if the curtain is open, close the curtain.
      if (!strcmp((char*) openclose.lastread, "trigger") && !curtainOpen)
      {
        openCurtain();
        curtainOpen = true;
      }
      else if (!strcmp((char*) openclose.lastread, "trigger") && curtainOpen)
      {
        closeCurtain();
        curtainOpen = false;
      }
    }
  }

  // ping the server to keep the mqtt connection alive
  if (!mqtt.ping()) {
    mqtt.disconnect();
  }

}

void openCurtain() {
  Serial.print("Opening the curtain...");

  Serial.println(F("dir HIGH"));
  digitalWrite(DIR, HIGH);
  moveCurtain();
  Serial.println(F("SLEEP LOW"));
  digitalWrite(SLEEP, LOW);
  Serial.print("Done opening curtain.");

  //delay(1000);                  // waits for a second

}


void closeCurtain() {
  Serial.print("Closing the curtain...");

  Serial.println(F("dir LOW"));
  digitalWrite(DIR, LOW);
  moveCurtain();
  Serial.println(F("SLEEP LOW"));
  digitalWrite(SLEEP, LOW);
  Serial.print("Done closing curtain");

  //delay(1000);                  // waits for a second
}

// for the motor -------------

void stepNow(int totalSteps) {
  Serial.print(totalSteps);
  Serial.println(F(" steps."));

  int i;
  for (i = 0; i < totalSteps; ++i) {
    digitalWrite(STEP, HIGH);
    delay(1);
    digitalWrite(STEP, LOW);
    delay(1);
  }
}

//----------------------------


void moveCurtain() {
  delay(5);
  stepNow(STEPS_PER_ROTATION);
}

void MQTT_connect()
{
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected())
  {
    return;
  }

  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5);  // wait 5 seconds
    retries--;
    if (retries == 0)
    {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
