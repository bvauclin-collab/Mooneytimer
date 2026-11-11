# ADS-B Rotary Dial: Version Log

One entry per version. Brady reports each result before work moves on.

| Version | Date | What changed | Test result |
|---|---|---|---|
| 1.1 | 2026-11-11 | New ADSB_Dial sketch replaces old firmware; serial hello + version | WORKED |
| 1.2 | 2026-11-11 | Screen on: startup splash with version, advisory notice, color test ring | PARTLY: red and blue swapped |

## v1.1 — 2026-11-11
**Changed:** First ADSB_Dial sketch (`ADSB_Dial.ino` + `version.h`). Serial only: prints the
version, the advisory notice and chip info, then an "alive" line every 2 s. The screen is not
driven yet (it stays dark). Claude flashed it over COM7 from the command line and the serial
read back OK.
**Test checklist:**
- [ ] In the IDE, File > Open `adsb-dial/ADSB_Dial/ADSB_Dial.ino`, then close the old sketch's window.
- [ ] Select port COM7, open Serial Monitor at 115200, press RESET on the board.
- [ ] See "ADS-B Dial firmware v1.1" and the ADVISORY ONLY lines, then "v1.1 alive" every 2 s.
- [ ] The old program no longer runs (a dark screen is expected in v1.1).
**Result:** WORKED (2026-11-11). Brady sees "v1.1 alive" every 2 s in the Serial Monitor.
Opening COM7 resets the board (rst:0x15 USB_UART_CHIP_RESET); that is expected, not a crash.

## v1.2 — 2026-11-11
**Changed:** Screen turned on. New modules: `board_pins.h` (pins from Brady's working sketch),
`display.h/.cpp` (PCF8574 power and reset, ST7701 init, backlight), `splash.h/.cpp` (startup
screen, mockup Option B: name, version, ADVISORY ONLY box, color ring red/green/blue/white/amber
clockwise from the top). Serial "alive" line now also says whether the display is OK.
**Test checklist:**
- [ ] Close Serial Monitor, press Upload in the IDE (or ask Claude to flash).
- [ ] Screen lights up and shows "ADS-B Dial", "v1.2" and the amber ADVISORY ONLY box.
- [ ] Ring colors, clockwise from the top: red, green, blue, white, amber. Report any that are wrong.
- [ ] The ring sits evenly inside the round edge (not cut off on one side).
- [ ] Serial Monitor shows "DISPLAY OK" and "v1.2 alive ... display OK".
**Result:** PARTLY WORKED (2026-11-11). Uploaded from the IDE. Screen, text, layout and ring
position all good. Ring colors from 12 o'clock came out royal blue, green, red, white, light blue
instead of red, green, blue, white, amber: red and blue are swapped (amber turns light blue for
the same reason). Fixed in v1.3.

<!-- Entry template
## vX.Y — YYYY-MM-DD
**Changed:** ...
**Test checklist:**
- [ ] ...
**Result:** WORKED / DID NOT WORK — notes
-->
