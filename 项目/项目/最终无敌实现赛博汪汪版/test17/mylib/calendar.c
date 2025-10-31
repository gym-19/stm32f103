#include "calendar.h"
#include "oled.h"
#include "stdio.h"
#include "delay.h"
#include "system_manager.h"

// ??????
static CalendarTypeDef calendar;
static const char* month_names[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

// ??????
static const char* week_days[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

void Calendar_Init(void)
{
    calendar.display_mode = CALENDAR_MODE_SIMPLE;
    calendar.week_start = 0;  // ??????
    calendar.selected_day = 1;
    Calendar_RefreshCurrentDate();
}

void Calendar_RefreshCurrentDate(void)
{
    DateTimeTypeDef current_time;
    uint32_t timestamp = RTC_GetTimestamp();
    
    TimestampToDateTime(timestamp, &current_time);
    
    calendar.current_year = current_time.year;
    calendar.current_month = current_time.month;
    calendar.current_day = current_time.day;
    calendar.current_hour = current_time.hour;
    calendar.current_minute = current_time.minute;
    calendar.current_second = current_time.second;
    
    // ???????????,?????
    if(calendar.current_year == current_time.year && 
       calendar.current_month == current_time.month) {
        calendar.selected_day = calendar.current_day;
    }
}

uint8_t Calendar_GetDaysInMonth(uint16_t year, uint8_t month)
{
    const uint8_t daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    
    if(month == 2) {
        // ????
        if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            return 29;
        }
        return 28;
    }
    return daysInMonth[month - 1];
}

uint8_t Calendar_GetDayOfWeek(uint16_t year, uint8_t month, uint8_t day)
{
    // ??Zeller???????
    uint16_t y;
    uint8_t m;
    uint8_t d;
    uint16_t century;
    uint8_t week_day;
    
    y = year;
    m = month;
    d = day;
    
    if(m < 3) {
        m += 12;
        y -= 1;
    }
    
    century = y / 100;
    y = y % 100;
    
    week_day = (d + (13*(m+1))/5 + y + y/4 + century/4 - 2*century) % 7;
    
    // ????:0=??,1=??,...,6=??
    return (week_day + 6) % 7;  // ???0=??,1=??,...,6=??
}

void Calendar_UpdateDisplay(void)
{
    char buffer[32];
    uint8_t week_day;
    
    // ???????
    week_day = Calendar_GetDayOfWeek(calendar.current_year, 
                                    calendar.current_month, 
                                    calendar.current_day);
    
    OLED_Clear();
    
    // ?1?:????? (y=0)
    sprintf(buffer, "%s %d %d", month_names[calendar.current_month - 1], 
                                calendar.current_day, 
                                calendar.current_year);
    OLED_ShowString(0, 0, (uint8_t*)buffer, 16);
    
    // ?2?:???? (y=16)
    sprintf(buffer, "%s", week_days[week_day]);
    OLED_ShowString(0, 2, (uint8_t*)buffer, 16);
    
    // ?3?:????? (y=32)
    sprintf(buffer, "Time: %02d:%02d:%02d", calendar.current_hour, 
                                          calendar.current_minute, 
                                          calendar.current_second);
    OLED_ShowString(0, 4, (uint8_t*)buffer, 16);
    
    // ?4?:?????? (y=48)
    OLED_ShowString(0, 6, (uint8_t*)"U:Back 5:Refresh", 16);
}

void Calendar_Run(uint8_t key)
{
    switch(key) {
        case '5':  // ????
            Calendar_RefreshCurrentDate();
            Calendar_UpdateDisplay();
            break;
            
        case 'U':
        case 'L':  // ?????
            Change_State(SYS_MENU);
            Show_Main_Menu(5);  // ????5??MENU_CALENDAR
            break;
    }
}


