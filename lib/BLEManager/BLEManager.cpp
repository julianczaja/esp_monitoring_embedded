#include "BLEManager.h"

BLEManager::BLEManager(DataManager dataManager)
{
    _dataManager = dataManager;
    initialized = false;
}

void BLEManager::init()
{
    Serial.println("BLEManager::init()");

    BLEDevice::init(DEVICE_NAME);

    _server = BLEDevice::createServer();
    _server->setCallbacks(new SettingsServerCallbacks());

    _service = _server->createService(SERVICE_UUID);

    setupCharacteristics();

    _service->start();

    _advertising = BLEDevice::getAdvertising();
    _advertising->addServiceUUID(SERVICE_UUID);
    _advertising->setScanResponse(true);
    _advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    _advertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("BLEManager::init() DONE");

    initialized = true;
}

void BLEManager::setupCharacteristics()
{
    characteristicsCallback = new SettingsCharacteristicCallbacks();

    // TODO: Make characteristic for every setting of camera

    BLECharacteristic *characteristic1 =
        _service->createCharacteristic(CHARACTERISTIC_UUID_1, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic1->setValue("characteristic1"); // TODO: Read default value from EEPROM
    characteristic1->setCallbacks(characteristicsCallback);

    BLECharacteristic *characteristic2 =
        _service->createCharacteristic(CHARACTERISTIC_UUID_2, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    characteristic2->setValue("characteristic2"); // TODO: Read default value from EEPROM
    characteristic2->setCallbacks(characteristicsCallback);
}

void SettingsServerCallbacks::onConnect(BLEServer *server)
{
    Serial.println("SettingsServerCallbacks::onConnect");
};

void SettingsServerCallbacks::onDisconnect(BLEServer *server)
{
    Serial.println("SettingsServerCallbacks::onDisconnect");
}

void SettingsCharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
    String uuid = characteristic->getUUID().toString().c_str();
    Serial.printf("SettingsCharacteristicCallbacks::onWrite -> BT UUID write received %s\n", uuid.c_str());

    std::string value = characteristic->getValue();
    if (uuid == CHARACTERISTIC_UUID_1)
    {
        Serial.printf("CHARACTERISTIC_UUID_1 value = %s\n", value.c_str());
    }

    if (uuid == CHARACTERISTIC_UUID_2)
    {
        Serial.printf("CHARACTERISTIC_UUID_2 value = %s\n", value.c_str());
    }
}

void SettingsCharacteristicCallbacks::onRead(BLECharacteristic *characteristic)
{
    std::string msg = characteristic->getValue();
    Serial.printf("SettingsCharacteristicCallbacks::onRead -> BLE received: %s\n", msg.c_str());
}
