#include "telegram_bot.hpp"

#define TBOT_COMMAND_PING   "/ping"

namespace pliskin
{


void TelegramBot::setup (const char * token)
{
    // Sync time with NTP, to check properly Telegram certificate
    configTime("CET-1CEST,M3.5.0,M10.5.0/3"  /* Berlin (see https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv) */, "time.google.com", "time.windows.com", "pool.ntp.org");
    // Set ceritficate, session and some other base client properies
    _client.setSession(&_session);
    _client.setTrustAnchors(&_certificate);
    _client.setBufferSizes(1024, 1024);

    _bot.setUpdateTime(2000);
    _bot.setTelegramToken(token);
    _bot.setFormattingStyle(AsyncTelegram2::FormatStyle::MARKDOWN);


    if (_bot.begin())
    {
        _initOk = true;
        // _bot.sendTo(TELEGRAM_HEATER_BOT_CHAT, "Heater Bot online");

        // setup commands
        _bot.deleteMyCommands();
        _bot.setMyCommands(TBOT_COMMAND_PING, "Responds with \"Pong\" if the bot is online");
    }
    else
        Serial.print(F("Telegram bot failed\n"));
}

void TelegramBot::loop (void)
{
    if (!_initOk)
        return;

    TBMessage msg;
    bool noMsgReceived = true;

    for (uint_fast8_t max_msgs = 10; max_msgs; max_msgs--)
    {
        if (!_bot.getNewMessage(msg))
            break;

        noMsgReceived = false;
        _totalMsgsReceived++;

        switch (msg.messageType)
        {
            case MessageType::MessageText: _handleText(msg); break;
            default: break; // ignore
        }
    }

    if (!noMsgReceived)
        Serial.printf_P(PSTR("Total telegram messages received: %u\n"), _totalMsgsReceived);
}

void TelegramBot::_handleText (TBMessage& msg)
{
    if (msg.text.equals(F(TBOT_COMMAND_PING)))
    {
        msg.disable_notification = true;
        _bot.sendMessage(msg, "Pong");
    }
    else
        _bot.sendMessage(msg, String("I don't understand: " + msg.text).c_str());
}

}