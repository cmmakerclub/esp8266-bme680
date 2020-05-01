#include <MqttConnector.h>
#include "bsec.h"


extern int relayPinState;
extern MqttConnector* mqtt;

extern int relayPin;
extern int LED_PIN;

extern char myName[];

static void readSensor();

extern String DEVICE_NAME;
extern int PUBLISH_EVERY;
extern Bsec iaqSensor;

// extern Adafruit_BME680 bme;
float t,h,p;

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
    // data["temperature"] = bme.temperature;
    // data["humidity"] = bme.humidity;
    // data["pressure"] = bme.pressure;
    // data["altitude_hPa"] = bme.readAltitude(SEALEVELPRESSURE_HPA);
    // data["gas_kOhm"] = bme.gas_resistance / 1000.0;
    data["rawTemperature"] = iaqSensor.rawTemperature;
    data["rawHumidity"] = iaqSensor.rawHumidity;
    data["gasResistance"] = iaqSensor.gasResistance;
    data["iaq"] = iaqSensor.iaq;
    data["iaqAccuracy"] = iaqSensor.iaqAccuracy;
    data["temperature"] = iaqSensor.temperature;
    data["humidity"] = iaqSensor.humidity;
    data["pressure"] = iaqSensor.pressure;
    data["staticIaq"] = iaqSensor.staticIaq;
    data["co2Equivalent"] = iaqSensor.co2Equivalent;
    data["breathVocEquivalent"] = iaqSensor.breathVocEquivalent;
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
