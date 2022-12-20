#include "led_matrix_effects.h"
#include <stdbool.h>

extern uint8_t frame_buffer[5];
extern uint8_t matrix_effect;
// Based on https://www.wfonts.com/font/3x7-normal
static const uint8_t font[36 * 3] = {
    // A
    0b1111110,
    0b0001001,
    0b1111111,
    // B
    0b1111110,
    0b1001001,
    0b1110111,
    // C
    0b1111110,
    0b1000001,
    0b1000001,
    // D
    0b1111111,
    0b1000001,
    0b1111110,
    // E
    0b1111111,
    0b1001001,
    0b1001001,
    // F
    0b1111111,
    0b0001001,
    0b0001001,
    // G
    0b1111110,
    0b1000001,
    0b0111001,
    // H
    0b1111111,
    0b0001000,
    0b1111111,
    // I
    0b0000000,
    0b1111111,
    0b0000000,
    // J
    0b1000000,
    0b1000000,
    0b0111111,
    // K
    0b1111111,
    0b0001000,
    0b1110111,
    // L
    0b1111111,
    0b1000000,
    0b1000000,
    // M
    0b1111111,
    0b0000010,
    0b1111111,
    // N
    0b1111111,
    0b0000010,
    0b1111100,
    // O
    0b1111110,
    0b1000001,
    0b1111111,
    // P
    0b1111110,
    0b0001001,
    0b0000111,
    // Q
    0b0111110,
    0b1100001,
    0b1011111,
    // R
    0b1111111,
    0b0001001,
    0b1110110,
    // S
    0b1000110,
    0b1001001,
    0b0110001,
    // T
    0b0000001,
    0b1111111,
    0b0000001,
    // U
    0b1111111,
    0b1000000,
    0b1111111,
    // V
    0b0111111,
    0b1000000,
    0b0111111,
    // W
    0b1111111,
    0b0100000,
    0b1111111,
    // X
    0b1111111,
    0b0001000,
    0b1111111,
    // Y
    0b0001111,
    0b1111000,
    0b0001111,
    // Z
    0b1110001,
    0b1001001,
    0b1000111,
    // 0
    0b1111110,
    0b1000001,
    0b1111111,
    // 1
    0b0000001,
    0b0000001,
    0b1111111,
    // 2
    0b1110001,
    0b1001001,
    0b1000110,
    // 3
    0b1001001,
    0b1001001,
    0b1111111,
    // 4
    0b0001111,
    0b0001000,
    0b1111111,
    // 5
    0b1001111,
    0b1001001,
    0b0111001,
    // 6
    0b1111110,
    0b1001001,
    0b1111001,
    // 7
    0b0000001,
    0b0000001,
    0b1111111,
    // 8
    0b1111110,
    0b1001001,
    0b1111111,
    // 9
    0b0001110,
    0b0001001,
    0b1111111,
};

// Modes
// Text
// Snow in the Wind
// Snowfall (with Tetris)
// Blank
//
static const char *text = "FROHES FEST! ";

// patterns:
// 0b10010
// 0b01001
// 0b00100
// 0b10010
// 0b01001
// 0b00100
// 0b10010

inline uint8_t output_character(uint8_t c, uint8_t off) {
  // Row is placed on PB0-PB6
  if ((c == ' ') || (off == 3)) {
    return 0;
  } else if (c == '.') {
    if (off == 1)
      return 0b1000000;
    else
      return 0;
  } else if (c == '!') {
    if (off == 1)
      return 0b1011111;
    else
      return 0;
  } else if (c == '-') {
    return 0b0001000;
  } else {
    uint8_t c_off;
    if ((c >= '0') && (c <= '9')) {
      c_off = 26 + c - '0';
    } else {
      c_off = c - 'A';
    }
    return font[c_off + c_off + c_off + off];
  }
}

static uint8_t t_offset = 0;
static uint8_t off = 0;
void text_effect(void) {
  uint8_t i = 0;
  off++;
  if (off > 3) {
    off = 0;
    t_offset++;
  }
  if (t_offset >= 13) {
    t_offset = 0;
  }
  uint8_t lt_offset = t_offset;
  uint8_t l_off = off;
  for (; i < 5; i++) {
    l_off++;
    if (l_off > 3) {
      l_off = 0;
      lt_offset++;
    }
    if (lt_offset >= 13) {
      lt_offset = 0;
    }
    frame_buffer[i] = output_character(text[lt_offset], l_off);
  }
}

void snow_effect(void) {
  uint8_t i = 0;
  t_offset++;
  if (t_offset >= 3) {
    t_offset = 0;
  }
  uint8_t lt_offset = t_offset;
  for (; i < 5; i++) {
    lt_offset++;
    if (lt_offset >= 3) {
      lt_offset = 0;
    }
    frame_buffer[i] = (0b1001 << lt_offset);
    frame_buffer[i] |= lt_offset ? 0 : 0b1000000;
  }
}
#define SOME_STUFF(x)                                                          \
  if ((content & (1 << (x - 1))) && !(content & (1 << x))) {                   \
    content |= (1 << x);                                                       \
    content &= ~(1 << (x - 1));                                                \
  }
void settling_effect(void) {
  uint8_t i = 0;
  t_offset++;
  if (t_offset >= 5) {
    t_offset = 0;
  }
  off++;
  if (off >= 3) {
    off = 0;
  }

  bool line_full = true;

  for (i = 0; i < 5; i++) {
    if (!(frame_buffer[i] & (1 << 6))) {
      line_full = false;
    }
  }
  if (line_full) {
    for (i = 0; i < 5; i++) {
      frame_buffer[i] <<= 1;
    }
  } else {
    for (i = 0; i < 5; i++) {
      // Use less ram, we have enough flash
      uint8_t content = frame_buffer[i];
      SOME_STUFF(6);
      SOME_STUFF(5);
      SOME_STUFF(4);
      SOME_STUFF(3);
      SOME_STUFF(2);
      SOME_STUFF(1);

      frame_buffer[i] = content;
    }
  }
  for (i = 0; i < 5; i++) {
    // Some basic "rng" to make the pattern not too obvious
    uint8_t new_flake = t_offset + off;
    if (new_flake >= 5) {
      new_flake -= 5;
    }

    if (i == new_flake) {
      frame_buffer[i] |= 0b1;
    }
  }
}
void blank_effect(void) {
  uint8_t i = 0;
  for (; i < 5; i++) {
    frame_buffer[i] = 0;
  }
}

void output_text(void) {
  if (matrix_effect == 0) {
    text_effect();
  } else if (matrix_effect == 1) {
    snow_effect();
  } else if (matrix_effect == 2) {
    settling_effect();
  } else if (matrix_effect == 3) {
    blank_effect();
  }
}
