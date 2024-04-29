#ifndef __TEMP_SENSOR_DS18B20_HPP__
#define __TEMP_SENSOR_DS18B20_HPP__

#include <OneWire.h>
#include <DallasTemperature.h>
#include "temp_sensor_interface.hpp"

namespace pliskin
{

class temp_ds18b20 : public TempSensorInterface
{
    private:
        OneWire * _wire = nullptr;   // pin D2
        DallasTemperature * _dallas = nullptr;
        uint32_t _lastRead = 0;
        float _lastTemp = 0.0f;
        DeviceAddress _address;
        bool _addrFound = false;

    public:
        void setup (const uint8_t pin) override
        {
            if (_wire)
                return;

            _wire = new OneWire(pin);
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

        void loop (void) override
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

        bool isConnected (void) override
        {
            if (!_addrFound)
                return false;

            return _dallas->isConnected(_address);
        }
};

};

#endif