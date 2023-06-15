#ifndef ESP_OTA_H
#define ESP_OTA_H

#include <ArduinoOTA.h>
#include "Led.h"

class EspOTA
{
public:
    EspOTA();

    void init();
    void waitForUpdate(int timeMs, Led led);
};

#endif