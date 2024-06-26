#include <Arduino.h>
#include <array>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "ota/ota.hpp"
#include "projutils/projutils.hpp"
#include "config.hpp"

#include "sensors/temp_sensor_dht22.hpp"
#include "sensors/temp_sensor_DS18B20.hpp"
#include "bot/telegram_bot.hpp"
#include "display/tft_display.hpp"
#include "fan_controller/fan_controller.hpp"

using namespace pliskin;

/** Wifi authentication **
 * 
 * this file needs to be created with the following content (and is obviously not included in version control):
 * 
 * #pragma once
 * 
 * const char* ssid = "<YourSSIDhere>";
 * const char* password = "<YourPasswordHere>";
 * #define TELEGRAM_HEATER_BOT_TOKEN "<YourAccessTokenHere>"
 * #define TELEGRAM_HEATER_BOT_CHAT 12345678
 */
#include "../../../../../../wifiauth2.h"

static bool mDNS_init_ok = false;
WiFiClient client;

static const std::array<TempSensorInterface*, 2> tempSensors = 
{
  new TempDht(D1),
  new TempDs18b20(D2)
};
static TelegramBot bot;
static tftDisplay disp;
static FanController fan;

void setup() {
  #ifndef DEBUG_PRINT
  // pinMode(LEDPIN, OUTPUT);
  #else
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  #endif

  disp.setup();

  for (auto& tempSensor : tempSensors)
    tempSensor->setup();

  fan.setup();
  fan.setPercent(10);
  disp.setFanController(&fan);

  // Wifi
  WiFi.hostname(DEVICENAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid2, password);
  // wifi_set_sleep_type(NONE_SLEEP_T);

  wl_status_t wstat;
  while (true)
  {
    delay(500);
    wstat = WiFi.status();
    if (wstat == WL_CONNECTED)
      break;

    #ifndef DEBUG_PRINT
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
    #endif
    dprintf("Connecting (%d) ...\n", wstat);
  };

  // mDNS
  mDNS_init_ok = MDNS.begin(DEVICENAME);

  // OTA
  ota::begin(DEVICENAME);

  bot.setup(TELEGRAM_HEATER_BOT_TOKEN);
}

void loop() {
  const uint32_t time = millis();
  static uint32_t next = 0, startFanMeasAt = 0;

  // Wifi status
  const bool connected = WiFi.isConnected();
  #ifndef DEBUG_PRINT
  digitalWrite(LEDPIN, !connected);
  #endif

  // mDNS
  if (mDNS_init_ok)
    MDNS.update();

  // OTA
  ota::handle();

  // program logic
  if (time >= next)
  {
    next = time + 1000;
    dprintf("\nSystime: %lu ms; WLAN: %sconnected (as %s)\n", time, (connected ? "":"dis"), WiFi.localIP().toString().c_str());

    uint_fast8_t sensor_ndx = 0;
    float temps[tempSensors.size()];
    for (auto& tempSensor : tempSensors)
    {
      dprintf("Sensor %u is %sconnected\n", sensor_ndx, (tempSensor->isConnected() ? "": "not "));
      temps[sensor_ndx] = tempSensor->getTemperature();
      tempSensor->debugPrint();
      sensor_ndx++;
    }

    dprintf("Fan: %d rpm\n", fan.getSpeedRpm());

    if (tempSensors.size() >= 2)
      disp.setTemps(temps[0], temps[1]);

    if (connected)
    {
        // TODO
    }
    else
      WiFi.reconnect();
  }

  for (auto& tempSensor : tempSensors)
    tempSensor->loop();

  fan.loop();

  bot.loop();
  disp.loop();

  yield();
}