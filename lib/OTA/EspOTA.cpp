#include "EspOta.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>

EspOTA::EspOTA() {}

void otaOnStart()
{
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
    else
        type = "filesystem";

    Serial.println("Start updating " + type);
}

void otaOnEnd()
{
    Serial.println("\nEnd");
}

void otaOnProgress(unsigned int progress, unsigned int total)
{
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
}

void otaOnError(ota_error_t error)
{
    Serial.printf("OTA Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
        Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
        Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
        Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
        Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
        Serial.println("End Failed");
}

void EspOTA::init()
{
    ArduinoOTA
        .onStart(otaOnStart)
        .onEnd(otaOnEnd)
        .onProgress(otaOnProgress)
        .onError(otaOnError);

    ArduinoOTA.begin();
}

void EspOTA::waitForUpdate(int timeMs, Led led)
{
    unsigned long *startConnection = (unsigned long *)malloc(sizeof(unsigned long));
    *startConnection = millis();

    while (true)
    {
        if ((millis() - *startConnection) > timeMs)
        {
            break;
        }
        ArduinoOTA.handle();
        led.dim(250);
    }
    free(startConnection);
}