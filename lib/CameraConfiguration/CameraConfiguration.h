#ifndef CAMERA_CONFIGURATION_H
#define CAMERA_CONFIGURATION_H

#include <Arduino.h>

typedef struct
{
    uint16_t frameSize;
    uint16_t photoInterval;
    uint16_t specialEffect;
    uint16_t whiteBalance;
    uint16_t quality;
    int8_t brightness;
    int8_t contrast;
    int8_t saturation;
    uint16_t flashOn;
    uint16_t verticalFlip;
    uint16_t horizontalMirror;
} CameraConfiguration;

#endif