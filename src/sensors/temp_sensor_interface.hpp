#ifndef __TEMP_SENSOR_INTERFACE_HPP__
#define __TEMP_SENSOR_INTERFACE_HPP__

#include <stdint.h>
#include <Arduino.h>

namespace pliskin
{

class TempSensorInterface
{
    private:
        uint32_t _lastRead = 0;

    protected:
        float _tempMin  = 80.0f;
        float _tempMax  = -50.0f;
        float _tempLast = 0.0f;
        const uint8_t _pin;

        void _updateTemp (const float updatedTemp)
        {
            _tempLast = updatedTemp;
            _tempMin  = std::min(_tempMin, _tempLast);
            _tempMax  = std::max(_tempMax, _tempLast);

            _lastRead = millis();
        }

    public:
        TempSensorInterface(const uint8_t pin) : _pin(pin) {};

        virtual void setup (void) = 0;
        virtual void loop (void) = 0;
        virtual bool isConnected (void) = 0;

        float getTemperature (void) const {return _tempLast;}
        uint32_t getDataAge (void) const {return millis() - _lastRead;}
        void debugPrint (void)
        {
            const div_t
                now = div((int) (100.0f * _tempLast), 100),
                min = div((int) (100.0f * _tempMin), 100),
                max = div((int) (100.0f * _tempMax), 100);

            Serial.printf_P(PSTR("Temp: %d.%02d C (min: %d.%02d, max: %d.%02d)\n"), now.quot, now.rem, min.quot, min.rem, max.quot, max.rem);
        }
};

}

#endif