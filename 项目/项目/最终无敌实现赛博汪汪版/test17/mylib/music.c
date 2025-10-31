#include "music.h"
#include "delay.h"
#include "bitband.h"

void Sound(uint16_t frq);
 void play_music(void);

// 
Music_t Music = {
    Sound,
    play_music,
};

/**
 * @brief 发出指定频率的声音
 * @param frq: 声音频率
 */
 void Sound(uint16_t frq)
{
    uint32_t halfPeriod = 500000 / frq;
    if (frq != 0) 
    {
        beep_on();
        delay_us(halfPeriod);
        beep_off();
        delay_us(halfPeriod);
    }
    else 
    {
        delay_us(halfPeriod * 2); 
    }
}

/**
 * @brief 播放预设音乐
 */
void play_music(void)
{
    uint16_t MusicalNote[] = {262, 294, 330, 349, 392, 440}; // ????
    uint8_t music[] = {0,0,4,4,5,5,4,3,3,2,2,1,1,0,0};       // ????
    uint8_t time[] = {4,4,4,4,4,4,8,4,4,4,4,4,4,8,8};        // ???????
    
    int32_t delayT = 10;
    uint16_t i, e;

    for (i = 0; i < sizeof(music) / sizeof(music[0]); i++) 
    {
        uint32_t playCount = ((uint32_t)time[i]) * MusicalNote[music[i]] / delayT;
        
        for (e = 0; e < playCount; e++) 
        {
            Sound(MusicalNote[music[i]]);
        }
        delay_ms(time[i] * 5);
    }
}

void beep_init(void)
{  
	GPIO_InitTypeDef Gpio_Value;//定义了初始化GPIO结构体类型的变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//通过APB2总线使能GPIOC组的时钟
	
	Gpio_Value.GPIO_Mode = GPIO_Mode_Out_PP;//选择了推挽的输出模式
	Gpio_Value.GPIO_Pin = GPIO_Pin_7;//选择了 7 号管脚
	Gpio_Value.GPIO_Speed = GPIO_Speed_50MHz;//选择了50MHz的输出速率
	GPIO_Init(GPIOC, &Gpio_Value);//按照上述配置初始化GPIOC组的管脚
	
	PCOut(7) = 0; 
}

void beep_on(void)
{
		PCOut(7) = 1;
}

void beep_off(void)
{
  	PCOut(7) = 0;
}
