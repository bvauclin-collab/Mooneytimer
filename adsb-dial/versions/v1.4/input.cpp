#include "input.h"
#include <Wire.h>
#include "board_pins.h"

static volatile int lastEncoded = 0;
static volatile int accumulator = 0;
static volatile int steps = 0;

static void IRAM_ATTR encoderIsr() {
  int a = digitalRead(PIN_ENCODER_A);
  int b = digitalRead(PIN_ENCODER_B);
  int encoded = (a << 1) | b;
  int sum = (lastEncoded << 2) | encoded;
  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) accumulator++;
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) accumulator--;
  lastEncoded = encoded;
  if (accumulator >= 2)  { steps++; accumulator = 0; }
  if (accumulator <= -2) { steps--; accumulator = 0; }
}

void inputBegin() {
  pinMode(PIN_ENCODER_A, INPUT_PULLUP);
  pinMode(PIN_ENCODER_B, INPUT_PULLUP);
  lastEncoded = (digitalRead(PIN_ENCODER_A) << 1) | digitalRead(PIN_ENCODER_B);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_A), encoderIsr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_ENCODER_B), encoderIsr, CHANGE);
}

int inputTakeSteps() {
  noInterrupts();
  int s = steps;
  steps = 0;
  interrupts();
  return -s;  // the raw count runs negative when turned clockwise
}

static bool readPressRaw() {
  Wire.requestFrom((uint8_t)PCF8574_ADDR, (uint8_t)1);
  if (!Wire.available()) return false;
  return !(Wire.read() & (1 << PCF_ENCODER_SW));
}

bool inputTakePress() {
  static bool stable = false;        // debounced state
  static bool lastRaw = false;
  static unsigned long changedAt = 0;
  static unsigned long lastPoll = 0;

  unsigned long now = millis();
  if (now - lastPoll < 5) return false;  // poll the I2C expander at most every 5 ms
  lastPoll = now;

  bool raw = readPressRaw();
  if (raw != lastRaw) { lastRaw = raw; changedAt = now; }
  if (raw != stable && now - changedAt >= 30) {
    stable = raw;
    return stable;  // report only the press-down edge
  }
  return false;
}
