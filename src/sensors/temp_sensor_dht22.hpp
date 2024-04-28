#ifndef __TEMP_SENSOR_DHT22_HPP__
#define __TEMP_SENSOR_DHT22_HPP__

#include "DHTesp.h"

namespace pliskin
{

class temp_dht : public DHTesp
{
    private:
        float _temp_min  = 80.0f;
        float _temp_max  = -50.0f;
        float _temp_last = 0.0f;

    public:
        using DHTesp::DHT22;

        void debug_print (void)
        {
            const div_t
                now = div((int) (100.0f * _temp_last), 100),
                min = div((int) (100.0f * _temp_min), 100),
                max = div((int) (100.0f * _temp_max), 100);

            Serial.printf_P(PSTR("Temp: %d.%02d C (min: %d.%02d, max: %d.%02d)\n"), now.quot, now.rem, min.quot, min.rem, max.quot, max.rem);
        }

        float getTemperature (void)
        {
            const float 
                temp_now = DHTesp::getTemperature();

            if (!isnanf(_temp_last))
            {
                _temp_last = temp_now;
                _temp_min  = std::min(_temp_min, _temp_last);
                _temp_max  = std::max(_temp_max, _temp_last);
            }

            return _temp_last;
        }
};

};

#endif