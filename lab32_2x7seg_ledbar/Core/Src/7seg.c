#include "7seg.h"

/*
 * Mapping mặc định của bài 3.2:
 * PE8  -> a
 * PE9  -> b
 * PE10 -> c
 * PE11 -> d
 * PE12 -> e
 * PE13 -> f
 * PE14 -> g
 * PE15 -> dp
 *
 * PG2 -> chọn LED 7 thanh bên trái
 * PG3 -> chọn LED 7 thanh bên phải
 *
 * Nếu nạp lên mạch thật mà sáng ngược:
 * - đổi SEG_ON / SEG_OFF
 * - hoặc đổi DIGIT_ON / DIGIT_OFF
 */

#define SEG_ON      GPIO_PIN_SET
#define SEG_OFF     GPIO_PIN_RESET

#define DIGIT_ON    GPIO_PIN_SET
#define DIGIT_OFF   GPIO_PIN_RESET

static uint8_t seg_left = 0;
static uint8_t seg_right = 0;
static uint8_t current_digit = 0;

/* Mã hiển thị cho LED 7 thanh:
 * bit0=a, bit1=b, bit2=c, bit3=d, bit4=e, bit5=f, bit6=g, bit7=dp
 */
static const uint8_t seg_code[10] =
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
    0x6F  // 9
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

void Set7SegDisplayValue(uint8_t value)
{
    if (value > 99)
    {
        value %= 100;
    }

    seg_left = value / 10;
    seg_right = value % 10;
}

void Run7SegDisplay(void)
{
    /* Tắt cả 2 digit trước khi đổi dữ liệu bus để tránh lóe sai */
    TurnOffAllDigits();

    if (current_digit == 0)
    {
        Write7SegBus(seg_code[seg_left]);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, DIGIT_ON);   // bật digit trái
        current_digit = 1;
    }
    else
    {
        Write7SegBus(seg_code[seg_right]);
        HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, DIGIT_ON);   // bật digit phải
        current_digit = 0;
    }
}
