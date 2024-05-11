#include "fan_controller.hpp"

namespace pliskin
{

static volatile uint_fast16_t _fan_flanks = 0;
static volatile uint32_t _fan_meas_start_time = 0;
IRAM_ATTR void _fan_meas_handler (void) 
{
    _fan_flanks++;
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
    if (((millis() - _fan_meas_start_time) >= 500) && _fan_meas_start_time)
    {
        _fan_meas_start_time = 0;
        _avgRpm.addValue(_fan_flanks); // TODO *60;
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

void FanController::startMeasurement (void)
{
    _fan_flanks = 0;
    _fan_meas_start_time = millis();
}

}