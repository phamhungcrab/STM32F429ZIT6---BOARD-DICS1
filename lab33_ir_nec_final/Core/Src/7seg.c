#include "7seg.h"

/*
 * Mapping phần cứng:
 * PE8  -> a
 * PE9  -> b
 * PE10 -> c
 * PE11 -> d
 * PE12 -> e
 * PE13 -> f
 * PE14 -> g
 * PE15 -> dp
 *
 * PG2 -> chọn digit trái
 * PG3 -> chọn digit phải
 *
 * Bản đầu này giả sử:
 * - LED 7 thanh là common-cathode
 * - transistor NPN điều khiển digit, mức HIGH ở PGx sẽ bật digit
 */

#define SEG_ON      GPIO_PIN_SET
#define SEG_OFF     GPIO_PIN_RESET

#define DIGIT_ON    GPIO_PIN_SET
#define DIGIT_OFF   GPIO_PIN_RESET

static uint8_t left_code = 0;
static uint8_t right_code = 0;
static uint8_t current_digit = 0;

/* Mã 7 đoạn cho HEX:
 * bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
 */
static const uint8_t hex7seg[16] =
{
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71  // F
};

static void TurnOffAllDigits(void)
{
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, DIGIT_OFF);
    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, DIGIT_OFF);
}

static void Write7SegBus(uint8_t code)
{
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8,  (code & (1 << 0)) ? SEG_ON : SEG_OFF);  // a
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9,  (code & (1 << 1)) ? SEG_ON : SEG_OFF);  // b
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (code & (1 << 2)) ? SEG_ON : SEG_OFF);  // c
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, (code & (1 << 3)) ? SEG_ON : SEG_OFF);  // d
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, (code & (1 << 4)) ? SEG_ON : SEG_OFF);  // e
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, (code & (1 << 5)) ? SEG_ON : SEG_OFF);  // f
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, (code & (1 << 6)) ? SEG_ON : SEG_OFF);  // g
    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, (code & (1 << 7)) ? SEG_ON : SEG_OFF);  // dp
}

void Set7SegHexByte(uint8_t value)
{
    uint8_t high_nibble = (value >> 4) & 0x0F;
    uint8_t low_nibble  = value & 0x0F;

    left_code = hex7seg[high_nibble];
    right_code = hex7seg[low_nibble];
}

void Run7SegDisplay(void)
{
    /* Tắt cả 2 digit trước khi đổi bus để tránh lóe sai nét */
    TurnOffAllDigits();

    if (current_digit == 0)
    {
        Write7SegBus(left_code);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, DIGIT_ON);
        current_digit = 1;
    }
    else
    {
        Write7SegBus(right_code);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, DIGIT_ON);
        current_digit = 0;
    }
}
