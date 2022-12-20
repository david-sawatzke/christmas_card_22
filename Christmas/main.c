#include <pdk/device.h>

#include "auto_sysclock.h"
#include "led_matrix.h"
#include "led_matrix_effects.h"
#include "sk6812.h"
#include "sk6812_effects.h"
#include <stdbool.h>
#include <stdint.h>

uint8_t frame_buffer[5];

// grb
uint8_t pixeldata[18];

uint16_t rgbstate = 0;
// Main processing loop
uint8_t effect = 0;
uint8_t matrix_effect = 0;
uint8_t button_delay = 0;
uint8_t effect_delay = 0;
uint8_t matrix_effect_delay = 0;
uint8_t button_state = 0b11111111;
bool attract_mode = 1;

// Main program
void main() {
  // Initialize hardware
  sk6812_init();
  init_frame_output();
  PAPH = 1 << 5;
  PAC = 0b11011111;
  PBC = 0b11111111;

  while (1) {
    // This loop takes approx. 4-5ms
    // Button Processing
    // Debouncing requires approx 28ms of the same state
    // This tries to save on ram, a 16 bit constant has dedicated ram space
    button_delay++;
    if ((button_delay & 0x7F) == 0b1000000) {
      if (attract_mode) {
        effect_delay++;
        matrix_effect_delay++;
      }
    }
    if ((button_delay & 0xF) == 0b1000) {
      button_state <<= 1;
      button_state |= (PA >> 5) & 0b1;
      if (button_state == 0b10000000) {
        attract_mode ^= 1;
        if (attract_mode) {
          effect++;
          effect_delay = 0;
          matrix_effect++;
          matrix_effect_delay = 0;
        }
      }
    }
    if (effect_delay == 0xFA) {
      effect++;
      effect_delay = 0;
    }
    if (matrix_effect_delay == 0xE3) {
      matrix_effect++;
      matrix_effect_delay = 0;
    }
    if (effect > 5) {
      effect -= 6;
    }
    if (matrix_effect > 3) {
      matrix_effect -= 4;
    }

    output_frame();
    if (!(button_delay & 0b11111))
      effects();
    output_frame();
    sk6812_output_data();
    output_frame();
    if (!(button_delay & 0b1111111))
      output_text();
    output_frame();
  }
}

// Startup code - Setup/calibrate system clock
unsigned char _sdcc_external_startup(void) {
  __asm wdreset __endasm;
  // Initialize the system clock (CLKMD register) with the IHRC, ILRC, or EOSC
  // clock source and correct divider. The AUTO_INIT_SYSCLOCK() macro uses
  // F_CPU (defined in the Makefile) to choose the IHRC or ILRC clock source
  // and divider. Alternatively, replace this with the more specific
  // PDK_SET_SYSCLOCK(...) macro from pdk/sysclock.h
  PDK_USE_FACTORY_IHRCR_16MHZ();
  CLKMD = 0b00110000;
  /* AUTO_INIT_SYSCLOCK(); */
  /* PDK_DISABLE_ILRC(); */

  // Insert placeholder code to tell EasyPdkProg to calibrate the IHRC or ILRC
  // internal oscillator. The AUTO_CALIBRATE_SYSCLOCK(...) macro uses F_CPU
  // (defined in the Makefile) to choose the IHRC or ILRC oscillator.
  // Alternatively, replace this with the more specific
  // EASY_PDK_CALIBRATE_IHRC(...) or EASY_PDK_CALIBRATE_ILRC(...) macro from
  // easy-pdk/calibrate.h
  /* AUTO_CALIBRATE_SYSCLOCK(TARGET_VDD_MV); */
  // Use Fab calibration

  T16M = 0;
  EOSCR = 0;
  TM2C = 0;
  TM3C = 0;
  return 0; // Return 0 to inform SDCC to continue with normal initialization.
}
