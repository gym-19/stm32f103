#include "stopwatch.h"
#include "oled.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>
#include "stm32f10x_tim.h"

Stopwatch sw;

static uint8_t need_full_refresh = 1;
static char last_display_str[16] = "00:00.00";
static char last_state_str[16] = "RESET";

// 定时器初始化
void Stopwatch_TIM_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  // 变量声明移到开头
	
	// 使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 定时器基础设置
	TIM_TimeBaseStructure.TIM_Period = 100 - 1;         // 自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;     // 预分频器
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	// 允许定时器更新中断
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	// 定时器中断优先级设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	// 启动定时器
	TIM_Cmd(TIM2, ENABLE);
}

// 定时器中断服务函数
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
		// 只有在运行状态时才更新时间
		if (sw.state == STOPWATCH_RUNNING) {
			sw.hundredths++;
			
			if (sw.hundredths >= 100) {
				sw.hundredths = 0;
				sw.seconds++;
			}
			
			// 更新显示字符串
			snprintf(sw.display_str, sizeof(sw.display_str), "%02lu:%02lu.%02lu",
					 sw.seconds / 60, sw.seconds % 60, sw.hundredths);
		}
	}
}

void Stopwatch_Init(void) {
	sw.state = STOPWATCH_RESET;
	sw.seconds = 0;
	sw.hundredths = 0;
	strcpy(sw.display_str, "00:00.00");
	need_full_refresh = 1;
	strcpy(last_display_str, "00:00.00");
	strcpy(last_state_str, "RESET");
	
	// 初始化定时器
	Stopwatch_TIM_Init();
}

void Stopwatch_ProcessKey(uint8_t key) {
	switch(key) {
		case '5': // 开始/暂停/继续
		if (sw.state == STOPWATCH_RESET || sw.state == STOPWATCH_PAUSED) {
			sw.state = STOPWATCH_RUNNING;
		} else if (sw.state == STOPWATCH_RUNNING) {
			sw.state = STOPWATCH_PAUSED;
		}
		break;
		
		case '0': // 重置
		sw.state = STOPWATCH_RESET;
		sw.seconds = 0;
		sw.hundredths = 0;
		strcpy(sw.display_str, "00:00.00");
		break;
		
	case 'L':
		break;
	}
}

void Stopwatch_UpdateDisplay(void)
{
	const char* state_str;
	
	switch(sw.state) {
		case STOPWATCH_RUNNING: state_str = "RUNNING"; break;
		case STOPWATCH_PAUSED: state_str = "PAUSED"; break;
		case STOPWATCH_RESET: state_str = "RESET"; break;
		default: state_str = "UNKNOWN"; break;
	}
	
	if (need_full_refresh || strcmp(state_str, last_state_str) != 0) {
		OLED_Clear();
		OLED_ShowString(0, 0, (uint8_t*)"=== Stopwatch ===", 16);
		OLED_ShowString(20, 2, (uint8_t*)sw.display_str, 16);
		OLED_ShowString(0, 4, (uint8_t*)state_str, 12);
		OLED_ShowString(0, 6, (uint8_t*)"5:Start/Pause 0:Reset", 12);
		
		need_full_refresh = 0;
	} else {
		if (strcmp(sw.display_str, last_display_str) != 0) {
			//OLED_ShowString(20, 2, (uint8_t*)"        ", 16);
			OLED_ShowString(20, 2, (uint8_t*)sw.display_str, 16);
		}
	}
	
	strcpy(last_display_str, sw.display_str);
	strcpy(last_state_str, state_str);
}

void Stopwatch_RequestFullRefresh(void)
{
	need_full_refresh = 1;
}

/**
 * @brief  秒表主运行函数
 * @param  key: 按键值
 */
void Stopwatch_Run(uint8_t key)
{
	if (key == 'U' || key == 'L') { // 返回菜单
		Stopwatch_ReturnToMenu();
		return;
	} else {
		Stopwatch_ProcessKey(key);
		// 处理按键后立即更新显示
		Stopwatch_UpdateDisplay();
	}
}

/**
 * @brief  秒表返回菜单处理
 */
void Stopwatch_ReturnToMenu(void)
{
	// 这个函数主要是为了保持接口一致性
	// 实际的菜单切换逻辑还是在main.c中处理
}


