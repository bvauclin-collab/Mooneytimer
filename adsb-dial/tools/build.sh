#!/usr/bin/env bash
# Build (and optionally flash) the ADS-B Dial sketch with the arduino-cli that ships
# inside Arduino IDE 2, so the command line uses the same cores and libraries as the IDE.
#
#   tools/build.sh            compile only
#   tools/build.sh COM5       compile, then upload to COM5
#
# Upload fails if the IDE's Serial Monitor (or anything else) has the port open.
set -euo pipefail

HERE="$(cd "$(dirname "$0")/.." && pwd)"
SKETCH="$HERE/ADSB_Dial"
BUILD="$HERE/build"
CLI="${ARDUINO_CLI:-/c/Users/Acer/AppData/Local/Programs/Arduino IDE/resources/app/lib/backend/resources/arduino-cli.exe}"
CONFIG="${ARDUINO_CLI_CONFIG:-C:/Users/Acer/.arduinoIDE/arduino-cli.yaml}"

# Same board settings Brady's working CrowPanel 2.1" sketches use (ESP32S3 Dev Module,
# 16 MB flash, Huge APP partition, OPI PSRAM, USB CDC on boot so Serial goes to USB).
FQBN="esp32:esp32:esp32s3:UploadSpeed=921600,USBMode=hwcdc,CDCOnBoot=cdc,CPUFreq=240,FlashMode=qio,FlashSize=16M,PartitionScheme=huge_app,PSRAM=opi"

"$CLI" --config-file "$CONFIG" compile --fqbn "$FQBN" --build-path "$BUILD" "$SKETCH"

if [ $# -ge 1 ]; then
  "$CLI" --config-file "$CONFIG" upload -p "$1" --fqbn "$FQBN" --input-dir "$BUILD" "$SKETCH"
fi
