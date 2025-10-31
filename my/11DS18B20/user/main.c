#include "led.h"
#include "beep.h"
#include "key.h"
#include "delay.h"
#include "ldt.h"
#include "dht.h"
#include "ds18b20.h"

int16_t ds18b20_to_temp_x10(uint8_t lo, uint8_t hi)
{
    int16_t raw = (hi << 8) | lo;  // ?????,?????
    int16_t temp;
    
    // DS18B20?12???????
    if (raw & 0x8000)  // ????
    {
        raw = ~raw + 1;  // ???
        temp = -( (raw * 625) / 1000 );  // ???0.1???
    }
    else  // ????
    {
        temp = (raw * 625) / 1000;  // 625 = 10000/16,???0.1???
    }
    
    return temp;
}

int main(void)
{
    u8 ds[2];
    int16_t temp;
		u8 init_flag;
    delay_init();
    key_init();
    led_init();
    ldt_init();
   
		init_flag = ds18b20_init();
    if(init_flag != 0)
    {
   
        while(1)
        {
            
            led_on(0);
            delay_ms(500);
						led_off(0);
        }
    }
    while (1)
    {
        get_ds18b20_value(ds);
        temp = ds18b20_to_temp_x10(ds[0], ds[1]);
        digit_show_data((int)temp);  // ??? -125 ? "-12.5"
    }
}



