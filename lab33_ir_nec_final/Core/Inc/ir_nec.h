#ifndef INC_IR_NEC_H_
#define INC_IR_NEC_H_

#include "main.h"
#include <stdint.h>

typedef struct
{
    uint32_t raw32;       /* 32 bit đầy đủ của frame NEC */
    uint8_t address;      /* byte 1 */
    uint8_t address_inv;  /* byte 2 */
    uint8_t command;      /* byte 3 */
    uint8_t command_inv;  /* byte 4 */
    uint8_t ready;        /* =1 khi đã nhận xong 1 lệnh hợp lệ */
} IR_NEC_Frame_t;

/* Gọi 1 lần sau khi start TIM2 */
void IR_NEC_Init(TIM_HandleTypeDef *htim);

/* Gọi trong EXTI handler của PG5 mỗi khi có cạnh xuống */
void IR_NEC_OnFallingEdge(void);

/* Lấy frame hiện tại */
IR_NEC_Frame_t IR_NEC_GetFrame(void);

/* Xóa cờ ready sau khi đã xử lý xong */
void IR_NEC_ClearFrame(void);

#endif /* INC_IR_NEC_H_ */
