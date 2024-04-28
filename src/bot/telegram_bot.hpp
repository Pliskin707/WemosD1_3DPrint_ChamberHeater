#ifndef __TELEGRAM_BOT_HPP__
#define __TELEGRAM_BOT_HPP__

#include "AsyncTelegram2.h"
#include "../../../../../../wifiauth2.h"    // see the main to learn how this works

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>  
#define UPDATER ESPhttpUpdate


namespace pliskin
{

class telegram_bot
{
    private:
        WiFiClientSecure _client;
        Session _session;
        X509List _certificate = X509List(telegram_cert);
        AsyncTelegram2 _bot = AsyncTelegram2(_client);
        bool _init_ok = false;

    public:
        void setup (void)
        {
            // Sync time with NTP, to check properly Telegram certificate
            configTime("CET-1CEST,M3.5.0,M10.5.0/3"  /* Berlin (see https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) */, "time.google.com", "time.windows.com", "pool.ntp.org");
            // Set ceritficate, session and some other base client properies
            _client.setSession(&_session);
            _client.setTrustAnchors(&_certificate);
            _client.setBufferSizes(1024, 1024);

            _bot.setUpdateTime(2000);
            _bot.setTelegramToken(TELEGRAM_HEATER_BOT_TOKEN);

            if (_bot.begin())
            {
                _init_ok = true;
                // _bot.sendTo(TELEGRAM_HEATER_BOT_CHAT, "Heater Bot online");
            }
            else
                Serial.print(F("Telegram bot failed\n"));
        }

        void loop (void)
        {
            if (!_init_ok)
                return;

            TBMessage msg;

            if (_bot.getNewMessage(msg))
                Serial.print(F("Telegram msg received\n"));
        }
};

};

#endif