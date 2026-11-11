#pragma once
#include <Arduino_GFX_Library.h>

// Round 480x480 ST7701 panel. Init sequence and timings match Brady's working sketch.
#define SCREEN_W 480
#define SCREEN_H 480
#define SCREEN_CX 240
#define SCREEN_CY 240

extern Arduino_RGB_Display *gfx;

// Powers the panel through the PCF8574, resets it, starts the RGB bus and turns the
// backlight on. Returns false (and says why on Serial) if any step fails.
bool displayBegin();

// Draws text centered on x = SCREEN_CX, with its top at y. Built-in 6x8 font scaled by size.
void displayCenterText(const char *text, int y, uint8_t size, uint16_t color);

// RGB565 colors (closest match to the approved mockup)
#define COL_BLACK      0x0000
#define COL_WHITE      0xFFFF
#define COL_RED        0xF800
#define COL_GREEN      0x07E0
#define COL_BLUE       0x001F
#define COL_AMBER      0xECE4
#define COL_AMBER_BG   0x4120
#define COL_AMBER_TXT  0xFE2E
#define COL_CREAM      0xFF7B
#define COL_LIGHTBLUE  0x85BD
#define COL_GRAY       0x8C30
