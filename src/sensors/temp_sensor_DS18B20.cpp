#include "temp_sensor_DS18B20.hpp"

namespace pliskin
{
void TempDs18b20::setup (void)
{
    if (_wire)
        return;

    _wire = new OneWire(_pin);
    if (!_wire)
        return;

    _dallas = new DallasTemperature(_wire);
    if (!_dallas)
        return;

    _dallas->begin();
    _addrFound = _dallas->getAddress(_address, 0);
    if (_addrFound)
    {
        Serial.print(F("1-Wire addr: 0x"));
        for (uint_fast8_t ii = 0; ii < sizeof(_address); ii++)
            Serial.print(_address[ii], HEX);
        Serial.println();
    }
    else
        Serial.println(F("1-Wire address search failed"));

    _dallas->setWaitForConversion(false);
}

void TempDs18b20::loop (void)
{
    if ((getDataAge() >= 2000) && _addrFound)
    {
        const auto& tempC = _dallas->getTempCByIndex(0);
        if (tempC != DEVICE_DISCONNECTED_C)
            _updateTemp(tempC);

        // _dallas->requestTemperaturesByAddress(_address);
        _dallas->requestTemperatures();
    }
}

bool TempDs18b20::isConnected (void)
{
    if (!_addrFound)
        return false;

    return _dallas->isConnected(_address);
}
}