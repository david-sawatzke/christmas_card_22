#include "led_matrix.h"

#include <delay.h>

#include <pdk/device.h>
#include <pdk/util.h>
extern uint8_t frame_buffer[5];

void output_frame(void) {
  uint8_t i;
  for (i = 0; i < 5; i++) {
    switch (i) {
    case 0:
      PA = 1 << 0;
      break;
    case 1:
      PA = 1 << 3;
      break;
    case 2:
      PA = 1 << 4;
      break;
    case 3:
      PA = 1 << 6;
      break;
    case 4:
      PA = 1 << 7;
      break;
    }
    PB = frame_buffer[i] & 0x7F;
    _delay_us(50);
    PB = 0;
  }
}

void init_frame_output() {}
