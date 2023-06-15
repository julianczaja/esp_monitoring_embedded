#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "WiFiConfiguration.h"
#include "Led.h"
#include "DataManager.h"
#include "EspCamera.h"
#include "EspOTA.h"
#include "BLEManager.h"

#define DEVICE_ID 1
#define WIFI_CONNECTION_TIMEOUT 10e3 // ms
#define SLEEP_TIME_DEFAULT 6e8       // us (10min)
#define MAX_TRIES 3

enum Mode
{
  CONFIGURATION = 0,
  CAMERA = 1
};


void setupConfigurationMode();
void setupCameraMode();
void onError(char *message);
void goToSleep(unsigned long sleepTime);
void connectToWifi(WiFiConfiguration *wifiConfiguration);

DataManager dataManager = DataManager();
Led ledBuiltin = Led(33);

void setup()
{
  Serial.begin(115200);
  dataManager.init();
  delay(1000);

  Mode mode;
  if (digitalRead(GPIO_NUM_16))
  {
    mode = CONFIGURATION;
  }
  else
  {
    mode = CAMERA;
  }

  if (mode == CONFIGURATION)
  {
    setupConfigurationMode();
  }
  else if (mode == CAMERA)
  {
    setupCameraMode();
  }
}

void loop() {}

void setupConfigurationMode()
{
  EspOTA ota;
  ota.init();
  ota.waitForUpdate(5000, ledBuiltin);

  BLEManager bleManager = BLEManager(dataManager);
  bleManager.init();
}

void setupCameraMode()
{
  EspCamera camera;

  if (camera.init(DEVICE_ID) == false)
  {
    dataManager.increaseFailuresCount();
  }

  WiFiConfiguration wifiConfiguration = WiFiConfiguration();
  dataManager.getWiFiConfiguration(&wifiConfiguration);
  connectToWifi(&wifiConfiguration);

  camera.warmup();

  for (int i = 0; i < MAX_TRIES; i++)
  {
    bool ret = camera.getAndSendPhoto();
    Serial.print("getAndSendPhoto ret = ");
    Serial.println(ret);
    if (ret)
    {
      break;
    }
    delay(2000);
  }

  for (int i = 0; i < MAX_TRIES; i++)
  {
    bool ret = camera.deinit();
    Serial.print("deinit ret = ");
    Serial.println(ret);
    if (ret)
    {
      break;
    }
    delay(2000);
  }

  goToSleep(SLEEP_TIME_DEFAULT);
}

void connectToWifi(WiFiConfiguration *wifiConfiguration)
{
  Serial.println("Connecting to Wi-Fi...");

  unsigned long *startConnection = (unsigned long *)malloc(sizeof(unsigned long));
  *startConnection = millis();

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiConfiguration->ssid, wifiConfiguration->password);

  while (WiFi.status() != WL_CONNECTED)
  {
    if ((millis() - *startConnection) > WIFI_CONNECTION_TIMEOUT)
    {
      onError((char *)"Can't connect to Wi-Fi");
    }
    Serial.print(".");
    ledBuiltin.on(30);
    delay(100);
    ledBuiltin.off();
  }
  free(startConnection);

  Serial.print("\nConnected with IP: ");
  Serial.println(WiFi.localIP());
}

void onError(char *message)
{
  Serial.print("Error! Message: ");
  Serial.println(message);
  dataManager.increaseFailuresCount();
  goToSleep(SLEEP_TIME_DEFAULT);
}

void goToSleep(unsigned long sleepTimeMicros)
{
  Serial.print("\nGoing to sleep for ");
  Serial.print(sleepTimeMicros / 60e6);
  Serial.println(" minutes!");
  Serial.flush();
  Serial.end();
  ledBuiltin.off();

  ESP.deepSleep(sleepTimeMicros);
}
