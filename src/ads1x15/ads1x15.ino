#include <Arduino.h>
#define SEALEVELPRESSURE_HPA (1013.7)
#define BME680_DEBUG 1

#ifdef ESP8266
  #include <ESP8266WiFi.h>
#else
  #include <WiFi.h>
#endif
#include <Adafruit_Sensor.h>
// #include "Adafruit_BME280.h"
#include <ArduinoJson.h>
#include <MqttConnector.h>
#include <Wire.h>
#include <SPI.h>

#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "_config.h"
#include <Adafruit_ADS1015.h>

MqttConnector *mqtt;
Adafruit_ADS1115 *ads;
//    0x49
// Adafruit_BME280 bme;

int relayPinState       = HIGH;
int relayPin            = 15;
int LED_PIN             = 2;



char myName[40];

void init_hardware()
{
  pinMode(relayPin, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(relayPin, relayPinState);;
  // serial port initialization
  Serial.begin(115200);
  Wire.begin();
  ads = new Adafruit_ADS1115(0x49);
  ads->setGain(GAIN_TWOTHIRDS);
    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  // GAIN_TWOTHIRDS (for an input range of +/- 6.144V)
  // GAIN_ONE (for an input range of +/-4.096V)
  // GAIN_TWO (for an input range of +/-2.048V)
  // GAIN_FOUR (for an input range of +/-1.024V)
  // GAIN_EIGHT (for an input range of +/-0.512V)
  // GAIN_SIXTEEN (for an input range of +/-0.256V)
  ads->begin();


  // *libalgobsec.a:(.literal.* .text.*)
  // *(.irom0.literal .irom.literal .irom.text.literal .irom0.text .irom0.text.* .irom.text .irom.text.*)
    // if (!bme.begin(0x76)) {
    //     Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    //     Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    //     Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    //     Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    //     Serial.print("        ID of 0x60 represents a BME 280.\n");
    //     Serial.print("        ID of 0x61 represents a BME 680.\n");
    //   while (1) {
    //   };
    // }
  delay(10);
  Serial.println();

}

void init_wifi() {
  WiFi.disconnect();
  delay(20);
  WiFi.mode(WIFI_STA);
  delay(50);
  const char* ssid =  WIFI_SSID.c_str();
  const char* pass =  WIFI_PASSWORD.c_str();
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf ("Connecting to %s:%s\r\n", ssid, pass);
    delay(300);
  }
  Serial.println("WiFi Connected.");
  digitalWrite(LED_PIN, HIGH);
}

void setup()
{
  init_hardware();
  init_wifi();
  init_mqtt();
}

void loop()
{
   // unsigned long time_trigger = millis();
    mqtt->loop();
}


void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
