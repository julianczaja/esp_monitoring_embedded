#ifndef BLEMANAGER_H
#define BLEMANAGER_H

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include "DataManager.h"

#define SERVICE_UUID          "691ff8e2-b4d3-4c2e-b72f-95b6e5acd64b"
#define CHARACTERISTIC_UUID_1 "5f05d250-c7a7-4d07-b342-92d4169700a5"
#define CHARACTERISTIC_UUID_2 "ad12eded-3750-443c-870f-7764d3877d80"

#define DEVICE_NAME "ESP MONITORING CONFIGURATOR"


class SettingsServerCallbacks : public BLEServerCallbacks
{
    private:
        void onConnect(BLEServer *server);
        void onDisconnect(BLEServer *server);
};

class SettingsCharacteristicCallbacks : public BLECharacteristicCallbacks
{
    public:
        void onWrite(BLECharacteristic *characteristic);
        void onRead(BLECharacteristic *characteristic);
};

class BLEManager
{
public:
    BLEManager(DataManager dataManager);

    void init();
    bool initialized = false;

private:
    void setupCharacteristics();
    BLEServer *_server;
    BLEService *_service;
    BLEAdvertising *_advertising;
    DataManager _dataManager;
    SettingsCharacteristicCallbacks *characteristicsCallback;
};

#endif
