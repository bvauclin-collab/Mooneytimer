// ADS-B Dial: standalone 1090 MHz traffic display on the Elecrow CrowPanel 2.1" rotary.
// ADVISORY ONLY. Not a certified instrument. Never a substitute for see-and-avoid.
//
// v1.2: screen on. Startup splash with version, advisory notice and a color test ring.

#include <Arduino.h>
#include "version.h"
#include "display.h"
#include "splash.h"

static bool displayOk = false;

static void printBanner() {
  Serial.println();
  Serial.println("========================================");
  Serial.printf("%s  firmware v%s\n", FW_NAME, FW_VERSION);
  Serial.println("ADVISORY ONLY - not a certified instrument.");
  Serial.println("Never a substitute for see-and-avoid.");
  Serial.println("========================================");
  Serial.printf("Chip: %s rev %d, %d cores, %lu MHz\n", ESP.getChipModel(),
                ESP.getChipRevision(), ESP.getChipCores(),
                (unsigned long)ESP.getCpuFreqMHz());
  Serial.printf("Flash: %lu MB   PSRAM: %lu KB\n",
                (unsigned long)(ESP.getFlashChipSize() / (1024 * 1024)),
                (unsigned long)(ESP.getPsramSize() / 1024));
}

void setup() {
  Serial.begin(115200);
  // Native USB: give the PC a moment to reopen the port after reset.
  unsigned long t0 = millis();
  while (!Serial && millis() - t0 < 3000) delay(10);
  printBanner();

  displayOk = displayBegin();
  if (displayOk) {
    splashDraw();
    Serial.println("Splash drawn");
  }
}

void loop() {
  static unsigned long last = 0;
  if (millis() - last >= 2000) {
    last = millis();
    Serial.printf("v%s alive, uptime %lu s, display %s\n", FW_VERSION,
                  millis() / 1000, displayOk ? "OK" : "FAILED");
  }
}
