#ifndef __TFT_DISPLAY_HPP__
#define __TFT_DISPLAY_HPP__

#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CABLE_SELECT    (2)     // "D4"
#define TFT_DATA_REG        (16)    // "D0"

namespace pliskin
{

class tftDisplay
{
    private:
        uint32_t _next = 0;
        uint16_t _prevColor = ST7735_CYAN;
        int16_t _tempPosX = 0;
        uint8_t _fontHeight = 0;
        float _temps[2] = {0.0f};

        Adafruit_ST7735 _controller{TFT_CABLE_SELECT, TFT_DATA_REG, NOT_A_PIN};

    public:
        tftDisplay() {}

        void setup (void);
        void loop (void);
        void setTemps (const float temp1, const float temp2);
        void clear (void);
};

};

#endif