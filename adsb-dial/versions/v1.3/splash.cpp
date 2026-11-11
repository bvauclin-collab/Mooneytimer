#include "splash.h"
#include "display.h"
#include "version.h"

static void drawColorRing() {
  // Five 72-degree bands, 16 px thick, just inside the round edge.
  // Arduino_GFX angles: 0 = 3 o'clock, clockwise, so -90 is 12 o'clock.
  const uint16_t colors[5] = {COL_RED, COL_GREEN, COL_BLUE, COL_WHITE, COL_AMBER};
  for (int i = 0; i < 5; i++) {
    float start = -90.0f + i * 72.0f;
    gfx->fillArc(SCREEN_CX, SCREEN_CY, 232, 216, start, start + 72.0f, colors[i]);
  }
}

void splashDraw() {
  gfx->fillScreen(COL_BLACK);
  drawColorRing();

  displayCenterText(FW_NAME, 118, 4, COL_WHITE);       // 32 px tall

  char ver[16];
  snprintf(ver, sizeof(ver), "v%s", FW_VERSION);
  displayCenterText(ver, 166, 3, COL_LIGHTBLUE);       // 24 px tall

  // Advisory box
  gfx->fillRoundRect(70, 225, 340, 130, 10, COL_AMBER_BG);
  gfx->drawRoundRect(70, 225, 340, 130, 10, COL_AMBER);
  gfx->drawRoundRect(71, 226, 338, 128, 9, COL_AMBER);
  displayCenterText("ADVISORY ONLY", 247, 3, COL_AMBER_TXT);
  displayCenterText("Not a certified instrument.", 292, 2, COL_CREAM);
  displayCenterText("Always see and avoid.", 320, 2, COL_CREAM);

  displayCenterText("Red Green Blue White Amber", 392, 2, COL_GRAY);
}
