#ifndef __TEMP_SENSOR_DS18B20_HPP__
#define __TEMP_SENSOR_DS18B20_HPP__

#include <OneWire.h>
#include <DallasTemperature.h>
#include "temp_sensor_interface.hpp"

namespace pliskin
{

class TempDs18b20 : public TempSensorInterface
{
    private:
        OneWire * _wire = nullptr;   // pin D2
        DallasTemperature * _dallas = nullptr;
        DeviceAddress _address;
        bool _addrFound = false;

    public:
        using TempSensorInterface::TempSensorInterface;

        void setup (void) override;
        void loop (void) override;
        bool isConnected (void) override;
};

};

#endif