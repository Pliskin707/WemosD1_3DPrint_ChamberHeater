#ifndef __TFT_DISPLAY_HPP__
#define __TFT_DISPLAY_HPP__

#include <Adafruit_ST7735.h>
#include <SPI.h>

#define TFT_CABLE_SELECT    (2)     // "D4"
#define TFT_DATA_REG        (16)    // "D0"
#define TFT_RESET           (0)     // "D3"

namespace pliskin
{

class tftDisplay : public Adafruit_ST7735
{
    private:
        uint32_t _next = 0;
        uint16_t _prevColor = ST7735_CYAN;
        int16_t _tempPosX = 0;
        float _temps[2] = {0.0f};

    public:
        tftDisplay() : Adafruit_ST7735(TFT_CABLE_SELECT, TFT_DATA_REG, NOT_A_PIN) {}

        void setup (void);
        void loop (void);
        void setTemps (const float temp1, const float temp2);
        void clear (void);
};

};

#endif