#ifndef RADAR_APP_H
#define RADAR_APP_H

#include <stdint.h>
#include "radar_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void RadarApp_Init(void);
void RadarApp_TaskLoop(void);

void RadarApp_Start(void);
void RadarApp_Stop(void);

void RadarApp_SetSpeedMode(RadarSpeedMode_t mode);
void RadarApp_NextSpeedMode(void);

void RadarApp_SetScanMode(uint8_t scan_mode_deg);
void RadarApp_ToggleScanMode(void);

#ifdef __cplusplus
}
#endif

#endif
