#include "sk6812_effects.h"

extern uint8_t pixeldata[18];
extern uint16_t rgbstate;
extern uint8_t effect;
extern uint8_t matrix_effect;
void effects(void) {
  uint8_t i = 0;
  if (matrix_effect == 0) {
    for (; i < 18; i += 3) {
      pixeldata[i + 0] = 223;
      pixeldata[i + 1] = 223;
      pixeldata[i + 2] = 20;
    }
    return;
  }
  uint16_t state = rgbstate;
  uint8_t upper_state;
  uint8_t lower_state;
  for (; i < 18; i += 3) {
    upper_state = state >> 8;
    lower_state = state & 0xFF;
    switch (effect) {
    case 0: // rainbow
      switch (upper_state) {
      case 0:
        pixeldata[i + 0] = lower_state;
        pixeldata[i + 1] = 0xFF - lower_state;
        pixeldata[i + 2] = 0;
        break;
      case 1:
        pixeldata[i + 0] = 0xFF - lower_state;
        pixeldata[i + 1] = 0;
        pixeldata[i + 2] = lower_state;
        break;
      case 2:
        pixeldata[i + 0] = 0;
        pixeldata[i + 1] = lower_state;
        pixeldata[i + 2] = 0xFF - lower_state;
      }
      state += 478;
      if (state >= 0x300)
        state -= 0x300;
      break;
    case 1: // Colorful flashing
      if (upper_state & 0b1) {
        lower_state = 0xFF - lower_state;
      }
      if (!(upper_state & 0b10)) {
        pixeldata[i + 0] = lower_state;
      } else {
        pixeldata[i + 0] = 0;
      }
      if (!(upper_state & 0b100)) {
        pixeldata[i + 1] = lower_state;
      } else {
        pixeldata[i + 1] = 0;
      }
      if (!(upper_state & 0b1000)) {
        pixeldata[i + 2] = lower_state;
      } else {
        pixeldata[i + 2] = 0;
      }
      state += 1716;
      if (state >= 0xE00)
        state -= 0xE00;
      break;
    case 2: // on-off pattern going around (regularly changing color)
    case 3: // on-off pattern going around (but two colors)
    case 4: // Light on going around (regularly changing color)
    case 5: // Two colors going around (regularly changing color)
      if ((effect == 2) || (effect == 4)) {
        if (lower_state & 0b10000) {
          lower_state = 0xFF;
        } else {
          lower_state = 0;
        }
      } else {
        if (lower_state & 0b10000) {
          upper_state = ~upper_state;
        }
        lower_state = 0xFF;
      }
      if ((upper_state & 0b1) == 0) {
        pixeldata[i + 0] = lower_state;
      } else {
        pixeldata[i + 0] = 0;
      }
      if ((upper_state & 0b10) == 0) {
        pixeldata[i + 1] = lower_state;
      } else {
        pixeldata[i + 1] = 0;
      }
      if ((upper_state & 0b100) == 0) {
        pixeldata[i + 2] = lower_state;
      } else {
        pixeldata[i + 2] = 0;
      }
      if ((effect == 4) || (effect == 5)) {
        state += 5;
      } else {
        state += 0b10000;
      }
      if (state >= 0x700)
        state -= 0x700;
      break;
    }
  }
  switch (effect) {
  case 0: // rainbow
    rgbstate += 4;
    if (rgbstate >= 0x300)
      rgbstate -= 0x300;
    break;
  case 1: // Colorful flashing
    rgbstate += 3;
    if (rgbstate >= 0xE00)
      rgbstate -= 0xE00;
    break;
  case 2: // on-off pattern going around (regularly changing color)
  case 3: // on-off pattern going around (but two colors)
  case 4: // Light on going around (regularly changing color)
  case 5: // Two colors going around (regularly changing color)
    rgbstate += 1;
    if (rgbstate >= 0x700)
      rgbstate -= 0x700;
    break;
  }
}
