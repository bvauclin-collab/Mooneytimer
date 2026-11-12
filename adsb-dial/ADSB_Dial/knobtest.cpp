#include "knobtest.h"
#include <math.h>
#include "display.h"
#include "input.h"
#include "version.h"

#define TICKS        24
#define TICK_R_IN    196
#define TICK_R_OUT   220
#define MARK_R       208
#define MARK_SIZE    16
#define HOME_SIZE    12
#define COL_TICK     0x5AEB   // gray (#5F5E5A)
#define COL_BOX_BG   0x016A   // dark blue (#042C53)
#define COL_BOX_EDGE 0x345B   // blue (#378ADD)
#define COL_BOX_TXT  0xB6BE   // light blue (#B5D4F4)

static int count = 0;
static int presses = 0;
static int markerIndex = 0;
static unsigned long flashUntil = 0;
static bool flashing = false;

static float tickAngle(int i) { return (i * 360.0f / TICKS - 90.0f) * (float)M_PI / 180.0f; }

static void pointAt(int i, int r, int &x, int &y) {
  float a = tickAngle(i);
  x = SCREEN_CX + (int)lroundf(r * cosf(a));
  y = SCREEN_CY + (int)lroundf(r * sinf(a));
}

// 4 px wide tick drawn as two triangles.
static void drawTick(int i, uint16_t color) {
  float a = tickAngle(i);
  float c = cosf(a), s = sinf(a);
  float px = -s * 2.0f, py = c * 2.0f;  // half-width perpendicular
  float x0 = SCREEN_CX + TICK_R_IN * c, y0 = SCREEN_CY + TICK_R_IN * s;
  float x1 = SCREEN_CX + TICK_R_OUT * c, y1 = SCREEN_CY + TICK_R_OUT * s;
  gfx->fillTriangle(x0 + px, y0 + py, x0 - px, y0 - py, x1 + px, y1 + py, color);
  gfx->fillTriangle(x0 - px, y0 - py, x1 - px, y1 - py, x1 + px, y1 + py, color);
}

static void drawTickOrHome(int i) {
  if (i == 0) {
    int x, y;
    pointAt(0, MARK_R, x, y);
    gfx->fillCircle(x, y, HOME_SIZE, COL_TICK);
  } else {
    drawTick(i, COL_TICK);
  }
}

static void drawMarker(int i, uint16_t color) {
  int x, y;
  pointAt(i, MARK_R, x, y);
  gfx->fillCircle(x, y, MARK_SIZE, color);
}

static void moveMarker(int newIndex) {
  drawMarker(markerIndex, COL_BLACK);       // erase old marker
  drawTickOrHome(markerIndex);              // restore what was under it
  markerIndex = newIndex;
  drawMarker(markerIndex, COL_AMBER);
}

static void drawCount() {
  gfx->fillRect(60, 196, 360, 72, COL_BLACK);
  char buf[12];
  snprintf(buf, sizeof(buf), "%+d", count);
  displayCenterText(buf, 200, 8, COL_WHITE);  // 64 px tall
}

static void drawPressBox(bool flash) {
  uint16_t bg = flash ? COL_BOX_TXT : COL_BOX_BG;
  uint16_t fg = flash ? COL_BOX_BG : COL_BOX_TXT;
  gfx->fillRoundRect(130, 315, 220, 56, 8, bg);
  gfx->drawRoundRect(130, 315, 220, 56, 8, COL_BOX_EDGE);
  gfx->drawRoundRect(131, 316, 218, 54, 7, COL_BOX_EDGE);
  char buf[20];
  snprintf(buf, sizeof(buf), "PRESSES: %d", presses);
  displayCenterText(buf, 332, 3, fg);
}

void knobTestBegin() {
  gfx->fillScreen(COL_BLACK);
  for (int i = 0; i < TICKS; i++) drawTickOrHome(i);
  markerIndex = 0;
  drawMarker(0, COL_AMBER);
  char title[24];
  snprintf(title, sizeof(title), "KNOB TEST v%s", FW_VERSION);
  displayCenterText(title, 130, 3, COL_GRAY);
  drawCount();
  drawPressBox(false);
  inputTakeSteps();  // ignore anything turned during the splash
  Serial.println("Knob test ready: turn the knob, press it");
}

void knobTestUpdate() {
  int s = inputTakeSteps();
  if (s != 0) {
    count += s;
    int idx = ((count % TICKS) + TICKS) % TICKS;
    moveMarker(idx);
    drawCount();
    Serial.printf("knob %+d  count %+d  marker %d\n", s, count, idx);
  }

  if (inputTakePress()) {
    presses++;
    flashing = true;
    flashUntil = millis() + 150;
    drawPressBox(true);
    Serial.printf("press #%d\n", presses);
  }
  if (flashing && (long)(millis() - flashUntil) >= 0) {
    flashing = false;
    drawPressBox(false);
  }
}
