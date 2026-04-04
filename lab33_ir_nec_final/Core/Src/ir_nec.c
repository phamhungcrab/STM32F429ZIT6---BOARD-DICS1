/*
 * ir_nec.c
 *
 *  Created on: 1 Apr 2026
 *      Author: HUNG PHAM
 */


#include "ir_nec.h"

static TIM_HandleTypeDef *ir_htim = 0;
static IR_NEC_Frame_t ir_frame = {0};

static uint32_t last_tick_us = 0;
static uint8_t receiving = 0;
static uint8_t bit_index = 0;
static uint32_t raw_data = 0;

/* Ngưỡng thời gian theo micro giây */
#define NEC_START_MIN_US   12000U
#define NEC_START_MAX_US   15000U

#define NEC_BIT0_MIN_US      900U
#define NEC_BIT0_MAX_US     1400U

#define NEC_BIT1_MIN_US     1900U
#define NEC_BIT1_MAX_US     2600U

#define NEC_FRAME_TIMEOUT_US 20000U

static void IR_NEC_ResetState(void)
{
    receiving = 0;
    bit_index = 0;
    raw_data = 0;
}

void IR_NEC_Init(TIM_HandleTypeDef *htim)
{
    ir_htim = htim;
    ir_frame.raw32 = 0;
    ir_frame.address = 0;
    ir_frame.address_inv = 0;
    ir_frame.command = 0;
    ir_frame.command_inv = 0;
    ir_frame.ready = 0;

    last_tick_us = 0;
    IR_NEC_ResetState();
}

static uint32_t IR_NEC_GetTickUS(void)
{
    if (ir_htim == 0)
    {
        return 0;
    }

    return __HAL_TIM_GET_COUNTER(ir_htim);
}

void IR_NEC_OnFallingEdge(void)
{
    uint32_t now_us;
    uint32_t delta_us;

    now_us = IR_NEC_GetTickUS();

    /* Cạnh đầu tiên: chỉ lưu mốc thời gian */
    if (last_tick_us == 0)
    {
        last_tick_us = now_us;
        return;
    }

    delta_us = now_us - last_tick_us;
    last_tick_us = now_us;

    /* Nếu khoảng cách quá dài thì coi như bắt đầu lại */
    if (delta_us > NEC_FRAME_TIMEOUT_US)
    {
        IR_NEC_ResetState();
        return;
    }

    /* Nhận bit start: khoảng từ đầu burst leader tới đầu burst data đầu tiên ~ 13.5 ms */
    if ((delta_us >= NEC_START_MIN_US) && (delta_us <= NEC_START_MAX_US))
    {
        receiving = 1;
        bit_index = 0;
        raw_data = 0;
        return;
    }

    if (!receiving)
    {
        return;
    }

    /* NEC gửi LSB trước */
    if ((delta_us >= NEC_BIT0_MIN_US) && (delta_us <= NEC_BIT0_MAX_US))
    {
        /* bit 0 -> không cần set */
    }
    else if ((delta_us >= NEC_BIT1_MIN_US) && (delta_us <= NEC_BIT1_MAX_US))
    {
        raw_data |= (1UL << bit_index);
    }
    else
    {
        /* Sai khuôn dạng -> bỏ frame hiện tại */
        IR_NEC_ResetState();
        return;
    }

    bit_index++;

    if (bit_index >= 32U)
    {
        ir_frame.raw32 = raw_data;
        ir_frame.address     = (uint8_t)( raw_data        & 0xFFU);
        ir_frame.address_inv = (uint8_t)((raw_data >> 8)  & 0xFFU);
        ir_frame.command     = (uint8_t)((raw_data >> 16) & 0xFFU);
        ir_frame.command_inv = (uint8_t)((raw_data >> 24) & 0xFFU);

        /* Kiểm tra tính hợp lệ của chuẩn NEC */
        if (((uint8_t)(ir_frame.address ^ ir_frame.address_inv) == 0xFFU) &&
            ((uint8_t)(ir_frame.command ^ ir_frame.command_inv) == 0xFFU))
        {
            ir_frame.ready = 1;
        }

        IR_NEC_ResetState();
    }
}

IR_NEC_Frame_t IR_NEC_GetFrame(void)
{
    return ir_frame;
}

void IR_NEC_ClearFrame(void)
{
    ir_frame.ready = 0;
}
