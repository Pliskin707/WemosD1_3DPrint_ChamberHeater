#include "tft_display.hpp"

namespace pliskin
{

void tftDisplay::setup (void)
{
    begin(40000000);
    initR(INITR_18BLACKTAB);
    clear();
    setRotation(2);
    setTextColor(color565(0x7F, 0x7F, 0xFF), ST7735_WHITE);

    for (uint_fast8_t ndx = 0; ndx < (sizeof(_temps) / sizeof(_temps[0])); ndx++)
    {
        setCursor(4, ndx * 8 + 4);
        printf_P(PSTR("Temp %u: "), ndx + 1);
        _tempPosX = getCursorX();
    }
}

void tftDisplay::loop (void)
{
    if (millis() > _next)
    {
        _next = millis() + 1000;
        
        for (uint_fast8_t ndx = 0; ndx < (sizeof(_temps) / sizeof(_temps[0])); ndx++)
        {
            setCursor(_tempPosX, ndx * 8 + 4);
            const div_t qr = div((int) (100.0f * _temps[ndx]), 100);
            printf_P(PSTR("%d.%02d C"), qr.quot, qr.rem);
        }
    }
}

void tftDisplay::setTemps (const float temp1, const float temp2)
{
    _temps[0] = temp1;
    _temps[1] = temp2;
}

void tftDisplay::clear (void)
{
    fillScreen(ST7735_WHITE);
}

}