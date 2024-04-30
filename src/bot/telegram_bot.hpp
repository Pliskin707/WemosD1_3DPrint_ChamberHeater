#ifndef __TELEGRAM_BOT_HPP__
#define __TELEGRAM_BOT_HPP__

#include "AsyncTelegram2.h"

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>  
#define UPDATER ESPhttpUpdate


namespace pliskin
{

class TelegramBot
{
    private:
        WiFiClientSecure _client;
        Session _session;
        X509List _certificate = X509List(telegram_cert);
        AsyncTelegram2 _bot = AsyncTelegram2(_client);
        bool _initOk = false;
        uint_fast16_t _totalMsgsReceived = 0;

        void _handleText (TBMessage& msg);

    public:
        void setup (const char * token);
        void loop (void);
};

};

#endif