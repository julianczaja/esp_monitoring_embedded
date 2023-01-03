#include "DataManager.h"

DataManager::DataManager() {}

void DataManager::init()
{
    Serial.print("Failure count: ");
    Serial.println(getFailuresCount());
}

void DataManager::setWiFiConfiguration(WiFiConfiguration *wifiConfiguration)
{
    Serial.println("\n> DataManager::setWiFiConfiguration");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    EEPROM.put(WIFI_CONFIGURATION_ADDRESS, *wifiConfiguration);
    delay(50);
    EEPROM.end();
}

void DataManager::getWiFiConfiguration(WiFiConfiguration *wifiConfiguration)
{
    Serial.println("\n> DataManager::getWiFiConfiguration");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    EEPROM.get(WIFI_CONFIGURATION_ADDRESS, *wifiConfiguration);
    delay(50);
    EEPROM.end();
}

void DataManager::increaseFailuresCount()
{
    Serial.println("\n> DataManager::increaseFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    uint16_t failuresCount = EEPROM.readUShort(FAILURES_COUNT_ADDRESS);
    delay(50);
    EEPROM.writeUShort(FAILURES_COUNT_ADDRESS, failuresCount + 1);
    delay(50);
    EEPROM.end();
}

uint16_t DataManager::getFailuresCount()
{
    Serial.println("\n> DataManager::getFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    uint16_t failuresCount = EEPROM.readUShort(FAILURES_COUNT_ADDRESS);
    delay(50);
    EEPROM.end();

    return failuresCount;
}

void DataManager::resetFailuresCount()
{
    Serial.println("\n> DataManager::resetFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    EEPROM.writeUShort(FAILURES_COUNT_ADDRESS, (uint16_t)0);
    delay(50);
    EEPROM.end();
}

void DataManager::clearAll()
{
    Serial.println("\n> DataManager::clearAll");

    EEPROM.begin(EEPROM_SIZE);
    delay(50);
    for (int i = 0; i < EEPROM_SIZE; ++i)
    {
        EEPROM.write(i, 0);
    }
    delay(50);
    EEPROM.end();
}
