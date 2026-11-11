#pragma once
// CrowPanel 2.1" rotary pins, copied from Brady's working sketch
// (CrowPanel21_ST7701_WORKING_BASELINE). Cross-check against the Elecrow schematic in Stage 2.

// I2C bus and the PCF8574 I/O expander on it
#define PIN_I2C_SDA      38
#define PIN_I2C_SCL      39
#define PCF8574_ADDR     0x21
#define PCF_TOUCH_RESET  0
#define PCF_TOUCH_INT    2
#define PCF_LCD_POWER    3
#define PCF_LCD_RESET    4
#define PCF_ENCODER_SW   5

// Rotary encoder
#define PIN_ENCODER_A    42
#define PIN_ENCODER_B    4

// Display: backlight, ST7701 command bus (software SPI), RGB pixel bus
#define PIN_LCD_BL       6
#define PIN_LCD_CS       16
#define PIN_LCD_SCK      2
#define PIN_LCD_MOSI     1
#define PIN_LCD_DE       40
#define PIN_LCD_VSYNC    7
#define PIN_LCD_HSYNC    15
#define PIN_LCD_PCLK     41
#define PIN_LCD_R0 46
#define PIN_LCD_R1 3
#define PIN_LCD_R2 8
#define PIN_LCD_R3 18
#define PIN_LCD_R4 17
#define PIN_LCD_G0 14
#define PIN_LCD_G1 13
#define PIN_LCD_G2 12
#define PIN_LCD_G3 11
#define PIN_LCD_G4 10
#define PIN_LCD_G5 9
#define PIN_LCD_B0 5
#define PIN_LCD_B1 45
#define PIN_LCD_B2 48
#define PIN_LCD_B3 47
#define PIN_LCD_B4 21
