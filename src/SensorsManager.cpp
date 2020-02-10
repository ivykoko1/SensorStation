#include "SensorsManager.h"

DHT_Unified* dht;
OneWire* oneWire;
DallasTemperature* dallasTemperature;

SensorsManager::SensorsManager(uint8_t DHT_PIN, uint8_t DS18B20_PIN)
{
    dht = new DHT_Unified(DHT_PIN, DHT_TYPE);
    oneWire = new OneWire(DS18B20_PIN);
    dallasTemperature = new DallasTemperature(oneWire);
    setResolution(resolution);
}

SensorsManager::~SensorsManager()
{
    
    disabled = true;
}

float SensorsManager::getTemperature(){
    if(dht_temp_enabled){
        sensors_event_t event;
        dht->temperature().getEvent(&event);
        return event.temperature;
    }else{
        float avg = 0;
        dallasTemperature->requestTemperatures();
        return dallasTemperature->getTempCByIndex(0);
    }
}

float SensorsManager::getHeatIndex(float temperature, uint8_t humidity){
    //TODO Heat Index Calculations
    return .0f;
}


void SensorsManager::begin()
{
    if (disabled)
    {
        dht->begin();
        dallasTemperature->begin();
        if (dallasTemperature->getDeviceCount() < 1)
        {
            #ifdef DEBUG
                Serial.println(F("No DS18B20 devices found!."));
            #endif
            dht_temp_enabled = true;
        }else{
            #ifdef DEBUG
                Serial.print(F("Number of DS18B20 found: "));
                Serial.println(dallasTemperature->getDeviceCount());
            #endif
            dht_temp_enabled = false;
        }
        
        disabled = false;
    }else
    {
        return;
    }
    
}

uint8_t SensorsManager::setResolution(uint8_t new_resolution)
{
    if (new_resolution >= 8 && new_resolution <= 16)
    {
        resolution = new_resolution;
        dallasTemperature->setResolution(resolution);
        #ifdef DEBUG
            Serial.print(F("Resolution set to: "));
            Serial.println(resolution);
        #endif 
    }else {}    
    return resolution;
}


boolean SensorsManager::enableDHT()
{
    dht_temp_enabled = true;
    return dht_temp_enabled;
}

boolean SensorsManager::disableDHT()
{
    dht_temp_enabled = false;
    return dht_temp_enabled;
}
