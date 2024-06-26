#include <Arduino.h>
#include <WiFi.h>
#include "esp_camera.h"
#include "WiFiConfiguration.h"
#include "CameraConfiguration.h"
#include "Led.h"
#include "DataManager.h"
#include "EspCamera.h"
#include "BLEManager.h"

#define DEVICE_ID 1
#define WIFI_CONNECTION_TIMEOUT 10e3 // ms
#define SLEEP_TIME_DEFAULT 3e8       // us (5min)
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
unsigned long getSleepTimeFromPhotoInterval(uint16_t photoInterval);

DataManager dataManager = DataManager();
Led ledBuiltin = Led(33);

void setup()
{
  Serial.begin(115200);
  Serial.println("Hello world");
  Serial.printf("Total heap: %d\n", ESP.getHeapSize());
  Serial.printf("Free heap: %d\n", ESP.getFreeHeap());
  Serial.printf("Total PSRAM: %d\n", ESP.getPsramSize());
  Serial.printf("Free PSRAM: %d\n", ESP.getFreePsram());

  dataManager.init();

  if (dataManager.getIsFirstRun())
  {
    Serial.println("First run detected!");
    dataManager.clearAll();
    dataManager.setDeviceId(DEVICE_ID);
    WiFiConfiguration wifiConfiguration = WiFiConfiguration();
    strcpy(wifiConfiguration.ssid, "ssid");
    strcpy(wifiConfiguration.password, "password");
    dataManager.setWiFiConfiguration(&wifiConfiguration);
    CameraConfiguration cameraConfiguration = {
        .frameSize = 9,
        .photoInterval = 4,
        .specialEffect = 0,
        .whiteBalance = 0,
        .quality = 10,
        .brightness = 0,
        .contrast = 0,
        .saturation = 0,
        .flashOn = 0,
        .verticalFlip = 0,
        .horizontalMirror = 0
    };
    dataManager.setCameraConfiguration(&cameraConfiguration);
    dataManager.setIsFirstRun(false);
  }

  delay(100);

  Mode mode;
  //  mode = CAMERA;
  //  mode = CONFIGURATION;
  if (digitalRead(GPIO_NUM_16))
  {
    mode = CAMERA;
  }
  else
  {
    mode = CONFIGURATION;
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
  Serial.println("setupConfigurationMode");
  BLEManager bleManager = BLEManager(DEVICE_ID, dataManager);
  bleManager.init();
}

void setupCameraMode()
{
  Serial.println("setupCameraMode");
  EspCamera camera;

  CameraConfiguration cameraConfiguration = CameraConfiguration();
  dataManager.getCameraConfiguration(&cameraConfiguration);

  String serverUrl = dataManager.getServerUrl();

  if (camera.init(DEVICE_ID, &cameraConfiguration, serverUrl) == false)
  {
    dataManager.increaseFailuresCount();
    onError((char *)"Can't init camera");
  }

  WiFiConfiguration wifiConfiguration = WiFiConfiguration();
  dataManager.getWiFiConfiguration(&wifiConfiguration);
  connectToWifi(&wifiConfiguration);

  camera.warmup();

  for (int i = 0; i < MAX_TRIES; i++)
  {
    bool isFlashOn = (bool) cameraConfiguration.flashOn;
    bool ret = camera.getAndSendPhoto(isFlashOn);
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

  goToSleep(getSleepTimeFromPhotoInterval(cameraConfiguration.photoInterval));
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

unsigned long getSleepTimeFromPhotoInterval(uint16_t photoInterval)
{
  switch (photoInterval)
  {
    case 0: return 5e6;     // FIVE_SECONDS
    case 1: return 30e6;    // HALF_MINUTE
    case 2: return 60e6;    // ONE_MINUTE
    case 3: return 120e6;   // TWO_MINUTES
    case 4: return 300e6;   // FIVE_MINUTES
    case 5: return 600e6;   // TEN_MINUTES
    case 6: return 1800e6;  // HALF_HOUR
    case 7: return 3600e6;  // ONE_HOUR
    default: return SLEEP_TIME_DEFAULT;
  }
}
