#include "platform_lcd.h"

void platform_lcd_present_rgb565(const uint16_t *framebuffer,
                                 int width,
                                 int height,
                                 int stride)
{
    (void)framebuffer;
    (void)width;
    (void)height;
    (void)stride;
    // TODO: Implement LCD transfer for ESP32 display driver.
}

void platform_lcd_init(void)
{
    // TODO: Initialize QSPI LCD controller and DMA buffers.
}
