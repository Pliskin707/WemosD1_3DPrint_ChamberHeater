#ifndef __TEMP_SENSOR_DHT22_HPP__
#define __TEMP_SENSOR_DHT22_HPP__

#include "DHTesp.h"
#include "temp_sensor_interface.hpp"

namespace pliskin
{

class TempDht : public DHTesp, public TempSensorInterface
{
    public:
        using TempSensorInterface::TempSensorInterface;

        void setup (void) override;
        void loop (void) override;
        bool isConnected (void) override;
};

};

#endif