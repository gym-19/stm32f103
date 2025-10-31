#include "system_manager.h"
#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "delay.h"
#include "usart1.h"
#include "mkey.h"
#include "oled.h"
#include "calculator.h"
#include "stopwatch.h"
#include "dht.h"
#include "stdio.h"
#include "sg90.h"
#include "ds18b20.h"
#include "music.h"
#include "key_manager.h"
#include "calendar.h"
#include "z_mytime.h"  // 确保包含RTC时间相关头文件

// 菜单索引常量
#define MENU_CALCULATOR    0
#define MENU_STOPWATCH     1
#define MENU_DHT           2
#define MENU_DS18B20       3
#define MENU_FAN_ADJUST    4
#define MENU_CALENDAR      5



// 系统状态变量
static SystemState current_state = STATE_STARTUP;
static uint8_t selected_menu_index = 0;

// 动画控制变量
static uint32_t anim_timer = 0;
static uint8_t anim_current_frame = 0;
static uint32_t frame_counter = 0;

// 更新计数器
static uint32_t display_counter = 0;
static uint32_t dht_counter = 0;

static uint32_t ds18b20_counter = 0;  // 新增：DS18B20更新计数器
// 新增：风扇调节变量
static float fan_angle = 90.0f;       // 风扇初始角度（中间位置）
static float fan_angle2 = 90.0f;      // 第二个舵机初始角度（中间位置）

// 菜单项名称 - 在这里添加新功能
const char* menu_items[] = {
    "Calculator",
    "Stopwatch", 
    "Temp/Humid",
    "Probe Temp",    // DS18B20温度探头
		"Fan Adjust",    // 风扇调节（SG90）
	  "Calendar"
};

// 新增：获取菜单项数量
uint8_t Get_Menu_Count(void) {
    return sizeof(menu_items) / sizeof(menu_items[0]);
}

void System_Init(void)
{
	// 外设初始化
	DateTimeTypeDef init_time;
	led_init();
	//buzzer_init();
	delay_init();
	usart_1_init();
	MatrixKey_Init();
	//beep_init();
	OLED_Init();
	OLED_Clear();
	RTC_InitTimestamp();
	
	// 功能模块初始化
	Calculator_Init();
	Stopwatch_Init();
	dht_init();
	ds18b20_init();  // 初始化DS18B20温度传感器
	SG90_Init();     // 初始化SG90舵机
	Calendar_Init();
	printf("=== dog case ===\r\n");
	
	// 显示开机画面
	picture_1();
	
	
    init_time.year = 2025;    // 年
    init_time.month = 10;      // 月（1-12）
    init_time.day = 13;       // 日（1-31）
    init_time.hour = 22;      // 时（0-23）
    init_time.minute = 44;    // 分（0-59）
    init_time.second = 0;     // 秒（0-59）
    
    // 设置RTC时间
    RTC_SetDateTime(&init_time);
	
}

void System_Update(void)
{
	// 开机动画处理
	if(current_state == STATE_STARTUP) {
		frame_counter += 10;
		if(frame_counter - anim_timer >= 300) {
			anim_timer = frame_counter;
			anim_current_frame = (anim_current_frame + 1) % 3;
			switch(anim_current_frame) {
				case 0: picture_2(); break;
				case 1: picture_1(); break;
				case 2: picture_3(); break;
			}
		}
	}
	
	// 秒表显示更新
	if(current_state == SYS_STOPWATCH) {
		display_counter += 10;
		if(display_counter >= 100) {
			Stopwatch_UpdateDisplay();
			display_counter = 0;
		}
	} else {
		display_counter = 0;
	}
	
	// DHT11数据更新
	if(current_state == SYS_DHT) {
		dht_counter += 10;
		if(dht_counter >= 2000) {
			DHT_UpdateDisplay();
			dht_counter = 0;
		}
	}
	// 新增：DS18B20温度周期性更新（2秒一次）
	if(current_state == SYS_DS18B20) {
		ds18b20_counter += 10;
		if(ds18b20_counter >= 2000) {
			DS18B20_UpdateDisplay();
			ds18b20_counter = 0;
		}
	}
	
	 if(current_state == SYS_CALENDAR) {
        static uint32_t calendar_counter = 0;
        calendar_counter += 10;
        if(calendar_counter >= 1000) {
            Calendar_RefreshCurrentDate();  // 更新当前日期
            Calendar_UpdateDisplay();       // 更新显示
            calendar_counter = 0;
        }
    }
}

void Change_State(SystemState new_state)
{
    current_state = new_state;
}

void Show_Main_Menu(uint8_t selected_index)
{
    uint8_t menu_count = Get_Menu_Count();
    int i;
    int display_index;
    uint8_t y_pos;
    char buf[32];
	
    selected_menu_index = selected_index;
    
    //OLED_Clear();
    OLED_ShowString(0, 0, (uint8_t*)"Main Menu", 16);
    
    // 动态显示3个菜单项（当前选中项及其前后各一项）
    for (i = 0; i < 3; i++) {
        // 计算要显示的菜单项索引（循环处理）
        display_index = (selected_index + i - 1 + menu_count) % menu_count;
        y_pos = 2 + i * 2;
        
        if (i == 1) {  // 中间项是选中项
            snprintf(buf, sizeof(buf), "> %s", menu_items[display_index]);
        } else {
            snprintf(buf, sizeof(buf), "  %s", menu_items[display_index]);
        }
        OLED_ShowString(0, y_pos, (uint8_t*)buf, 16);
    }
}
// 新增：DS18B20温度显示更新
void DS18B20_UpdateDisplay(void)
{
	u8 ds18b20_data[2];  // 存储温度数据（高8位和低8位）
	float temp;          // 转换后的温度值
	char temp_str[16];   // 显示字符串
	
	// 读取DS18B20数据
	get_ds18b20_value(ds18b20_data);
	
	// 转换温度数据（DS18B20精度为0.0625℃）
	temp = (ds18b20_data[1] << 8 | ds18b20_data[0]) * 0.0625f;
	
	// 显示温度
	//OLED_Clear();
	OLED_ShowString(0, 0, (uint8_t*)"Probe Temp:", 16);
	sprintf(temp_str, "%.2f C", temp);  // 保留两位小数
	OLED_ShowString(0, 2, (uint8_t*)temp_str, 16);
	OLED_ShowString(0, 6, (uint8_t*)"U:Back", 16);  // 提示返回
}

// 新增：DS18B20状态按键处理
void DS18B20_Run(uint8_t key)
{
	// 按'U'或'L'返回主菜单
	if(key == 'U' || key == 'L') {
		Change_State(SYS_MENU);
		Show_Main_Menu(3);  // 返回时选中当前菜单项
	}
	// 按'5'手动刷新温度
	else if(key == '5') {
		DS18B20_UpdateDisplay();
	}
}

void Fan_Adjust_UpdateDisplay(void)
{
    char angle1_str[16];  // 第一个舵机信息
    char angle2_str[16];  // 第二个舵机信息
    
    //OLED_Clear();
    // 第一行：精简标题
    OLED_ShowString(0, 0, (uint8_t*)"Fan Ctrl", 16);  
    
    // 第二行：第一个舵机角度+操作提示（用ASCII字符替换特殊符号）
    sprintf(angle1_str, "1:%.0fo 2^ 8v", fan_angle);  // o代替°，^代替↑，v代替↓
    OLED_ShowString(0, 2, (uint8_t*)angle1_str, 16);
    
    // 第三行：第二个舵机角度+操作提示
    sprintf(angle2_str, "2:%.0fo 1^ 7v", fan_angle2);
    OLED_ShowString(0, 4, (uint8_t*)angle2_str, 16);
    
    // 第四行：返回提示
    OLED_ShowString(0, 6, (uint8_t*)"U:Back", 16);
}

void Fan_Adjust_Run(uint8_t key)
{
    switch(key) {
        // 第一个舵机控制（原逻辑保留）
        case '2':  // 上键：增加第一个舵机角度（0-180°）
            fan_angle += 10.0f;
            if(fan_angle > 180.0f) fan_angle = 180.0f;
            SG90_SetAngle1(fan_angle);  // 控制第一个舵机
            Fan_Adjust_UpdateDisplay();
            break;
        case '8':  // 下键：减少第一个舵机角度（0-180°）
            fan_angle -= 10.0f;
            if(fan_angle < 0.0f) fan_angle = 0.0f;
            SG90_SetAngle1(fan_angle);  // 控制第一个舵机
            Fan_Adjust_UpdateDisplay();
            break;
        
        // 新增：第二个舵机控制
        case '1':  // 按键1：增加第二个舵机角度（0-180°）
            fan_angle2 += 10.0f;
            if(fan_angle2 > 180.0f) fan_angle2 = 180.0f;
            SG90_SetAngle2(fan_angle2);  // 控制第二个舵机
            Fan_Adjust_UpdateDisplay();
            break;
        case '7':  // 按键7：减少第二个舵机角度（0-180°）
            fan_angle2 -= 10.0f;
            if(fan_angle2 < 0.0f) fan_angle2 = 0.0f;
            SG90_SetAngle2(fan_angle2);  // 控制第二个舵机
            Fan_Adjust_UpdateDisplay();
            break;
        
        // 返回主菜单（原逻辑保留）
        case 'U':
        case 'L':  
            Change_State(SYS_MENU);
            Show_Main_Menu(4);  // 返回时选中风扇调节菜单
            break;
    }
}



void DHT_UpdateDisplay(void)
{
	u8 dht_data[5];
	u8 checksum;
	char temp_str[16];
	char hum_str[16];
	
	// 读取DHT11数据
	get_dht_value(dht_data);
	
	// 数据校验
	checksum = dht_data[0] + dht_data[1] + dht_data[2] + dht_data[3];
	if(checksum != dht_data[4]) {
		OLED_Clear();
		OLED_ShowString(0, 0, (uint8_t*)"DHT11 Error!", 16);
		OLED_ShowString(0, 2, (uint8_t*)"Check connection", 16);
		return;
	}
	
	// 清屏并显示温湿度数据
	//OLED_Clear();
	
	// 温度显示
	sprintf(temp_str, "Temp:%d.%dC", dht_data[2], dht_data[3]);
	OLED_ShowString(0, 0, (uint8_t*)temp_str, 16);
	
	// 湿度显示
	sprintf(hum_str, "Hum: %d.%d%%", dht_data[0], dht_data[1]);
	OLED_ShowString(0, 2, (uint8_t*)hum_str, 16);
}

void DHT_Run(uint8_t key)
{
	if(key == '5') {
		OLED_Clear();
		DHT_UpdateDisplay();
	}
}

// 提供给按键管理器使用的函数
SystemState Get_Current_State(void)
{
	return current_state;
}

uint8_t Get_Selected_Menu_Index(void)
{
	return selected_menu_index;
}

void Set_Selected_Menu_Index(uint8_t index)
{
	selected_menu_index = index;
}

