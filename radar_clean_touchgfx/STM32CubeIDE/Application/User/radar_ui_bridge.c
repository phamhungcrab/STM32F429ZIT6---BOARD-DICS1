/*
 * radar_ui_bridge.c
 *
 *  Created on: 24 Jun 2026
 *      Author: HUNG PHAM
 */


#include "radar_ui_bridge.h"
#include "radar_config.h"
#include "main.h"
/*
 * Bridge này là vùng dữ liệu chung:
 * - RadarTask ở C ghi dữ liệu vào đây.
 * - TouchGFX C++ đọc dữ liệu từ đây để update UI.
 *
 * Dùng critical section ngắn để tránh đang copy struct thì bị ngắt/task khác sửa.
 */

static RadarUiData_t g_radar_ui_data;

static void RadarUiBridge_EnterCritical(void)
{
    __disable_irq();
}

static void RadarUiBridge_ExitCritical(void)
{
    __enable_irq();
}

void RadarUiBridge_Init(void)
{
    RadarUiData_t init_data;

    init_data.radar_enabled     = 0;
    init_data.angle_deg         = SERVO_CENTER_ANGLE_DEG;
    init_data.distance_cm       = 0;
    init_data.distance_valid    = 0;

    init_data.object_detected   = 0;
    init_data.near_warning      = 0;
    init_data.radar_status      = RADAR_STATUS_SCAN;

    init_data.scan_mode_deg     = RADAR_SCAN_MODE_180_DEG;
    init_data.speed_mode        = RADAR_SPEED_MED;

    init_data.object_count            = 0;
    init_data.last_object_distance_cm = 0;
    init_data.last_object_angle_deg   = 0;

    init_data.buzzer_on         = 0;
    init_data.led3_on           = 0;
    init_data.led4_on           = 0;
    init_data.oled_connected    = 0;

    RadarUiBridge_SetData(&init_data);
}

void RadarUiBridge_SetData(const RadarUiData_t *data)
{
    if (data == 0)
    {
        return;
    }

    __disable_irq();

    /*
     * Giữ lại các biến điều khiển do UI/Button set.
     *
     * Lý do:
     * RadarApp_TaskLoop cập nhật sensor data liên tục.
     * ScreenScan/ScreenHome/Settings cũng cập nhật enabled/speed/mode.
     *
     * Nếu SetData ghi đè toàn bộ struct, có thể xảy ra lỗi:
     * - UI vừa gọi RadarApp_Start() set enabled = 1
     * - Task đang giữ bản data cũ enabled = 0
     * - Task SetData lại làm enabled tụt về 0
     *
     * Vì vậy SetData chỉ nên update dữ liệu đo/hiển thị,
     * còn 3 biến control phải giữ theo state hiện tại.
     */
    uint8_t keep_enabled = g_radar_ui_data.radar_enabled;
    uint8_t keep_speed   = g_radar_ui_data.speed_mode;
    uint8_t keep_mode    = g_radar_ui_data.scan_mode_deg;

    g_radar_ui_data = *data;

    g_radar_ui_data.radar_enabled = keep_enabled;
    g_radar_ui_data.speed_mode    = keep_speed;
    g_radar_ui_data.scan_mode_deg = keep_mode;

    __enable_irq();
}

void RadarUiBridge_GetData(RadarUiData_t *data)
{
    if (data == 0)
    {
        return;
    }

    RadarUiBridge_EnterCritical();
    *data = g_radar_ui_data;
    RadarUiBridge_ExitCritical();
}

void RadarUiBridge_SetRadarEnabled(uint8_t enabled)
{
    RadarUiBridge_EnterCritical();
    g_radar_ui_data.radar_enabled = enabled ? 1U : 0U;
    RadarUiBridge_ExitCritical();
}

uint8_t RadarUiBridge_GetRadarEnabled(void)
{
    uint8_t enabled;

    RadarUiBridge_EnterCritical();
    enabled = g_radar_ui_data.radar_enabled;
    RadarUiBridge_ExitCritical();

    return enabled;
}

void RadarUiBridge_SetSpeedMode(uint8_t speed_mode)
{
    if (speed_mode > RADAR_SPEED_FAST)
    {
        speed_mode = RADAR_SPEED_MED;
    }

    RadarUiBridge_EnterCritical();
    g_radar_ui_data.speed_mode = speed_mode;
    RadarUiBridge_ExitCritical();
}

uint8_t RadarUiBridge_GetSpeedMode(void)
{
    uint8_t mode;

    RadarUiBridge_EnterCritical();
    mode = g_radar_ui_data.speed_mode;
    RadarUiBridge_ExitCritical();

    return mode;
}

void RadarUiBridge_SetScanMode(uint8_t scan_mode_deg)
{
    if ((scan_mode_deg != RADAR_SCAN_MODE_90_DEG) &&
        (scan_mode_deg != RADAR_SCAN_MODE_180_DEG))
    {
        scan_mode_deg = RADAR_SCAN_MODE_180_DEG;
    }

    RadarUiBridge_EnterCritical();
    g_radar_ui_data.scan_mode_deg = scan_mode_deg;
    RadarUiBridge_ExitCritical();
}

uint8_t RadarUiBridge_GetScanMode(void)
{
    uint8_t mode;

    RadarUiBridge_EnterCritical();
    mode = g_radar_ui_data.scan_mode_deg;
    RadarUiBridge_ExitCritical();

    return mode;
}

void RadarUiBridge_NextSpeedMode(void)
{
    RadarUiBridge_EnterCritical();

    if (g_radar_ui_data.speed_mode == RADAR_SPEED_SLOW)
    {
        g_radar_ui_data.speed_mode = RADAR_SPEED_MED;
    }
    else if (g_radar_ui_data.speed_mode == RADAR_SPEED_MED)
    {
        g_radar_ui_data.speed_mode = RADAR_SPEED_FAST;
    }
    else
    {
        g_radar_ui_data.speed_mode = RADAR_SPEED_SLOW;
    }

    RadarUiBridge_ExitCritical();
}
