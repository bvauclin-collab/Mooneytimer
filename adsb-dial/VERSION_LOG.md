# ADS-B Rotary Dial: Version Log

One entry per version. Brady reports each result before work moves on.

| Version | Date | What changed | Test result |
|---|---|---|---|
| 1.1 | 2026-11-11 | New ADSB_Dial sketch replaces old firmware; serial hello + version | WORKED |
| 1.2 | 2026-11-11 | Screen on: startup splash with version, advisory notice, color test ring | PARTLY: red and blue swapped |
| 1.3 | 2026-11-11 | Fix red/blue swap (R and B pin groups swapped in display setup) | WORKED |
| 1.4 | 2026-11-11 | Knob and press test screen (mockup Option B) after a 3 s splash | WORKED |

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

## v1.3 — 2026-11-11
**Changed:** `display.cpp` passes the blue pins where the red pins were and vice versa, so
colors come out right. Nothing else changed. Compiles clean.
**Test checklist:**
- [ ] Close Serial Monitor, press Upload in the IDE.
- [ ] Startup screen shows "v1.3".
- [ ] Ring from 12 o'clock, clockwise: red, green, blue, white, amber.
- [ ] ADVISORY ONLY box is dark amber/brown with amber text (not blue).
**Result:** WORKED (2026-11-11). Brady confirms the colors are correct.

## v1.4 — 2026-11-11
**Changed:** New `input.h/.cpp` (knob on GPIO 42/4 by interrupt, 2 transitions per detent,
clockwise positive; press on PCF8574 bit 5, debounced 30 ms) and `knobtest.h/.cpp` (24-tick
ring, amber marker, signed count, press box that flashes). Splash shows 3 s, then the test
screen. Each click and press is printed to Serial; the "alive" line is now every 10 s.
**Test checklist:**
- [ ] Close Serial Monitor, press Upload. Splash shows "v1.4" for about 3 s, then "KNOB TEST v1.4".
- [ ] Turn clockwise one click: marker moves one tick clockwise, count goes to +1.
- [ ] Counter-clockwise: marker moves back, count goes down (negative past zero).
- [ ] Turn 24 clicks one way: marker lands back on the gray 12 o'clock dot, count +24 or -24.
- [ ] Turn fast: no skipped or doubled clicks (count matches clicks felt).
- [ ] Press the knob: PRESSES goes up by exactly 1 and the box flashes. Holding it counts once.
- [ ] Serial Monitor shows a "knob" line per click and a "press #" line per press.
**Result:** WORKED (2026-11-11). Stage 1 (hello world, display, knob and press) complete.

<!-- Entry template
## vX.Y — YYYY-MM-DD
**Changed:** ...
**Test checklist:**
- [ ] ...
**Result:** WORKED / DID NOT WORK — notes
-->
