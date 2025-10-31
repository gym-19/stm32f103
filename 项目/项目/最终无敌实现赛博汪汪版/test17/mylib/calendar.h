#ifndef __CALENDAR_H
#define __CALENDAR_H

#include "stm32f10x.h"
#include "z_mytime.h"
#include "system_manager.h"

// ??????(??????????)
typedef enum {
    CALENDAR_MODE_SIMPLE = 0    // ??????
} CalendarMode;

// ?????
typedef struct {
    CalendarMode display_mode;
    uint16_t current_year;
    uint8_t current_month;
    uint8_t current_day;
    uint8_t current_hour;      // ????
    uint8_t current_minute;    // ????
    uint8_t current_second;    // ????
    uint8_t selected_day;
    uint8_t week_start;  // 0=??, 1=??
} CalendarTypeDef;

// ????
void Calendar_Init(void);
void Calendar_UpdateDisplay(void);
void Calendar_Run(uint8_t key);
uint8_t Calendar_GetDaysInMonth(uint16_t year, uint8_t month);
uint8_t Calendar_GetDayOfWeek(uint16_t year, uint8_t month, uint8_t day);
void Calendar_RefreshCurrentDate(void);

#endif


