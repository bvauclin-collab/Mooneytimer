# ADS-B Rotary Dial: Version Log

One entry per version. Brady reports each result before work moves on.

| Version | Date | What changed | Test result |
|---|---|---|---|
| 1.1 | 2026-11-11 | New ADSB_Dial sketch replaces old firmware; serial hello + version | WORKED |

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

<!-- Entry template
## vX.Y — YYYY-MM-DD
**Changed:** ...
**Test checklist:**
- [ ] ...
**Result:** WORKED / DID NOT WORK — notes
-->
