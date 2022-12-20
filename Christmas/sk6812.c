#include <pdk/device.h>
#include <pdk/util.h>

#include "sk6812.h"
#include "sk6812_pins.h"

#define _ASMS(x) _VAR(x)
void sk6812_init(void) { SK6812_PORTC |= 1 << SK6812_PIN; }
extern uint8_t pixeldata[18];

void sk6812_output_data(void) {
  uint8_t index = 0;
  for (; index < 18; index += 1) {
    sk6812_send_8bit(pixeldata[index] >> 3); // Pfusch!!
  }
}

void sk6812_send_8bit(uint8_t val) {
  (void)val; //val is referenced from asm later, compiler can not see this and complains, so we declare it used manually
  __asm
      disgint
      mov a, #8
1$:
      set1.io _ASMS(SK6812_PORT), #(SK6812_PIN)             ;0
      sl _sk6812_send_8bit_PARM_1                           ;1
      t1sn.io f, c                                          ;2
      set0.io _ASMS(SK6812_PORT), #(SK6812_PIN)             ;3
      nop                                                   ;4
      nop                                                   ;5
      nop                                                   ;6
      set0.io _ASMS(SK6812_PORT), #(SK6812_PIN)             ;7
      dzsn a                                                ;8
      goto 1$                                               ;10
      engint
  __endasm;
}
