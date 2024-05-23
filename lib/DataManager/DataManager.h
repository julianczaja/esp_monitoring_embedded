#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>

#include "WiFiConfiguration.h"
#include "CameraConfiguration.h"

#define EEPROM_SIZE                 128
#define FIRST_RUN_FLAG_ADDRESS      0
#define WIFI_CONFIGURATION_ADDRESS  1
#define FAILURES_COUNT_ADDRESS      65
#define FRAME_SIZE_ADDRESS          67
#define PHOTO_INTERVAL_ADDRESS      69
#define SPECIAL_EFFECT_ADDRESS      71
#define WHITE_BALANCE_ADDRESS       73
#define QUALITY_ADDRESS             75
#define BRIGHTNESS_ADDRESS          77
#define CONTRAST_ADDRESS            79
#define SATURATION_ADDRESS          81
#define FLASH_ON_ADDRESS            83
#define VERTICAL_FLIP_ADDRESS       85
#define HORIZONTAL_MIRROR_ADDRESS   87

#define READ_WRITE_WAIT_TIME_MS     30

class DataManager
{
public:
    DataManager();

    void init();

    bool getIsFirstRun();
    void setIsFirstRun(bool isFirstRun);

    void setWiFiConfiguration(WiFiConfiguration *wifiConfiguration);
    void getWiFiConfiguration(WiFiConfiguration *wifiConfiguration);

    void getCameraConfiguration(CameraConfiguration *cameraConfiguration);
    void setCameraConfiguration(const CameraConfiguration *cameraConfiguration);

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