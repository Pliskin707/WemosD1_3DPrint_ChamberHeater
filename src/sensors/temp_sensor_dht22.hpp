#ifndef __TEMP_SENSOR_DHT22_HPP__
#define __TEMP_SENSOR_DHT22_HPP__

#include "DHTesp.h"
#include "temp_sensor_interface.hpp"

namespace pliskin
{

class temp_dht : public DHTesp, public TempSensorInterface
{
    public:
        using TempSensorInterface::TempSensorInterface;

        void setup (void) override
        {
            DHTesp::setup(_pin, DHTesp::DHT22);
        }

        void loop (void) override
        {
            if (getDataAge() > 200uL)
            {
                const float 
                    tempNow = DHTesp::getTemperature();

                if (!isnanf(tempNow))
                    _updateTemp(tempNow);
            }
        }

        bool isConnected (void) override
        {
            return getStatus() == DHTesp::ERROR_NONE;
        }
};

};

#endif