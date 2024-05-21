#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include "WiFiConfiguration.h"

#define EEPROM_SIZE                 128
#define WIFI_CONFIGURATION_ADDRESS  0
#define FAILURES_COUNT_ADDRESS      64
#define FRAME_SIZE_ADDRESS          66
#define PHOTO_INTERVAL_ADDRESS      68
#define SPECIAL_EFFECT_ADDRESS      70
#define WHITE_BALANCE_ADDRESS       72
#define QUALITY_ADDRESS             74
#define BRIGHTNESS_ADDRESS          76
#define CONTRAST_ADDRESS            78
#define SATURATION_ADDRESS          80
#define FLASH_ON_ADDRESS            82
#define VERTICAL_FLIP_ADDRESS       84
#define HORIZONTAL_MIRROR_ADDRESS   86

#define READ_WRITE_WAIT_TIME_MS     50

class DataManager
{
public:
    DataManager();

    void init();

    void setWiFiConfiguration(WiFiConfiguration *wifiConfiguration);
    void getWiFiConfiguration(WiFiConfiguration *wifiConfiguration);

    uint16_t getFrameSize();
    void setFrameSize(uint16_t frameSize);

    uint16_t getPhotoInterval();
    void setPhotoInterval(uint16_t photoInterval);

    uint16_t getSpecialEffect();
    void setSpecialEffect(uint16_t specialEffect);

    uint16_t getWhiteBalance();
    void setWhiteBalance(uint16_t whiteBalance);

    uint16_t getQuality();
    void setQuality(uint16_t quality);

    int8_t getBrightness();
    void setBrightness(int8_t brightness);

    int8_t getContrast();
    void setContrast(int8_t contrast);

    int8_t getSaturation();
    void setSaturation(int8_t saturation);

    uint16_t getFlashOn();
    void setFlashOn(uint16_t flashOn);

    uint16_t getVerticalFlip();
    void setVerticalFlip(uint16_t verticalFlip);

    uint16_t getHorizontalMirror();
    void setHorizontalMirror(uint16_t horizontalMirror);

    uint16_t getFailuresCount();
    void increaseFailuresCount();
    void resetFailuresCount();

    void clearAll();

private:
    bool _isInitialized = false;
};

#endif