#include "temp_sensor_dht22.hpp"

namespace pliskin
{
void TempDht::setup (void)
{
    DHTesp::setup(_pin, DHTesp::DHT22);
}

void TempDht::loop (void)
{
    if (getDataAge() > 200uL)
    {
        const float 
            tempNow = DHTesp::getTemperature();

        if (!isnanf(tempNow))
            _updateTemp(tempNow);
    }
}

bool TempDht::isConnected (void)
{
    return getStatus() == DHTesp::ERROR_NONE;
}
}