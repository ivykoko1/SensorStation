#ifndef SENSORSMANAGER_H
#define SENSORSMANAGER_H

#include <DHT_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define DHT_TYPE DHT11
#define FILTER_PASS 32

class SensorsManager
{
private:
    /* data */
    bool dht_temp_enabled = false;
    bool disabled = true;
    uint8_t resolution = 12;

public:
    SensorsManager(uint8_t DHT_PIN, uint8_t DS18B20_PIN);
    ~SensorsManager();
    //Enable DHT as temperature source.
    bool enableDHT();
    //Disable DHT as temperature source.
    bool disableDHT();
    //Enable sensors
    void begin();
    //Get temperature from sensor
    float getTemperature();
    //Get humidity from sensor
    uint8_t getHumidity();
    //Calculate heat index
    float getHeatIndex(float temperature, uint8_t humidity);
    //Set Resolution for DS18B20 (default = 12);
    uint8_t setResolution(uint8_t resolution);
    uint8_t getResolution() { return resolution; }
};

#endif // !