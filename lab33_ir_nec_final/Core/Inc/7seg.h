#ifndef INC_7SEG_H_
#define INC_7SEG_H_

#include "main.h"
#include <stdint.h>

/* Hiển thị 1 byte theo dạng HEX trên 2 LED 7 thanh
   Ví dụ:
   0x00 -> "00"
   0x1A -> "1A"
   0xFF -> "FF"
*/
void Set7SegHexByte(uint8_t value);

/* Hàm quét 2 digit, gọi lặp rất nhanh trong while(1) */
void Run7SegDisplay(void);

#endif /* INC_7SEG_H_ */
