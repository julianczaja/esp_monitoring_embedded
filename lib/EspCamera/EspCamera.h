#ifndef ESP_CAMERA_H
#define ESP_CAMERA_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "esp_camera.h"
#include "CameraConfiguration.h"
#include "Led.h"

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27

#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

class EspCamera
{
public:
    EspCamera();

    bool init(uint8_t deviceId, const CameraConfiguration *cameraConfiguration, String serverUrl);
    bool deinit();
    void warmup();
    bool getAndSendPhoto(bool isFlashOn);

private:
    const String uploadPhotoEndpoint = "photo/";

    void updateConfiguration(sensor_t *sensor, const CameraConfiguration cameraConfiguration);
    void printCameraConfiguration(const CameraConfiguration *cameraConfiguration);
    framesize_t getFrameSizeByOrdinal(int ordinal);

    bool _isInitialized = false;
    uint8_t _deviceId;
    String _serverUrl;
    HTTPClient _http;
    WiFiClient _client;
    Led _ledFlash = Led(4);
};

#endif