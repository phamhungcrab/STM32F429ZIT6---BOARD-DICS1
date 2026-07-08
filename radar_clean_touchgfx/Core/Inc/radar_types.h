#ifndef RADAR_TYPES_H
#define RADAR_TYPES_H

#include <stdint.h>
#include "radar_config.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
    RADAR_SPEED_SLOW = 0,
    RADAR_SPEED_MED  = 1,
    RADAR_SPEED_FAST = 2
} RadarSpeedMode_t;

typedef enum
{
    RADAR_STATUS_SCAN = 0,
    RADAR_STATUS_DETECT,
    RADAR_STATUS_ALERT
} RadarStatus_t;

typedef struct
{
    uint8_t  radar_enabled;

    uint16_t angle_deg;
    uint16_t distance_cm;
    uint8_t  distance_valid;

    uint8_t  object_detected;
    uint8_t  near_warning;
    uint8_t  radar_status;

    uint8_t  scan_mode_deg;
    uint8_t  speed_mode;

    uint16_t object_count;

    /*
     * Last detected object information:
     * - object_count chỉ tăng khi có object mới.
     * - Nhưng last_object_distance_cm và last_object_angle_deg
     *   sẽ update mỗi lần radar đang phát hiện được vật.
     */
    uint16_t last_object_distance_cm;
    uint16_t last_object_angle_deg;

    uint8_t  buzzer_on;
    uint8_t  led3_on;
    uint8_t  led4_on;
    uint8_t  oled_connected;
} RadarUiData_t;

#ifdef __cplusplus
}
#endif

#endif
