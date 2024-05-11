#include "tft_display.hpp"
#include "Fonts/FreeSans9pt7b.h"

namespace pliskin
{

void tftDisplay::setup (void)
{
    _controller.initR(INITR_18BLACKTAB);
    _controller.setSPISpeed(32000000);
    clear();
    _controller.setRotation(2);
    _controller.setTextColor(_controller.color565(0x7F, 0x7F, 0xFF), ST7735_WHITE);
    _controller.setFont(&FreeSans9pt7b);
    _fontHeight = 14;

    for (uint_fast8_t ndx = 0; ndx < (sizeof(_temps) / sizeof(_temps[0])); ndx++)
    {
        _controller.setCursor(4, (ndx + 1) * _fontHeight + 4);
        _controller.printf_P(PSTR("T%u: "), ndx + 1);
        _tempPosX = _controller.getCursorX();
    }
}

void tftDisplay::loop (void)
{
    if ((millis() > _next))
    {
        _next = millis() + 1000;
        GFXcanvas1 canv(_controller.width() - _tempPosX, _fontHeight);
        canv.setTextColor(ST7735_WHITE);
        canv.setFont(&FreeSans9pt7b);
        // canv.setTextColor(_controller.color565(0x7F, 0x7F, 0x00));
        
        for (uint_fast8_t ndx = 0; ndx < (sizeof(_temps) / sizeof(_temps[0])); ndx++)
        {
            // TODO use canvas so the old content gets cleared
            // _controller.setCursor(_tempPosX, (ndx + 1) * _fontHeight + 4);
            canv.fillScreen(ST7735_BLACK);
            canv.setCursor(0, canv.height() - 1);
            const div_t qr = div((int) (100.0f * _temps[ndx]), 100);
            // _controller.printf_P(PSTR("%d.%02d °C"), qr.quot, qr.rem);
            canv.printf_P(PSTR("%d.%02d °C"), qr.quot, qr.rem);
            _controller.drawBitmap(_tempPosX, ndx * _fontHeight + 4, canv.getBuffer(), canv.width(), canv.height(), _controller.color565(0x7F, 0x7F, 0x00), ST7735_MAGENTA);
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
    _controller.fillScreen(ST7735_WHITE);
}

}