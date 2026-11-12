#pragma once

// v1.4 knob and press test screen (mockup Option B): 24-tick ring with an amber marker
// that moves one tick per detent, signed count in the middle, press counter below.
// 24 clicks one way should bring the marker back to the gray 12 o'clock dot.
void knobTestBegin();
void knobTestUpdate();  // call from loop()
