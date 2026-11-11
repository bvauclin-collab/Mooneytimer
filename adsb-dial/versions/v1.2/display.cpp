#include "display.h"
#include <Wire.h>
#include "board_pins.h"

static uint8_t pcfState = 0xFF;

static bool pcfWrite(uint8_t value) {
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(value);
  uint8_t err = Wire.endTransmission();
  if (err != 0) {
    Serial.printf("PCF8574 write failed, I2C error %u\n", err);
    return false;
  }
  pcfState = value;
  return true;
}

static bool pcfSetBit(uint8_t bit, bool high) {
  uint8_t v = high ? (pcfState | (1 << bit)) : (pcfState & ~(1 << bit));
  return pcfWrite(v);
}

static Arduino_DataBus *st7701Bus = new Arduino_SWSPI(
    GFX_NOT_DEFINED, PIN_LCD_CS, PIN_LCD_SCK, PIN_LCD_MOSI, GFX_NOT_DEFINED);

static Arduino_ESP32RGBPanel *rgbPanel = new Arduino_ESP32RGBPanel(
    PIN_LCD_DE, PIN_LCD_VSYNC, PIN_LCD_HSYNC, PIN_LCD_PCLK,
    PIN_LCD_R0, PIN_LCD_R1, PIN_LCD_R2, PIN_LCD_R3, PIN_LCD_R4,
    PIN_LCD_G0, PIN_LCD_G1, PIN_LCD_G2, PIN_LCD_G3, PIN_LCD_G4, PIN_LCD_G5,
    PIN_LCD_B0, PIN_LCD_B1, PIN_LCD_B2, PIN_LCD_B3, PIN_LCD_B4,
    1, 20, 10, 10,           // HSYNC polarity, front porch, pulse width, back porch
    1, 8, 10, 10,            // VSYNC polarity, front porch, pulse width, back porch
    1, 18000000UL, false,    // PCLK active edge, 18 MHz, no DE idle high
    0, 0,
    4800);                   // bounce buffer (pixels)

Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    SCREEN_W, SCREEN_H, rgbPanel, 0, true, st7701Bus, GFX_NOT_DEFINED,
    st7701_type5_init_operations, sizeof(st7701_type5_init_operations));

bool displayBegin() {
  Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
  Wire.setClock(400000);
  delay(50);

  if (!pcfWrite(0xFF)) {
    Serial.println("PCF8574 not found: display cannot be powered");
    return false;
  }
  Serial.println("PCF8574 OK");

  pcfSetBit(PCF_LCD_POWER, true);
  delay(100);
  pcfSetBit(PCF_LCD_RESET, false);
  delay(100);
  pcfSetBit(PCF_LCD_RESET, true);
  delay(120);

  pinMode(PIN_LCD_BL, OUTPUT);
  digitalWrite(PIN_LCD_BL, LOW);

  Serial.println("Starting ST7701...");
  if (!gfx->begin()) {
    Serial.println("DISPLAY INIT FAILED");
    return false;
  }
  gfx->fillScreen(COL_BLACK);
  digitalWrite(PIN_LCD_BL, HIGH);
  Serial.println("DISPLAY OK, backlight on");
  return true;
}

void displayCenterText(const char *text, int y, uint8_t size, uint16_t color) {
  gfx->setTextSize(size);
  gfx->setTextColor(color);
  int w = (int)strlen(text) * 6 * size;
  gfx->setCursor(SCREEN_CX - w / 2, y);
  gfx->print(text);
}
