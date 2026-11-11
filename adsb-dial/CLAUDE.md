# Project: ADS-B Receiver with Display (Elecrow CrowPanel 2.1" ESP32-S3 Rotary Dial)

## Goal
A standalone, portable ADS-B receiver built around the Elecrow CrowPanel 2.1" HMI
ESP32 Rotary Display. It receives 1090 MHz ADS-B traffic itself (no Stratux, Stratus,
Sentry, or phone needed), decodes it on the ESP32-S3, and shows nearby aircraft on its
own round screen as a compass-rose traffic display. FIS-B weather is a later phase
(see "Weather" below). Optional later phase: broadcast GDL90 over WiFi so an iPad
running ForeFlight can use it as a traffic source.

## About the user
- Brady, a pilot (Mooney M20C). He does NOT write code. Claude writes all code and walks
  him through every hardware and toolchain step, assuming no embedded experience.
- He pastes error text back when something breaks. Warn him in advance about steps that
  commonly go wrong.
- Mac is the main computer (a PC is also available).

## How to work (non-negotiable)
1. Build in small, testable stages. Stop after each stage and let him test before moving on.
2. Propose UI/layout options before writing UI code.
3. Never guess pins, voltages, or connector types. Verify against the Elecrow schematic,
   wiki, or datasheets, and tell him what you could not verify.
4. Before any wiring step, tell him exactly what to connect, in plain words, and what
   voltage each pin must see. Warn about anything that can damage a part.
5. Keep code modular: radio capture, decoder, GPS, UI, and (later) networking are separate
   files so each can be tested alone.
6. State uncertainty plainly. Do not present unproven feasibility as settled.

## Workflow (agreed with Brady)
- Priority is speed. Claude Code writes all the code. No Codex unless Claude hits a roadblock
  or Brady decides he prefers its workflow.
- Toolchain: Arduino IDE on Brady's Windows PC (he has built many projects in it). Add
  arduino-cli alongside it so Claude Code can compile; it shares the IDE's boards and libraries.
- ONE working sketch, `ADSB_Dial/ADSB_Dial.ino` (plus its module files), stays open in the
  Arduino IDE. Claude edits those files in place on disk; the IDE picks up the change and Brady
  just presses Upload. Never make him open a new sketch or a second IDE window per version.
  Brady should not type in the IDE while Claude is editing, or the two copies will conflict.
- Versions are numbered by stage and edit: 1.1, 1.2, 1.3, then 2.1 and so on. The version
  number lives in the code (a `FW_VERSION` constant, shown on the startup screen and printed to
  serial) so Brady can confirm which version is on the device.
- Saved copies of each version: commit and tag it `vX.Y` in git, and copy the sketch to
  `versions/vX.Y/` as a backup to roll back to. These copies are never opened in the IDE.
- Every version ships with a short test checklist covering only what that version changed.
  Brady installs it, runs the checklist and reports "worked" or "didn't work" (with any error
  text). Do not start the next version until he reports back.
- Keep `VERSION_LOG.md` up to date: one entry per version saying what changed, its checklist,
  and Brady's test result.
- Graphics: render every UI change as an image (480x480, round mask) and show it to Brady
  BEFORE writing the device code. Iterate on the image until he approves it, then write the code.
  Say plainly that a mockup is an approximation of what LVGL will draw.

## Toolchain (found on the PC, 2026-11-11)
- arduino-cli 1.5.1 is bundled inside Arduino IDE 2 at
  `%LOCALAPPDATA%\Programs\Arduino IDE\resources\app\lib\backend\resources\arduino-cli.exe`;
  use it with `--config-file C:/Users/Acer/.arduinoIDE/arduino-cli.yaml` so it shares the IDE's
  cores and libraries. `tools/build.sh [COMx]` wraps compile (+ upload). A clean build takes ~2 min.
- Core: esp32:esp32 3.3.12. Elecrow's readme asks for 2.0.14, but Brady's working CrowPanel
  sketches (M20C Cockpit Companion, `Documents\CrowPanel21_ST7701_WORKING_BASELINE`) run on
  3.3.12 with Arduino_GFX 1.6.9 + Adafruit_CST8XX, no LVGL. That baseline compiles via the CLI.
- Board settings (from his working builds): ESP32S3 Dev Module, 16 MB flash, Huge APP, OPI PSRAM,
  USB CDC On Boot enabled, 921600 upload.
- Known pins (from his working sketch, still to cross-check against the schematic in Stage 2):
  I2C SDA 38 / SCL 39, PCF8574 at 0x21 (touch reset P0, touch int P2, LCD power P3, LCD reset P4,
  encoder press P5), encoder A 42 / B 4, backlight 6, ST7701 SPI CS 16 / SCK 2 / MOSI 1.
- LVGL not ready: the installed lvgl is 9.6.0 but the shared `libraries/lv_conf.h` is for v8.4,
  and Elecrow's examples use LVGL 8.3. Decide before Stage 7; changing shared libraries could
  break Brady's other projects.

## Safety framing (non-negotiable)
- This is a portable advisory aid, not a certified instrument, not installed equipment, and
  never a substitute for see-and-avoid or certified traffic systems.
- Show an advisory-only notice at every startup.
- The UI must never make an empty screen look like "no traffic." Always show data status
  separately: antenna/receiver activity, GPS fix, and "last message received" age.
  "NO DATA" and "NO TRAFFIC" must look different.
- Receive-only. No transmitting.
- It is a PED: clipped in, powered by USB, not wired into the aircraft.

## Hardware in hand
| Part | Role | Status |
|---|---|---|
| Elecrow CrowPanel 2.1" HMI ESP32 Rotary Display | Brain, 480x480 round ST7701 display, touch, rotary encoder with press | Pinout NOT yet verified: find which GPIOs are exposed |
| 1090 MHz antenna (SMA whip) | Antenna | Being purchased (Stratux-style 1090/978 whip bundle). Check SMA gender vs SAWbird |
| Nooelec SAWbird+ ADS-B | 1090 MHz SAW filter + LNA (~34 dB), dual channel (1090 and 978 sides) | In hand. Power from ONE source only (USB, pin header, or bias-T). 3.3-5.5 V |
| AD8313 RF log-detector board (0.1-3 GHz, SMA in) | Converts RF envelope to a voltage | In hand. 2.7-5.5 V supply. Header: VDD, GND, OUT, GND |
| LM311 comparators, DIP-8 (10) | Converts the detector voltage to digital pulses | In hand. Open-collector output |
| 10k 25-turn trimmer pots (3296W-103) | Comparator threshold adjust | Ordered. Verify markings and value on arrival |
| GPS+BDS module (label "DW-114181") | Ownship position | In hand. Model, pinout, voltage, and antenna UNVERIFIED |
| 24 MHz 8-channel logic analyzer | Bench debugging of pulse output | In hand. Confirm software works on Mac (sigrok/PulseView) |
| CP2102 USB-to-serial adapter | Bench test of the GPS before wiring to the ESP32 | In hand |
| USB-C panel-mount sockets (10) | Case hardware, later | In hand (5.1k CC resistors reportedly on board) |

Not needed for this project (they belong to a separate project): piezo buzzers, DS3231,
BME280, DS18B20, GY-521 IMU, voltage modules, PCM5102 DAC, KCX_BT_EMITTER, Arduino Nano,
16x2 LCD, Raspberry Pi 4.

Still needed: threshold resistors (values chosen after bench measurement), pull-up resistor,
0.1 uF bypass capacitors, breadboard and jumpers, GPS antenna if not included.

## Planned signal chain
antenna -> SAWbird+ (1090 input) -> AD8313 (RF in) -> LM311 comparator -> pulse input on ESP32
-> decoder -> traffic list -> compass-rose display
GPS module -> UART -> ESP32 (ownship position for bearing and range)

Wiring cautions already identified (verify each against datasheets):
- Power the LM311 from 5 V (its minimum supply is about 3.5 V); pull its open-collector output
  up to 3.3 V so the ESP32 input is safe.
- SAWbird: never power from two sources at once.
- Common ground across every board.

## Known risks (be honest about these)
1. **Decoder feasibility on the ESP32-S3 is unproven.** ADS-B is 1 Mbps pulse-position
   modulation: 8 us preamble, then 56 or 112 data bits, with 0.5 us pulses. No working
   AD8313 + LM311 + ESP32-S3 decoder has been found. Research capture options (RMT RX, PCNT,
   GPIO interrupts with a hardware timer, I2S/LCD_CAM parallel sampling of the comparator
   output at 2 MHz or more) and report feasibility BEFORE committing. Prove pulses on the
   logic analyzer first.
2. **AD8313 slow recovery ("doughnut effect"):** its output falls slowly after a pulse, so a
   fixed comparator threshold can miss weak signals. A published design (MLAB ADSBAFE01)
   uses a fast comparator (~50 ns) and an adjustable/adaptive threshold. The LM311 is likely
   slower; check its datasheet. Pulses are about 0.5-1 us, so this may be marginal.
3. **GPS and Elecrow pin availability** are unknown.
4. **Fallbacks if the discrete chain fails:** a ready-made 1090 MHz receiver module (for
   example the GNS 5892 over UART, or an Aerobits TT-SC1b with an existing ESP32-S3 driver),
   or an ADSBee-style module. Do not switch without asking.

## Decoding notes (for later stages; verify before relying)
- Frame types of interest: DF17 extended squitter (ADS-B), DF11, DF18 (ground traffic).
- CRC-24 check on every frame; discard failures.
- Position uses CPR encoding. Local decoding against the GPS ownship position works with a
  single frame; global decoding needs an even/odd pair.
- Build a test harness with known-good captured frames so the decoder can be checked on the
  desktop before it runs on the ESP32.

## Build stages (stop after each for testing)
0. Toolchain: a command-line build (arduino-cli or PlatformIO) so Claude Code can compile and
   flash, plus whatever the Elecrow board needs (their example libraries, LVGL). Ask which
   toolchain he already has working. Confirm the board shows up as a serial port.
1. Hello world, then display test, then knob and press input test.
2. Pinout verification: list exposed GPIOs; choose pins for the GPS UART and the pulse input.
3. Bench RF test with NO ESP32: antenna -> SAWbird -> AD8313 -> LM311, watch the output on
   the logic analyzer. Goal: see burst-shaped pulses when aircraft are in range.
4. GPS: test with the CP2102 adapter first (NMEA text, baud rate), then on the ESP32 UART.
5. Pulse capture on the ESP32, verified against the logic analyzer.
6. Frame detection, CRC, and message decode with the test harness.
7. Traffic compass-rose UI on the round display (LVGL): own position at center, range rings,
   knob for range, altitude difference labels, data-status indicators. Layout options first.
8. Optional: WiFi access point broadcasting GDL90 (UDP 4000) for ForeFlight. Needs heartbeat,
   ownship report, and traffic reports. Not tested against an ESP32 yet.
9. Weather (see below).

## Weather (FIS-B), later phase
- FIS-B comes over 978 MHz UAT, which uses frequency-shift keying, so the AD8313 power
  detector chain cannot receive it.
- A separate UAT receiver is required. None selected yet. Candidate: an ADSBee-style dual-band
  module; the SAWbird+ already has a 978 MHz side. Decide before designing hardware.
- Decoding FIS-B (METAR/TAF/NEXRAD/TFR) is a large firmware task in itself.
- Weather is delayed, has coverage gaps, and is not a substitute for official briefings.
  The display must say so.
- Keep the architecture open so a second data source can be added later. Do not design
  around weather yet.

## Power and enclosure (later)
- First versions run on USB power. A battery and charger come later.
- The case is 3D printed, with a panel-mount USB-C port. Case design waits until the
  electronics are proven.
