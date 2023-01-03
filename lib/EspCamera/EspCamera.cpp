#include "EspCamera.h"

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "esp_camera.h"

EspCamera::EspCamera() {}

bool EspCamera::init(uint8_t deviceId)
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
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.grab_mode = CAMERA_GRAB_LATEST;
    config.fb_count = 2;
    config.fb_location = CAMERA_FB_IN_PSRAM;

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK)
    {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return false;
    }

    sensor_t *s = esp_camera_sensor_get();

    s->set_framesize(s, FRAMESIZE_UXGA);         // FRAMESIZE_[QQVGA|HQVGA|QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA|QXGA(ov3660)]);
    s->set_quality(s, 10);                       // 10 to 63
    // s->set_brightness(s, 1);                  // -2 to 2
    // s->set_contrast(s, 0);                    // -2 to 2
    // s->set_saturation(s, 0);                  // -2 to 2
    s->set_special_effect(s, 0);                 // 0 to 6 (0 - No Effect, 1 - Negative, 2 - Grayscale, 3 - Red Tint, 4 - Green Tint, 5 - Blue Tint, 6 - Sepia)
    // s->set_whitebal(s, 1);                    // aka 'awb' in the UI; 0 = disable , 1 = enable
    // s->set_awb_gain(s, 1);                    // 0 = disable , 1 = enable
    s->set_wb_mode(s, 0);                        // 0 to 4 - if awb_gain enabled (0 - Auto, 1 - Sunny, 2 - Cloudy, 3 - Office, 4 - Home)
    s->set_exposure_ctrl(s, 1);                  // 0 = disable , 1 = enable
    s->set_aec2(s, 1);                           // 0 = disable , 1 = enable
    // s->set_ae_level(s, 0);                    // -2 to 2
    // s->set_aec_value(s, 300);                 // 0 to 1200
    s->set_gain_ctrl(s, 1);                      // 0 = disable , 1 = enable
    s->set_agc_gain(s, 15);                      // 0 to 30
    // s->set_gainceiling(s, (gainceiling_t)0);  // 0 to 6
    // s->set_bpc(s, 0);                         // 0 = disable , 1 = enable
    // s->set_wpc(s, 1);                         // 0 = disable , 1 = enable
    // s->set_raw_gma(s, 1);                     // 0 = disable , 1 = enable
    // s->set_lenc(s, 1);                        // 0 = disable , 1 = enable
    // s->set_hmirror(s, 0);                     // 0 = disable , 1 = enable
    // s->set_vflip(s, 0);                       // 0 = disable , 1 = enable
    // s->set_dcw(s, 1);                         // 0 = disable , 1 = enable
    // s->set_colorbar(s, 0);                    // 0 = disable , 1 = enable

    _isInitialized = true;

    return true;
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
    delay(7500);
}

bool EspCamera::getAndSendPhoto()
{
    Serial.println("\n-----------------------------------\nsendPhoto()");
    unsigned long tStart = millis();

    String getAll;
    String getBody;

    camera_fb_t *fb = NULL;
    fb = esp_camera_fb_get();
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
