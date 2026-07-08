#ifndef BUZZER_LED_H
#define BUZZER_LED_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void BuzzerLed_Init(void);

void Buzzer_Set(uint8_t on);
void LedScan_Set(uint8_t on);
void LedAlert_Set(uint8_t on);

void Alert_Update(uint8_t detected, uint8_t near_warning);

#ifdef __cplusplus
}
#endif

#endif
