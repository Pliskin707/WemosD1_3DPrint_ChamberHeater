#ifndef __TFT_DISPLAY_HPP__
#define __TFT_DISPLAY_HPP__

#include <Adafruit_ST7735.h>
#include <SPI.h>
#include "fan_controller/fan_controller.hpp"

#define TFT_CABLE_SELECT    (NOT_A_PIN)
#define TFT_DATA_REG        (D0)

namespace pliskin
{

class tftDisplay
{
    private:
        uint32_t _next      = 0;
        int16_t _tempPosX   = 0;
        int16_t _fanPosX    = 0;
        uint8_t _fontHeight = 0;
        bool _updated       = false;
        float _temps[2]     = {0.0f};

        Adafruit_ST7735 _controller{TFT_CABLE_SELECT, TFT_DATA_REG, NOT_A_PIN};

        // these pointers are used to read status data when updating the screen content
        const FanController * _fan = nullptr;

    public:
        tftDisplay() {}

        void setup (void);
        void setFanController (const FanController * const fan);
        void loop (void);
        bool frameUpdated (void) const {return _updated;}

        void setTemps (const float temp1, const float temp2);
        void clear (void);
};

};

#endif