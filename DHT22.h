#ifndef DHT22_H
#define DHT22_H
#include <Arduino.h>

class DHT22()
{
  private:

  public:
    DHT22(); //default constructor call
    void activateDHT22();
    void getTemperature();
    void getHumidity();
    void prepairDHT4LoRaWan();


};



//constructor
DHT22::DHT22() //default constructor
{

  DHT_Unified dht(DHTPIN, DHTTYPE);
  uint32_t delayMS = 50;
  bool DHTactivated = true;
  bool messureTemperature = true;  //true means only Temperature, false means only Humidity

}


//activate the DHT22 Sensor

DHT22.activateDHT22()
{
  Wire.begin();
  dht.begin();
}


//Get the Temperature
DHT22::getTemperature()
{

}

DHT22::getHumidity()
{

}

DHT22::prepairDHT4LoRaWan() {
  binSensorData = 0b0000000000000000;
  binPlatformData = 0b0000000000000000;
  Serial.println("[Messurment]:Getting Data from DHT22");
  //dht.begin();
  delay(50);
  sensors_event_t event;
  // Delay between measurements.
  delay(2000);
  if (DHTactivated == false)
  {

    DHTactivated = true;
    Serial.print("[Messurment]:DHT22 activated\n");
  }
  /////////////////***************--Get temperature event and print its value--****************/////////////////
  if (DHTactivated == true)
  {
    if ( messureTemperature == true) {
      if (isnan(event.temperature))
      {
        Serial.println(F("[Messurment]:Error reading temperature!"));

      }
      else
      {
        // Delay between measurements.
        delay(2000);
        dht.temperature().getEvent(&event);
        // Get temperature event and print its value.
        Serial.print(F("[Messurment]:Temperature: "));
        delay(50);
        sensorDataTemperature = event.temperature;
        temperature4LED = sensorDataTemperature;
        delay(500);
        Serial.print(sensorDataTemperature);
        Serial.print(F(" °C\n"));
        //Convert Deicimal Value in Binary in binSensorData
        decToBinary(sensorDataTemperature);
        //Convert Binary for Zenner-Logic in binPlatformData
        zennerParserPrepair();
        binaryTemperature = binPlatformData;

      }
    }
    /////////////////***************--Get humidity event and print its value--****************/////////////////
    if (messureTemperature == false) {
      // Delay between measurements.
      delay(delayMS);
      dht.humidity().getEvent(&event);
      delay(delayMS);
      if (isnan(event.relative_humidity))
      {
        Serial.println(F("[Messurment]:Error reading humidity!"));
      }
      else
      {
        Serial.print(F("[Messurment]:Humidity: "));
        sensorDataHumidity = event.relative_humidity;
        humidity4LED = sensorDataHumidity;
        Serial.print(event.relative_humidity);
        Serial.print(F("%\n"));
        //Convert Deicimal Value in Binary in binSensorData
        decToBinary(sensorDataHumidity);
        //Convert Binary for Zenner-Logic in binPlatformData
        zennerParserPrepair();
        binaryHumidity = binPlatformData;

      }
    }
  }
}
#endif
