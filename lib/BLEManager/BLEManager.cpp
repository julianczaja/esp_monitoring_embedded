#include "BLEManager.h"

BLEManager::BLEManager(uint8_t deviceId, DataManager dataManager)
{
    _deviceId = deviceId;
    _dataManager = dataManager;
    initialized = false;
}

void BLEManager::init()
{
    Serial.println("BLEManager::init()");

    BLEDevice::init(DEVICE_NAME);

    _server = BLEDevice::createServer();
    _server->setCallbacks(new SettingsServerCallbacks());

    _infoService = _server->createService(BLEUUID(INFO_SERVICE_UUID), 1U, 0U);
    _settingsService = _server->createService(BLEUUID(SETTINGS_SERVICE_UUID), 24U, 1U);

    setupCharacteristics();

    _infoService->start();
    _settingsService->start();

    _advertising = BLEDevice::getAdvertising();
    _advertising->addServiceUUID(INFO_SERVICE_UUID);
    _advertising->addServiceUUID(SETTINGS_SERVICE_UUID);
    _advertising->setScanResponse(true);
    _advertising->setMinPreferred(0x06); // functions that help with iPhone connections issue
    _advertising->setMinPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("BLEManager::init() DONE");

    initialized = true;
}

void BLEManager::setupCharacteristics()
{
    characteristicsCallback = new SettingsCharacteristicCallbacks(_dataManager);

    BLECharacteristic *deviceIdCharacteristic = _infoService->createCharacteristic(
        DEVICE_ID_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ);
    deviceIdCharacteristic->setReadProperty(_deviceId);
    // deviceIdCharacteristic->addDescriptor();
    deviceIdCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *frameSizeCharacteristic = _settingsService->createCharacteristic(
        FRAME_SIZE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t frameSize = _dataManager.getFrameSize();
    frameSizeCharacteristic->setValue(frameSize);
    frameSizeCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *photoIntervalCharacteristic = _settingsService->createCharacteristic(
        PHOTO_INTERVAL_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t photoInterval = _dataManager.getPhotoInterval();
    photoIntervalCharacteristic->setValue(photoInterval);
    photoIntervalCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *specialEffectCharacteristic = _settingsService->createCharacteristic(
        SPECIAL_EFFECT_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t specialEffect = _dataManager.getFrameSize();
    specialEffectCharacteristic->setValue(specialEffect);
    specialEffectCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *whiteBalanceCharacteristic = _settingsService->createCharacteristic(
        WHITE_BALANCE_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t whiteBalance = _dataManager.getWhiteBalance();
    whiteBalanceCharacteristic->setValue(whiteBalance);
    whiteBalanceCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *qualityCharacteristic = _settingsService->createCharacteristic(
        QUALITY_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t quality = _dataManager.getQuality();
    qualityCharacteristic->setValue(quality);
    qualityCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *brightnessCharacteristic = _settingsService->createCharacteristic(
        BRIGHTNESS_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    int8_t brightness = _dataManager.getBrightness();
    brightnessCharacteristic->setValue(std::to_string(brightness));
    brightnessCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *contrastCharacteristic = _settingsService->createCharacteristic(
        CONTRAST_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t contrast = _dataManager.getContrast();
    contrastCharacteristic->setValue(contrast);
    contrastCharacteristic->setCallbacks(characteristicsCallback);
    
    BLECharacteristic *saturationCharacteristic = _settingsService->createCharacteristic(
        SATURATION_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t saturation = _dataManager.getSaturation();
    saturationCharacteristic->setValue(saturation);
    saturationCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *flashOnCharacteristic = _settingsService->createCharacteristic(
        FLASH_ON_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t flashOn = _dataManager.getFlashOn();
    flashOnCharacteristic->setValue(flashOn);
    flashOnCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *verticalFlipCharacteristic = _settingsService->createCharacteristic(
        VERTICAL_FLIP_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t verticalFlip = _dataManager.getVerticalFlip();
    verticalFlipCharacteristic->setValue(verticalFlip);
    verticalFlipCharacteristic->setCallbacks(characteristicsCallback);

    BLECharacteristic *horizontalMirrorCharacteristic = _settingsService->createCharacteristic(
        HORIZONTAL_MIRROR_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
    uint16_t horizontalMirror = _dataManager.getHorizontalMirror();
    horizontalMirrorCharacteristic->setValue(horizontalMirror);
    horizontalMirrorCharacteristic->setCallbacks(characteristicsCallback);
}

void SettingsServerCallbacks::onConnect(BLEServer *server)
{
    Serial.println("SettingsServerCallbacks::onConnect");
};

void SettingsServerCallbacks::onDisconnect(BLEServer *server)
{
    Serial.println("SettingsServerCallbacks::onDisconnect");
    BLEDevice::startAdvertising();
}

SettingsCharacteristicCallbacks::SettingsCharacteristicCallbacks(DataManager dataManager) 
{
    _dataManager = dataManager;
}

void SettingsCharacteristicCallbacks::onWrite(BLECharacteristic *characteristic)
{
    String uuid = characteristic->getUUID().toString().c_str();
    Serial.printf("SettingsCharacteristicCallbacks::onWrite -> BT UUID write received %s\n", uuid.c_str());

    std::string value = characteristic->getValue();
    Serial.printf("SettingsCharacteristicCallbacks::onWrite -> value= %s\n", value.c_str());

    if (uuid == FRAME_SIZE_CHARACTERISTIC_UUID)
    {
        uint16_t frameSize = strtol(value.c_str(), NULL, 10);
        Serial.printf("FRAME_SIZE_CHARACTERISTIC_UUID value converted: %d\n", frameSize);
        _dataManager.setFrameSize(frameSize);
    }
    else if (uuid == PHOTO_INTERVAL_CHARACTERISTIC_UUID)
    {
        uint16_t photoInterval = strtol(value.c_str(), NULL, 10);
        Serial.printf("PHOTO_INTERVAL_CHARACTERISTIC_UUID value converted: %d\n", photoInterval);
        _dataManager.setPhotoInterval(photoInterval);
    }
    else if (uuid == SPECIAL_EFFECT_CHARACTERISTIC_UUID)
    {
        uint16_t specialEffect = strtol(value.c_str(), NULL, 10);
        Serial.printf("SPECIAL_EFFECT_CHARACTERISTIC_UUID value converted: %d\n", specialEffect);
        _dataManager.setSpecialEffect(specialEffect);
    }
    else if (uuid == WHITE_BALANCE_CHARACTERISTIC_UUID)
    {
        uint16_t whiteBalance = strtol(value.c_str(), NULL, 10);
        Serial.printf("WHITE_BALANCE_CHARACTERISTIC_UUID value converted: %d\n", whiteBalance);
        _dataManager.setWhiteBalance(whiteBalance);
    }
    else if (uuid == QUALITY_CHARACTERISTIC_UUID)
    {
        uint16_t quality = strtol(value.c_str(), NULL, 10);
        Serial.printf("QUALITY_CHARACTERISTIC_UUID value converted: %d\n", quality);
        _dataManager.setQuality(quality);
    }
    else if (uuid == BRIGHTNESS_CHARACTERISTIC_UUID)
    {
        int8_t brightness = strtol(value.c_str(), NULL, 10);
        Serial.printf("BRIGHTNESS_CHARACTERISTIC_UUID value converted: %d\n", brightness);
        _dataManager.setBrightness(brightness);
    }
    else if (uuid == CONTRAST_CHARACTERISTIC_UUID)
    {
        int8_t contrast = strtol(value.c_str(), NULL, 10);
        Serial.printf("CONTRAST_CHARACTERISTIC_UUID value converted: %d\n", contrast);
        _dataManager.setContrast(contrast);
    }
    else if (uuid == SATURATION_CHARACTERISTIC_UUID)
    {
        int8_t saturation = strtol(value.c_str(), NULL, 10);
        Serial.printf("SATURATION_CHARACTERISTIC_UUID value converted: %d\n", saturation);
        _dataManager.setSaturation(saturation);
    }
    else if (uuid == FLASH_ON_CHARACTERISTIC_UUID)
    {
        uint16_t flashOn = strtol(value.c_str(), NULL, 10);
        Serial.printf("FLASH_ON_CHARACTERISTIC_UUID value converted: %d\n", flashOn);
        _dataManager.setFlashOn(flashOn);
    }
    else if (uuid == VERTICAL_FLIP_CHARACTERISTIC_UUID)
    {
        uint16_t verticalFlip = strtol(value.c_str(), NULL, 10);
        Serial.printf("VERTICAL_FLIP_CHARACTERISTIC_UUID value converted: %d\n", verticalFlip);
        _dataManager.setVerticalFlip(verticalFlip);
    }
    else if (uuid == HORIZONTAL_MIRROR_CHARACTERISTIC_UUID)
    {
        uint16_t horizontalMirror = strtol(value.c_str(), NULL, 10);
        Serial.printf("HORIZONTAL_MIRROR_CHARACTERISTIC_UUID value converted: %d\n", horizontalMirror);
        _dataManager.setHorizontalMirror(horizontalMirror);
    }
    else
    {
        Serial.printf("Error: Unknown characteristic: %s\n", uuid);
    }
}

void SettingsCharacteristicCallbacks::onRead(BLECharacteristic *characteristic)
{
    String uuid = characteristic->getUUID().toString().c_str();
    Serial.printf("SettingsCharacteristicCallbacks::onRead -> BT UUID write received %s\n", uuid.c_str());

    int16_t data = 0;

    if (uuid == FRAME_SIZE_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getFrameSize();
    }
    else if (uuid == PHOTO_INTERVAL_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getPhotoInterval();
    }
    else if (uuid == SPECIAL_EFFECT_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getSpecialEffect();
    }
    else if (uuid == WHITE_BALANCE_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getWhiteBalance();
    }
    else if (uuid == QUALITY_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getQuality();
    }
    else if (uuid == BRIGHTNESS_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getBrightness();
    }
    else if (uuid == CONTRAST_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getContrast();
    }
    else if (uuid == SATURATION_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getSaturation();
    }
    else if (uuid == FLASH_ON_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getFlashOn();
    }
    else if (uuid == VERTICAL_FLIP_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getVerticalFlip();
    }
    else if (uuid == HORIZONTAL_MIRROR_CHARACTERISTIC_UUID)
    {
        data = _dataManager.getHorizontalMirror();
    }
    else
    {
        Serial.printf("Error: Unknown characteristic: %s\n", uuid);
    }

    std::string a = std::to_string(data);
    
    Serial.printf("SettingsCharacteristicCallbacks::onRead -> setting %s\n", a.c_str());
    characteristic->setValue(a);
}
