#include "radar_debug.h"
#include "main.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#if defined(HAL_UART_MODULE_ENABLED)
extern UART_HandleTypeDef huart1;
#endif

void RadarDebug_Printf(const char *fmt, ...)
{
#if defined(HAL_UART_MODULE_ENABLED)
    char buffer[160];

    va_list args;
    va_start(args, fmt);
    int len = vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    if (len <= 0)
    {
        return;
    }

    if (len > (int)sizeof(buffer))
    {
        len = sizeof(buffer);
    }

    HAL_UART_Transmit(&huart1, (uint8_t *)buffer, (uint16_t)len, 50);
#else
    (void)fmt;
#endif
}
