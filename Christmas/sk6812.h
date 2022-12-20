#ifndef SK6812_H_
#define SK6812_H_

#include <stdint.h>

void sk6812_init(void);
void sk6812_output_data(void);
void sk6812_send_8bit(uint8_t val);

#endif // SK6812_H_
