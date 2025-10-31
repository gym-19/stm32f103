#include "key_manager.h"
#include "led.h"
#include "delay.h"
#include "calculator.h"
#include "stopwatch.h"
#include "stdio.h" 
#include "calendar.h"


// 在key_manager.c中添加状态映射
static SystemState menu_index_to_state[] = {
    SYS_CALCULATOR,   // 索引0
    SYS_STOPWATCH,    // 索引1
    SYS_DHT,          // 索引2
    SYS_DS18B20,      // 索引3
    SYS_FAN_ADJUST,   // 索引4
    SYS_CALENDAR      // 索引5
};


uint8_t Scan_Keys(void)
{
	uint8_t matrix_key = MatrixKey_Scan();
	uint8_t independent_key = Scan_Independent_Keys();
	return (independent_key != 0) ? independent_key : matrix_key;
}

void Key_Process(void)
{
		uint8_t menu_count;
    SystemState current_state;
    uint8_t selected_index;
    uint8_t current_key = Scan_Keys();
    
    if(current_key != 0)
    {
        printf("case: %c\r\n", current_key);
        
        // LED提示
        led_on(0);
        delay_ms(10);
        led_off(0);
        
        current_state = Get_Current_State();
        selected_index = Get_Selected_Menu_Index();
        menu_count = Get_Menu_Count();  // 获取动态菜单数量
        
        switch(current_state) 
        {
        case STATE_STARTUP:
            Change_State(SYS_MENU);
            OLED_Clear();
            Show_Main_Menu(0);
            break;
            
        case SYS_MENU:
            if(current_key == '5') {
                // 进入选中的功能
                Change_State(menu_index_to_state[selected_index]); // SYS_CALCULATOR = 2, 所以+2
                OLED_Clear();
                
                // 根据选中项调用相应的显示函数
                switch(selected_index) {
                case MENU_CALCULATOR:
                    Calculator_UpdateDisplay();
                    break;
                case MENU_STOPWATCH:
                    Stopwatch_UpdateDisplay();
                    break;
                case MENU_DHT:
                    DHT_UpdateDisplay();
                    break;
                case MENU_DS18B20:  // 新增：进入DS18B20温度探头
										Change_State(SYS_DS18B20);
										OLED_Clear();
										DS18B20_UpdateDisplay();
										break;
								case MENU_FAN_ADJUST:  // 新增：进入风扇调节
										Change_State(SYS_FAN_ADJUST);
										OLED_Clear();
										Fan_Adjust_UpdateDisplay();
										break;
                case MENU_CALENDAR:
                    Change_State(SYS_CALENDAR);
                    OLED_Clear();
                    Calendar_UpdateDisplay();
                    break;
            }
					}
            else if(current_key == '2') {  // 下键
                selected_index = (selected_index + 1) % menu_count;
                Set_Selected_Menu_Index(selected_index);
                Show_Main_Menu(selected_index);
            }
            else if(current_key == '8') {  // 上键
                selected_index = (selected_index - 1 + menu_count) % menu_count;
                Set_Selected_Menu_Index(selected_index);
                Show_Main_Menu(selected_index);
            }
            else if(current_key == 'U' || current_key == 'L') {
                Change_State(STATE_STARTUP);
            }
            break;
            
        case SYS_CALCULATOR:
            Calculator_Run(current_key);
            if (current_key == 'U' || current_key == 'L') {
                Change_State(SYS_MENU);
                Calculator_Init();
								OLED_Clear();
                Show_Main_Menu(MENU_CALCULATOR);
            }
            break;
            
        case SYS_STOPWATCH:
            Stopwatch_Run(current_key);
            if (current_key == 'U' || current_key == 'L') {
                Change_State(SYS_MENU);
                Stopwatch_Init();
								OLED_Clear();
                Show_Main_Menu(MENU_STOPWATCH);
            }
            break;
            
        case SYS_DHT:
            DHT_Run(current_key);
            if (current_key == 'U' || current_key == 'L') {
                Change_State(SYS_MENU);
								OLED_Clear();
                Show_Main_Menu(MENU_DHT);
            }
            break;
						// 新增：DS18B20状态处理
				case SYS_DS18B20:
						DS18B20_Run(current_key);
						break;
			
					// 新增：风扇调节状态处理
				case SYS_FAN_ADJUST:
						Fan_Adjust_Run(current_key);
						break;
				
        case SYS_CALENDAR:
             Calendar_Run(current_key);
             if (current_key == 'U' || current_key == 'L') {
                Change_State(SYS_MENU);
                OLED_Clear();
                Show_Main_Menu(MENU_CALENDAR);
               }
             break;
        }
    }
}
// 独立按键扫描函数
uint8_t Scan_Independent_Keys(void)
{
	static uint8_t last_key0 = 1, last_key1 = 1, last_key_up = 1;
	uint8_t current_key0, current_key1, current_key_up;
	
	current_key0 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8);
	current_key1 = GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9);
	current_key_up = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
	
	if(last_key0 && !current_key0) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)) {
			last_key0 = 0;
			return 'U';
		}
	}
	
	if(last_key1 && !current_key1) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)) {
			last_key1 = 0;
			return 'U';
		}
	}
	
	if(last_key_up && !current_key_up) {
		delay_ms(10);
		if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)) {
			last_key_up = 0;
			return 'U';
		}
	}
	
	if(!last_key0 && current_key0) last_key0 = 1;
	if(!last_key1 && current_key1) last_key1 = 1;
	if(!last_key_up && current_key_up) last_key_up = 1;
	
	return 0;
}

