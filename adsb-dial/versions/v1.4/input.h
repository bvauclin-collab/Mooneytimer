#pragma once
#include <Arduino.h>

// Rotary knob and its press switch.
// Knob: quadrature on GPIO 42/4, read in an interrupt; 2 transitions = 1 detent
// (measured on this CrowPanel in Brady's earlier sketch). Clockwise is positive.
// Press: PCF8574 bit 5, active low, polled over I2C. Call displayBegin() first (it starts I2C).

void inputBegin();

// Detents turned since the last call (+ clockwise, - counter-clockwise).
int inputTakeSteps();

// True once per press, on the press-down edge (debounced).
bool inputTakePress();
