#include "buzzer_led.h"
#include "main.h"
#include <stdint.h>

void Buzzer_Set(uint8_t on)
{
#if defined(BUZZER_GPIO_Port) && defined(BUZZER_Pin)
    HAL_GPIO_WritePin(BUZZER_GPIO_Port,
                      BUZZER_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#else
    (void)on;
#endif
}

void LedScan_Set(uint8_t on)
{
#if defined(LED3_SCAN_GPIO_Port) && defined(LED3_SCAN_Pin)
    HAL_GPIO_WritePin(LED3_SCAN_GPIO_Port,
                      LED3_SCAN_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#else
    (void)on;
#endif
}

void LedAlert_Set(uint8_t on)
{
#if defined(LED4_ALERT_GPIO_Port) && defined(LED4_ALERT_Pin)
    HAL_GPIO_WritePin(LED4_ALERT_GPIO_Port,
                      LED4_ALERT_Pin,
                      on ? GPIO_PIN_SET : GPIO_PIN_RESET);
#else
    (void)on;
#endif
}
