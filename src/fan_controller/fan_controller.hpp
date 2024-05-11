#ifndef __FAN_CONTROLLER_HPP__
#define __FAN_CONTROLLER_HPP__

#include <Arduino.h>
#include "mavg.hpp"

#define FAN_CONTROL_PIN (RX)
// #define FAN_STATUS_PIN  (D6)
#define FAN_STATUS_PIN  (D4)

namespace pliskin
{

class FanController
{
    private:
        uint8_t _controlPercent = 100;
        uint16_t _statusRpm     = 0;
        movingAverage _avgRpm{5};

    public:
        void setup (void);
        void loop (void);
        void setPercent (uint8_t percent);
        uint16_t getSpeedRpm (void) const;
        void startMeasurement (void);
};

}
#endif