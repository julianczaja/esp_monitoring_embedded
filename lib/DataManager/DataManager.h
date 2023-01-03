#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include "WiFiConfiguration.h"

#define WIFI_CONFIGURATION_ADDRESS 0
#define FAILURES_COUNT_ADDRESS 64
#define EEPROM_SIZE 128

class DataManager
{
public:
    DataManager();

    void init();

    void setWiFiConfiguration(WiFiConfiguration *wifiConfiguration);
    void getWiFiConfiguration(WiFiConfiguration *wifiConfiguration);

    void increaseFailuresCount();
    void resetFailuresCount();
    uint16_t getFailuresCount();

    void clearAll();

private:
    bool _isInitialized = false;
};

#endif