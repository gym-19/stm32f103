#include "stm32f10x.h"
#include "led.h"
#include "buzzer.h"
#include "delay.h"
#include "usart1.h"
#include "stdio.h"
#include "mkey.h"
#include "oled.h"
#include "calculator.h"
#include "stopwatch.h"

// 系统状态定义
typedef enum {
	SYS_MENU = 0,
	SYS_CALCULATOR,
	SYS_STOPWATCH
} SystemState;

// 函数声明
uint8_t Scan_Independent_Keys(void);
void Show_Main_Menu(uint8_t menu_selection);

int main(void)
{
	// 变量声明
	SystemState current_system_state = SYS_MENU;
	SystemState last_system_state = SYS_MENU;
	uint8_t menu_selection = 0;
	uint8_t matrix_key;
	uint8_t independent_key;
	uint8_t current_key;
	static uint32_t display_counter = 0;
	const uint32_t display_update_interval = 100; // 100ms更新一次显示
	
	// 外设初始化
	led_init();
	buzzer_init();
	delay_init();
	usart_1_init();
	MatrixKey_Init();
	OLED_Init();
	OLED_Clear();
	
	// 功能模块初始化
	Calculator_Init();
	Stopwatch_Init(); // 这会初始化定时器
	
	printf("=== Multi-Function System Started ===\r\n");
	printf("Using TIM2 for precise stopwatch timing\r\n");
	
	// 显示初始菜单
	Show_Main_Menu(menu_selection);
	
	// 主循环
	while(1)
	{
		// 按键扫描
		matrix_key = MatrixKey_Scan();
		independent_key = Scan_Independent_Keys();
		current_key = (independent_key != 0) ? independent_key : matrix_key;
		
		// 状态切换检测
		if (current_system_state != last_system_state) {
			// 状态发生变化，请求相应模块的全屏刷新
			if (current_system_state == SYS_STOPWATCH) {
				Stopwatch_RequestFullRefresh(); // 请求秒表全屏刷新
			}
			last_system_state = current_system_state;
		}
		
		// 按键处理
		if(current_key != 0)
		{
			printf("Key Pressed: %c\r\n", current_key);
			
			// LED提示
			led_on(0);
			delay_ms(100);
			led_off(0);
			
			// 根据当前系统状态处理按键
			switch(current_system_state) 
			{
			case SYS_MENU:
				if(current_key == '5') { // 确认选择
					if(menu_selection == 0) {
						current_system_state = SYS_CALCULATOR;
						// 立即切换到计算器界面
						Calculator_UpdateDisplay();
					} else {
						current_system_state = SYS_STOPWATCH;
						// 立即切换到秒表界面
						Stopwatch_UpdateDisplay();
					}
				}
				else {
					// 菜单导航逻辑
					if(current_key == '8' || current_key == '2') {
						menu_selection = (menu_selection == 0) ? 1 : 0;
						Show_Main_Menu(menu_selection);
					}
				}
				break;
				
			case SYS_CALCULATOR:
				Calculator_Run(current_key);
				// 检查是否需要返回菜单
				if (current_key == 'U' || current_key == 'L') {
					current_system_state = SYS_MENU;
					Calculator_Init();
					Show_Main_Menu(menu_selection);
				}
				break;
				
			case SYS_STOPWATCH:
				Stopwatch_Run(current_key);
				// 检查是否需要返回菜单
				if (current_key == 'U' || current_key == 'L') {
					current_system_state = SYS_MENU;
					Stopwatch_Init(); // 重新初始化秒表
					Show_Main_Menu(menu_selection);
				}
				break;
			}
		}
		
		// 控制秒表显示更新频率，避免频繁刷新导致闪烁
		if(current_system_state == SYS_STOPWATCH) {
			display_counter += 10; // 每次循环延时10ms
			
			if(display_counter >= display_update_interval) {
				Stopwatch_UpdateDisplay();
				display_counter = 0;
			}
		} else {
			display_counter = 0;
		}
		
		delay_ms(10); // 主循环延时
	}
}

/**
 * @brief  显示主菜单
 * @param  menu_selection: 当前选中的菜单项
 */
void Show_Main_Menu(uint8_t menu_selection)
{
	OLED_Clear();
	OLED_ShowString(0, 0, (u8*)"=== Main Menu ===", 16);
	OLED_ShowString(0, 2, (u8*)(menu_selection == 0 ? "> 1. Calculator" : "  1. Calculator"), 16);
	OLED_ShowString(0, 4, (u8*)(menu_selection == 1 ? "> 2. Stopwatch" : "  2. Stopwatch"), 16);
	OLED_ShowString(0, 6, (u8*)"8/2:Nav 5:Select", 12);
}

/**
 * @brief  独立按键扫描函数
 * @retval 按键值，0表示无按键
 */
uint8_t Scan_Independent_Keys(void)
{
	static uint8_t last_key0 = 1, last_key1 = 1, last_key_up = 1;
	uint8_t current_key0, current_key1, current_key_up;
	
	// 读取当前状态（非阻塞）
	current_key0 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	current_key1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	current_key_up = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	
	// KEY0 按下检测（下降沿）
	if(last_key0 && !current_key0) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)) {
			last_key0 = 0;
			return 'U';
		}
	}
	
	// KEY1 按下检测
	if(last_key1 && !current_key1) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)) {
			last_key1 = 0;
			return 'U';
		}
	}
	
	// KEY_UP 按下检测
	if(last_key_up && !current_key_up) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
			last_key_up = 0;
			return 'U';
		}
	}
	
	// 按键释放检测
	if(!last_key0 && current_key0) last_key0 = 1;
	if(!last_key1 && current_key1) last_key1 = 1;
	if(!last_key_up && current_key_up) last_key_up = 1;
	
	return 0;
}


