#ifndef INC_LEDBAR_H_
#define INC_LEDBAR_H_

#include "main.h"
#include <stdint.h>

/* Các mode hiệu ứng cho dãy LED đơn */
#define LEDBAR_MODE_RUN_L         1
#define LEDBAR_MODE_RUN_R         2
#define LEDBAR_MODE_FLASH         3
#define LEDBAR_MODE_SPOT_BUMPER   4

void LEDBar_SetMode(uint8_t mode);
void LEDBar_RunStep(void);

#endif /* INC_LEDBAR_H_ */
