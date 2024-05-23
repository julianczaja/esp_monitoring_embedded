#include "EspCamera.h"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

EspCamera::EspCamera() {}

bool EspCamera::init(uint8_t deviceId, const CameraConfiguration *cameraConfiguration)
{
    _deviceId = deviceId;

    _http.setReuse(false);
    _http.setTimeout(5000);

    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sccb_sda = SIOD_GPIO_NUM;
    config.pin_sccb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG; 
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_count = 1;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    config.frame_size = getFrameSizeByOrdinal(cameraConfiguration->frameSize);
    config.jpeg_quality = cameraConfiguration->quality;   

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return false;
    }

    sensor_t *sensor = esp_camera_sensor_get();

    if (sensor == nullptr) 
    {
        Serial.println("Error: sensor is null");
        return false;
    }

    updateConfiguration(sensor, *cameraConfiguration);


    _isInitialized = true;

    return true;
}

void EspCamera::updateConfiguration(sensor_t *sensor, const CameraConfiguration cameraConfiguration)
{
    Serial.println("\n> Camera::updateConfiguration");
    printCameraConfiguration(&cameraConfiguration);
    framesize_t frameSize = getFrameSizeByOrdinal(cameraConfiguration.frameSize);
    sensor->set_framesize(sensor, frameSize);                              // 0 to 22;
    sensor->set_special_effect(sensor, cameraConfiguration.specialEffect); // 0 to 6 (0-No Effect, 1-Negative, 2-Grayscale, 3-Red Tint, 4-Green Tint, 5-Blue Tint, 6-Sepia)
    sensor->set_wb_mode(sensor, cameraConfiguration.whiteBalance);         // 0 to 4 (if awb_gain enabled) (0-Auto, 1-Sunny, 2-Cloudy, 3-Office, 4-Home)
    sensor->set_quality(sensor, cameraConfiguration.quality);              // 10 to 63
    sensor->set_brightness(sensor, cameraConfiguration.brightness);        // -2 to 2
    sensor->set_contrast(sensor, cameraConfiguration.contrast);            // -2 to 2
    sensor->set_saturation(sensor, cameraConfiguration.saturation);        // -2 to 2
    sensor->set_vflip(sensor, cameraConfiguration.verticalFlip);           // 0=disable, 1=enable
    sensor->set_hmirror(sensor, cameraConfiguration.horizontalMirror);     // 0=disable, 1=enable

    sensor->set_whitebal(sensor, 1);                   // 0=disable, 1=enable
    sensor->set_awb_gain(sensor, 1);                   // 0=disable, 1=enable
    sensor->set_exposure_ctrl(sensor, 1);              // 0=disable, 1=enable
    sensor->set_aec2(sensor, 1);                       // 0=disable, 1=enable
    sensor->set_ae_level(sensor, 0);                   // -2 to 2
    sensor->set_aec_value(sensor, 300);                // 0 to 1200
    sensor->set_gain_ctrl(sensor, 1);                  // 0=disable, 1=enable
    sensor->set_agc_gain(sensor, 15);                  // 0 to 30
    sensor->set_gainceiling(sensor, (gainceiling_t)0); // 0 to 6
    sensor->set_bpc(sensor, 0);                        // 0=disable, 1=enable
    sensor->set_wpc(sensor, 1);                        // 0=disable, 1=enable
    sensor->set_raw_gma(sensor, 1);                    // 0=disable, 1=enable
    sensor->set_lenc(sensor, 1);                       // 0=disable, 1=enable
    sensor->set_dcw(sensor, 1);                        // 0=disable, 1=enable
    sensor->set_colorbar(sensor, 0);                   // 0=disable, 1=enable
}

bool EspCamera::deinit()
{
    esp_err_t err = esp_camera_deinit();
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return false;
    }
    return true;
}

void EspCamera::warmup()
{
    Serial.println("\n> Camera::warmup");
    delay(1000);

    camera_fb_t* frameBuffer = nullptr;
    for (int i = 0; i < 3; i++) 
    {
        frameBuffer = esp_camera_fb_get();
        esp_camera_fb_return(frameBuffer);
        frameBuffer = nullptr;
        delay(1000);
    }   
}

bool EspCamera::getAndSendPhoto(bool isFlashOn)
{
    Serial.println("\n-----------------------------------\nsendPhoto()");
    unsigned long tStart = millis();

    String getAll;
    String getBody;

    if (isFlashOn)
    {
        _ledFlash.on(100);
    }
    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
    if (isFlashOn)
    {
        _ledFlash.off();
    }
    if (!fb)
    {
        Serial.println("Camera capture failed");
        return false;
    }

    String path = serverName + serverPath + String(_deviceId);

    Serial.println("Connecting to server: " + path);

    if (_http.begin(_client, path))
    {
        Serial.println("Connection successful!");

        uint32_t imageLen = fb->len;
        uint8_t *fbBuf = fb->buf;
        size_t fbLen = fb->len;

        _http.addHeader("Content-Type", "image/jpeg");
        _http.addHeader("Content-Length", String(imageLen));

        int httpResponseCode = _http.POST(fbBuf, fbLen);

        esp_camera_fb_return(fb);

        if (httpResponseCode == HTTP_CODE_OK)
        {
            Serial.println("POST success");
        }
        else
        {
            Serial.println("POST failed - code: " + String(httpResponseCode));
            return false;
        }
    }
    else
    {
        getBody = "Connection to " + serverName + " failed.";
        Serial.println(getBody);
        return false;
    }

    Serial.print("sendPhoto() DONE IN ");
    Serial.print(millis() - tStart);
    Serial.println(" ms");

    return true;
}

void EspCamera::printCameraConfiguration(const CameraConfiguration *cameraConfiguration)
{
  printf("Camera Configuration:\n");
  printf("  -> Frame Size: %u\n", cameraConfiguration->frameSize);
  printf("  -> Special Effect: %u\n", cameraConfiguration->specialEffect);
  printf("  -> White Balance: %u\n", cameraConfiguration->whiteBalance);
  printf("  -> Quality: %u\n", cameraConfiguration->quality);
  printf("  -> Brightness: %d\n", cameraConfiguration->brightness);
  printf("  -> Contrast: %d\n", cameraConfiguration->contrast);
  printf("  -> Saturation: %d\n", cameraConfiguration->saturation);
  printf("  -> Flash On: %u\n", cameraConfiguration->flashOn);
  printf("  -> Vertical Flip: %u\n", cameraConfiguration->verticalFlip);
  printf("  -> Horizontal Mirror: %u\n", cameraConfiguration->horizontalMirror);
  printf("  -> Photo Interval: %u\n", cameraConfiguration->photoInterval);
}

framesize_t EspCamera::getFrameSizeByOrdinal(int ordinal)
{
    Serial.print("\n> EspCamera::getFrameSizeByOrdinal ");
    Serial.println(ordinal);
    if (ordinal >= FRAMESIZE_96X96 && ordinal <= FRAMESIZE_QSXGA)
    {
        return static_cast<framesize_t>(ordinal);
    }
    else
    {
        return FRAMESIZE_INVALID;
    }
}
