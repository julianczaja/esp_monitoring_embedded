#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "DataManager.h"

#define DEVICE_NAME "ESP Monitoring device"

#define INFO_SERVICE_UUID                       "691ff8e2-b4d3-4c2e-b72f-95b6e5acd64b"
#define DEVICE_ID_CHARACTERISTIC_UUID           "17a4e7f7-f645-4f67-a618-98037cb4372a"

#define SETTINGS_SERVICE_UUID                   "ecb44d46-93cf-45cc-bd34-b82205e80d7b"
#define FRAME_SIZE_CHARACTERISTIC_UUID          "2c0980bd-efc9-49e2-8043-ad94bf4bf81e"
#define PHOTO_INTERVAL_CHARACTERISTIC_UUID      "184711ee-5964-4f31-9d2b-5405da58a7d9"
#define SPECIAL_EFFECT_CHARACTERISTIC_UUID      "805fad03-b784-4bd0-8499-c86a2bc6f8d4"
#define WHITE_BALANCE_CHARACTERISTIC_UUID       "b243a39d-a337-4516-b183-d6cdbcc524e9"
#define QUALITY_CHARACTERISTIC_UUID             "06135106-f60d-4d46-858d-b8988f33aafa"
#define BRIGHTNESS_CHARACTERISTIC_UUID          "1d7e8059-f231-44f2-be7c-9cb51855c30b"
#define CONTRAST_CHARACTERISTIC_UUID            "2cd319ae-4bd4-4914-8571-80262adcbe16"
#define SATURATION_CHARACTERISTIC_UUID          "f974b4b3-d1c0-4170-87e0-bfe19a1caf22"
#define FLASH_ON_CHARACTERISTIC_UUID            "e8da265f-dba8-495e-b056-888b36bf9298"
#define VERTICAL_FLIP_CHARACTERISTIC_UUID       "597b76be-0816-4564-872c-0355df5d12fe"
#define HORIZONTAL_MIRROR_CHARACTERISTIC_UUID   "ae21155a-7cde-41d7-ad70-2b7daac1de59"

class SettingsServerCallbacks : public BLEServerCallbacks
{
private:
    void onConnect(BLEServer *server);
    void onDisconnect(BLEServer *server);
};

class SettingsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
public:
    SettingsCharacteristicCallbacks(DataManager dataManager);

    void onWrite(BLECharacteristic *characteristic);
    void onRead(BLECharacteristic *characteristic);

private:
    DataManager _dataManager;
};

class BLEManager
{
public:
    BLEManager(uint8_t deviceId, DataManager dataManager);

    void init();
    bool initialized = false;

private:
    void setupCharacteristics();
    BLEServer *_server;
    BLEService *_infoService;
    BLEService *_settingsService;
    BLEAdvertising *_advertising;
    uint8_t _deviceId;
    DataManager _dataManager;
    SettingsCharacteristicCallbacks *characteristicsCallback;
};

#endif
