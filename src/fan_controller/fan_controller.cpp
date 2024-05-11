#include "fan_controller.hpp"

namespace pliskin
{

static volatile uint_fast16_t _fan_flanks = 0;
static volatile uint32_t _debounce_until = 0;
IRAM_ATTR void _fan_meas_handler (void) 
{
    const auto millisNow = millis();

    if (millisNow > _debounce_until)
        _fan_flanks++;
    
    _debounce_until = millisNow + 4;
}

void FanController::setup(void)
{
    // setup PWM fan speed control
    pinMode(FAN_CONTROL_PIN, OUTPUT);
    analogWriteFreq(20000);
    analogWriteRange(100);
    analogWrite(FAN_CONTROL_PIN, _controlPercent);

    // setup rpm measurement
    pinMode(FAN_STATUS_PIN, INPUT);
    attachInterrupt(FAN_STATUS_PIN, &_fan_meas_handler, RISING);
}

void FanController::loop(void)
{
    // measure for half a second only, so the noise from SPI and/or USART transmissions doesn't get caught
    if (millis() > _next)
    {
        _next = millis() + 1000;
        _avgRpm.addValue(_fan_flanks * 60);
        _fan_flanks = 0;
    }
}

void FanController::setPercent (uint8_t percent)
{
    if (percent > 100)
        percent = 100;

    if (percent != _controlPercent)
    {
        _controlPercent = percent;
        analogWrite(FAN_CONTROL_PIN, percent);
    }
}

uint16_t FanController::getSpeedRpm (void) const
{
    return (uint16_t) _avgRpm.getAvgValue();
    return _statusRpm;
}

}