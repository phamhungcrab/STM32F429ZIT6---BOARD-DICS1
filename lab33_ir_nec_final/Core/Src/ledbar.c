/*
 * ledbar.c
 *
 *  Created on: 1 Apr 2026
 *      Author: HUNG PHAM
 */


#include "ledbar.h"

static uint8_t ledbar_mode = LEDBAR_MODE_RUN_L;
static uint8_t led_value = 0x01;
static uint8_t flash_state = 0;
static uint8_t bumper_index = 0;

static const uint8_t bumper_pattern[] =
{
    0x81, // 10000001
    0x42, // 01000010
    0x24, // 00100100
    0x18, // 00011000
    0x18,
    0x24,
    0x42,
    0x81,
    0x00
};

static void LEDBar_WriteByte(uint8_t value)
{
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, (value & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, (value & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, (value & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, (value & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, (value & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_10, (value & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9,  (value & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8,  (value & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void LEDBar_SetMode(uint8_t mode)
{
    ledbar_mode = mode;

    /* reset trạng thái khi đổi mode */
    led_value = 0x01;
    flash_state = 0;
    bumper_index = 0;
}

void LEDBar_RunStep(void)
{
    switch (ledbar_mode)
    {
        case LEDBAR_MODE_RUN_L:
            /* phải -> trái */
            if (led_value == 0x00)
                led_value = 0x01;
            else if (led_value == 0x80)
                led_value = 0x00;
            else
                led_value <<= 1;
            LEDBar_WriteByte(led_value);
            break;

        case LEDBAR_MODE_RUN_R:
            /* trái -> phải */
            if (led_value == 0x00)
                led_value = 0x80;
            else if (led_value == 0x01)
                led_value = 0x00;
            else
                led_value >>= 1;
            LEDBar_WriteByte(led_value);
            break;

        case LEDBAR_MODE_FLASH:
            flash_state = !flash_state;
            LEDBar_WriteByte(flash_state ? 0xFF : 0x00);
            break;

        case LEDBAR_MODE_SPOT_BUMPER:
            LEDBar_WriteByte(bumper_pattern[bumper_index]);
            bumper_index++;
            if (bumper_index >= sizeof(bumper_pattern))
                bumper_index = 0;
            break;

        default:
            LEDBar_WriteByte(0x00);
            break;
    }
}
