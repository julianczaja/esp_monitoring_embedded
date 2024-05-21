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
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.put(WIFI_CONFIGURATION_ADDRESS, *wifiConfiguration);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

void DataManager::getWiFiConfiguration(WiFiConfiguration *wifiConfiguration)
{
    Serial.println("\n> DataManager::getWiFiConfiguration");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.get(WIFI_CONFIGURATION_ADDRESS, *wifiConfiguration);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getFrameSize()
{
    Serial.println("\n> DataManager::getFrameSize");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t frameSize = EEPROM.readUShort(FRAME_SIZE_ADDRESS);
    Serial.printf("\n> read frameSize: %d\n", frameSize);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return frameSize;
}

void DataManager::setFrameSize(uint16_t frameSize)
{
    Serial.println("\n> DataManager::setFrameSize");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write frameSize: %d\n", frameSize);
    EEPROM.writeUShort(FRAME_SIZE_ADDRESS, frameSize);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getPhotoInterval()
{
    Serial.println("\n> DataManager::getPhotoInterval");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t photoInterval = EEPROM.readUShort(PHOTO_INTERVAL_ADDRESS);
    Serial.printf("\n> read photo interval: %d\n", photoInterval);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return photoInterval;
}

void DataManager::setPhotoInterval(uint16_t photoInterval)
{
    Serial.println("\n> DataManager::setPhotoInterval");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write photo interval: %d\n", photoInterval);
    EEPROM.writeUShort(PHOTO_INTERVAL_ADDRESS, photoInterval);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getSpecialEffect()
{
    Serial.println("\n> DataManager::getSpecialEffect");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t specialEffect = EEPROM.readUShort(SPECIAL_EFFECT_ADDRESS);
    Serial.printf("\n> read special effect: %d\n", specialEffect);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return specialEffect;
}

void DataManager::setSpecialEffect(uint16_t specialEffect)
{
    Serial.println("\n> DataManager::setSpecialEffect");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write special effect: %d\n", specialEffect);
    EEPROM.writeUShort(SPECIAL_EFFECT_ADDRESS, specialEffect);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getWhiteBalance()
{
    Serial.println("\n> DataManager::getWhiteBalance");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t whiteBalance = EEPROM.readUShort(WHITE_BALANCE_ADDRESS);
    Serial.printf("\n> read white balance: %d\n", whiteBalance);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return whiteBalance;
}

void DataManager::setWhiteBalance(uint16_t whiteBalance)
{
    Serial.println("\n> DataManager::setWhiteBalance");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write white balance: %d\n", whiteBalance);
    EEPROM.writeUShort(WHITE_BALANCE_ADDRESS, whiteBalance);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getQuality()
{
    Serial.println("\n> DataManager::getQuality");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t quality = EEPROM.readUShort(QUALITY_ADDRESS);
    Serial.printf("\n> read quality: %d\n", quality);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return quality;
}

void DataManager::setQuality(uint16_t quality)
{
    Serial.println("\n> DataManager::setQuality");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.writeUShort(QUALITY_ADDRESS, quality);
    Serial.printf("\n> set quality: %d\n", quality);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

int8_t DataManager::getBrightness()
{
    Serial.println("\n> DataManager::getBrightness");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    int8_t brightness = EEPROM.readChar(BRIGHTNESS_ADDRESS);
    Serial.printf("\n> read brightness: %d\n", brightness);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return brightness;
}

void DataManager::setBrightness(int8_t brightness)
{
    Serial.println("\n> DataManager::setBrightness");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write brightness: %d\n", brightness);
    EEPROM.writeChar(BRIGHTNESS_ADDRESS, brightness);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}
int8_t DataManager::getContrast()
{
    Serial.println("\n> DataManager::getContrast");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    int8_t contrast = EEPROM.readChar(CONTRAST_ADDRESS);
    Serial.printf("\n> read contrast: %d\n", contrast);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return contrast;
}

void DataManager::setContrast(int8_t contrast)
{
    Serial.println("\n> DataManager::setContrast");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write contrast: %d\n", contrast);
    EEPROM.writeChar(CONTRAST_ADDRESS, contrast);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

int8_t DataManager::getSaturation()
{
    Serial.println("\n> DataManager::getSaturation");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    int8_t saturation = EEPROM.readChar(SATURATION_ADDRESS);
    Serial.printf("\n> read saturation: %d\n", saturation);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return saturation;
}

void DataManager::setSaturation(int8_t saturation)
{
    Serial.println("\n> DataManager::setSaturation");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write saturation: %d\n", saturation);
    EEPROM.writeChar(SATURATION_ADDRESS, saturation);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getFlashOn()
{
    Serial.println("\n> DataManager::getFlashOn");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t flashOn = EEPROM.readUShort(FLASH_ON_ADDRESS);
    Serial.printf("\n> read flash on: %d\n", flashOn);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return flashOn;
}

void DataManager::setFlashOn(uint16_t flashOn)
{
    Serial.println("\n> DataManager::setFlashOn");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write flash on: %d\n", flashOn);
    EEPROM.writeUShort(FLASH_ON_ADDRESS, flashOn);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getVerticalFlip()
{
    Serial.println("\n> DataManager::getVerticalFlip");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t verticalFlip = EEPROM.readUShort(VERTICAL_FLIP_ADDRESS);
    Serial.printf("\n> read vertical flip: %d\n", verticalFlip);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return verticalFlip;
}

void DataManager::setVerticalFlip(uint16_t verticalFlip)
{
    Serial.println("\n> DataManager::setVerticalFlip");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write vertical flip: %d\n", verticalFlip);
    EEPROM.writeUShort(VERTICAL_FLIP_ADDRESS, verticalFlip);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getHorizontalMirror()
{
    Serial.println("\n> DataManager::getHorizontalMirror");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t horizontalMirror = EEPROM.readUShort(HORIZONTAL_MIRROR_ADDRESS);
    Serial.printf("\n> read horizontal mirror: %d\n", horizontalMirror);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return horizontalMirror;
}

void DataManager::setHorizontalMirror(uint16_t horizontalMirror)
{
    Serial.println("\n> DataManager::setHorizontalMirror");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    Serial.printf("\n> write horizontal mirror: %d\n", horizontalMirror);
    EEPROM.writeUShort(HORIZONTAL_MIRROR_ADDRESS, horizontalMirror);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

uint16_t DataManager::getFailuresCount()
{
    Serial.println("\n> DataManager::getFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t failuresCount = EEPROM.readUShort(FAILURES_COUNT_ADDRESS);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();

    return failuresCount;
}

void DataManager::increaseFailuresCount()
{
    Serial.println("\n> DataManager::increaseFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    uint16_t failuresCount = EEPROM.readUShort(FAILURES_COUNT_ADDRESS);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.writeUShort(FAILURES_COUNT_ADDRESS, failuresCount + 1);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

void DataManager::resetFailuresCount()
{
    Serial.println("\n> DataManager::resetFailuresCount");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.writeUShort(FAILURES_COUNT_ADDRESS, (uint16_t)0);
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}

void DataManager::clearAll()
{
    Serial.println("\n> DataManager::clearAll");

    EEPROM.begin(EEPROM_SIZE);
    delay(READ_WRITE_WAIT_TIME_MS);
    for (int i = 0; i < EEPROM_SIZE; ++i)
    {
        EEPROM.write(i, 0);
    }
    delay(READ_WRITE_WAIT_TIME_MS);
    EEPROM.end();
}
