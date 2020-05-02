#include <MqttConnector.h>
#include "bsec.h"
#include <Adafruit_ADS1015.h>


extern int relayPinState;
extern MqttConnector* mqtt;

extern int relayPin;
extern int LED_PIN;

extern char myName[];

static void readSensor();

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;

extern Adafruit_ADS1115 *ads;
// extern Bsec iaqSensor;
// extern Adafruit_BME280 bme;
// float t,h,p;

void register_publish_hooks() {
  strcpy(myName, DEVICE_NAME.c_str());
  mqtt->on_prepare_data_once([&](void) {
    Serial.println("initializing sensor...");
  });

  mqtt->on_before_prepare_data([&](void) {
    readSensor();
  });

  mqtt->on_prepare_data([&](JsonObject *root) {
    JsonObject& data = (*root)["d"];
    JsonObject& info = (*root)["info"];
    data["myName"] = myName;
    data["millis"] = millis();
      int16_t adc4, adc5, adc6, adc7;
    adc4 = ads->readADC_SingleEnded(0);
    adc5 = ads->readADC_SingleEnded(1);
    adc6 = ads->readADC_SingleEnded(2);
    adc7 = ads->readADC_SingleEnded(3);
    // data["analogCH0"] = adc0;
    // data["analogCH1"] = adc1;
    // data["analogCH2"] = adc2;
    // data["analogCH3"] = adc3;
    data["analogCH4"] = adc4;
    data["analogCH5"] = adc5;
    data["analogCH6"] = adc6;
    data["analogCH7"] = adc7;
    // data["temperature"] = bme.readTemperature();
    // data["humidity"] = bme.readHumidity();
    // data["pressure"] = bme.readPressure();
    // data["humidity"] = bme.humidity;
    // data["pressure"] = bme.pressure;
    // data["altitude_hPa"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
    // data["gas_kOhm"] = bme.gas_resistance / 1000.0;
    data["relayState"] = relayPinState;
    data["updateInterval"] = PUBLISH_EVERY;
    data["A0"] = analogRead(A0);
  }, PUBLISH_EVERY);

  mqtt->on_after_prepare_data([&](JsonObject * root) {
    /**************
      JsonObject& data = (*root)["d"];
      data.remove("version");
      data.remove("subscription");
    **************/
  });

  mqtt->on_published([&](const MQTT::Publish & pub) {
      Serial.println("Published.");
  });
}

static void readSensor() {
  //   if (! bme.performReading()) {
  //     Serial.println("Failed to perform reading :(");
  //     return;
  //   }
  // Serial.print("Temperature = ");
  // Serial.print(bme.temperature);
  // Serial.println(" *C");
  //
  // Serial.print("Pressure = ");
  // Serial.print(bme.pressure / 100.0);
  // Serial.println(" hPa");
  //
  // Serial.print("Humidity = ");
  // Serial.print(bme.humidity);
  // Serial.println(" %");
  //
  // Serial.print("Gas = ");
  // Serial.print(bme.gas_resistance / 1000.0);
  // Serial.println(" KOhms");
  //
  // Serial.print("Approx. Altitude = ");
  // Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  // Serial.println(" m");
  //
  // t = String(bme.readTemperature()).toFloat();
  // h = String(bme.readHumidity()).toFloat();
  // p = String(bme.readPressure()).toFloat();
  //
  // if ((isnan(h) || h == 0)  ||  (isnan(t) || t == 0)) {
  //   Serial.println("read bme280 failed... try again..");
  // }
  // else {
  //   Serial.printf("Temperature: %.2f, Humidity: %.2f, Sensor Pressure = %.2f\r\n", t, h, p);
  // }

}
