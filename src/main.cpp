#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "ota/ota.hpp"
#include "projutils/projutils.hpp"
#include "config.hpp"

#include "sensors/temp_sensor_dht22.hpp"
#include "bot/telegram_bot.hpp"

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
static temp_dht sensor_temp_dht;
static telegram_bot bot;

void setup() {
  #ifndef DEBUG_PRINT
  pinMode(LEDPIN, OUTPUT);
  #else
  Serial.begin(115200);
  #endif

  sensor_temp_dht.setup(5, DHTesp::DHT22); // that is pin "D1" on a Wemos D1 (next to the "RX" pin)

  // Wifi
  WiFi.hostname(DEVICENAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  wifi_set_sleep_type(NONE_SLEEP_T);

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

  bot.setup();
}

void loop() {
  const uint32_t time = millis();
  static uint32_t next = 0;

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
    dprintf("Systime: %lu ms; WLAN: %sconnected (as %s)\n", time, (connected ? "":"dis"), WiFi.localIP().toString().c_str());
    sensor_temp_dht.getTemperature();
    sensor_temp_dht.debug_print();
    Serial.println(sensor_temp_dht.getStatusString());

    if (connected)
    {
        // TODO
    }
    else
      WiFi.reconnect();
  }

  bot.loop();

  yield();
}