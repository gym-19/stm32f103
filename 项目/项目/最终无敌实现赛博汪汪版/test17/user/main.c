#include "stm32f10x.h"
#include "system_manager.h"
#include "key_manager.h"
#include "delay.h"
#include "music.h"
#include "button.h"

int main(void)
{
    // 系统初始化
    System_Init();
		beep_init();  // 确保蜂鸣器初始化
		play_music();
		if (button_status(2) == 1) 
			{
					//Ybeep_off();
					delay_ms(1000);
			}
    
    // 主循环
    while(1)
    {
        // 按键处理
        Key_Process();
        
        // 系统状态更新
        System_Update();
        
        delay_ms(10);
    }
}

