#ifndef __STOPWATCH_H
#define __STOPWATCH_H

#include "stm32f10x.h"

// 秒表状态定义
typedef enum {
	STOPWATCH_RESET = 0,
	STOPWATCH_RUNNING,
	STOPWATCH_PAUSED
} StopwatchState;

// 秒表结构体
typedef struct {
	StopwatchState state;
	uint32_t seconds;
	uint32_t hundredths;
	char display_str[16];
} Stopwatch;

// 函数声明
void Stopwatch_Init(void);
void Stopwatch_ProcessKey(uint8_t key);
void Stopwatch_UpdateDisplay(void);
void Stopwatch_RequestFullRefresh(void);
void Stopwatch_Run(uint8_t key);
void Stopwatch_ReturnToMenu(void);

#endif
