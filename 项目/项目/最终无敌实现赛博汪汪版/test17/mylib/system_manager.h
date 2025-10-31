#ifndef __SYSTEM_MANAGER_H
#define __SYSTEM_MANAGER_H

#include "stm32f10x.h"

// 系统状态定义
typedef enum {
    STATE_STARTUP = 0,
    SYS_MENU,
    SYS_CALCULATOR,
    SYS_STOPWATCH,
    SYS_DHT,
		SYS_DS18B20,       // 新增：DS18B20温度探头状态
		SYS_FAN_ADJUST,   // 新增：风扇调节状态
	  SYS_CALENDAR,
		//SYS_FAN          // 新增：风扇调节状态（控制第二个舵机）
    // 可以在这里添加新状态，比如 SYS_SETTINGS, SYS_CLOCK 等
} SystemState;



void System_Init(void);
void System_Update(void);
void Change_State(SystemState new_state);
void Show_Main_Menu(uint8_t selected_index);
void DHT_UpdateDisplay(void);
void DHT_Run(uint8_t key);

// 新增：DS18B20和风扇调节相关函数声明
void DS18B20_UpdateDisplay(void);  // DS18B20温度显示更新
void DS18B20_Run(uint8_t key);     // DS18B20状态按键处理
void Fan_Adjust_UpdateDisplay(void);// 风扇调节显示更新
void Fan_Adjust_Run(uint8_t key);  // 风扇调节状态按键处理

extern void Calendar_UpdateDisplay(void);
extern void Calendar_Run(uint8_t key);

// 添加这些函数声明，供key_manager.c使用
SystemState Get_Current_State(void);
uint8_t Get_Selected_Menu_Index(void);
void Set_Selected_Menu_Index(uint8_t index);

// 新增：获取菜单项数量
uint8_t Get_Menu_Count(void);

#endif

