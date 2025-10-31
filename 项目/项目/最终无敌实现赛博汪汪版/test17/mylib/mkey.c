#include "mkey.h"
#include "delay.h"
#include "bitband.h"

#define LONG_PRESS_THRESHOLD_MS 1000
#define SCAN_CYCLE_MS 10

// 行线引脚定义（PA4-PA7，输出）
#define ROW0    4   // PA4
#define ROW1    5   // PA5
#define ROW2    6   // PA6
#define ROW3    7   // PA7

// 列线引脚定义（输入）
#define COL0    15  // PA15
#define COL1    13  // PC13
#define COL2    4   // PC4
#define COL3    5   // PC5

// 按键映射表
const uint8_t KeyMap[4][4] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', '*'},
    {'1', '2', '3', '-'},
    {'#', '0', '.', '+'}
};

// 初始化矩阵键盘
void MatrixKey_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	// 使能时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	
	// 禁用JTAG，释放PA15
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	// 配置行线（PA4-PA7，推挽输出）
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 初始时所有行线置高（使用位带操作）
	PAOut(ROW0) = 1;
	PAOut(ROW1) = 1;
	PAOut(ROW2) = 1;
	PAOut(ROW3) = 1;
	
	// 配置列线0（PA15，上拉输入）
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	// 配置列线1（PC13，上拉输入）
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// 配置列线2（PC4，上拉输入）
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	// 配置列线3（PC5，上拉输入）
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
}

// 扫描按键（使用位带操作优化）
uint8_t MatrixKey_Scan(void) {
	uint32_t timeout = 0;
	static uint8_t last_key = 0;
	uint8_t current_key = 0;
	uint8_t row, col;
	
	// === 新增：用于'#'键长按检测的状态机变量 ===
	static uint8_t hash_key_pressing = 0; // 标记'#'键是否正在被按下
	static uint16_t hash_press_duration = 0; // 记录'#'键按下的持续时间
	
	// 初始化：所有行置高，准备扫描
	PAOut(ROW0) = 1;
	PAOut(ROW1) = 1;
	PAOut(ROW2) = 1;
	PAOut(ROW3) = 1;
	
	for (row = 0; row < 4; row++) {
		// 将当前行拉低，其他行保持高
		switch(row) {
			case 0: PAOut(ROW0) = 0; break;
			case 1: PAOut(ROW1) = 0; break;
			case 2: PAOut(ROW2) = 0; break;
			case 3: PAOut(ROW3) = 0; break;
		}
		delay_us(50); // 小延时等待信号稳定
		
		for (col = 0; col < 4; col++) {
			uint8_t col_state = 0;
			// 读取列线状态
			switch(col) {
				case 0: col_state = PAIn(COL0); break;
				case 1: col_state = PCIn(COL1); break;
				case 2: col_state = PCIn(COL2); break;
				case 3: col_state = PCIn(COL3); break;
			}
			
			// 如果检测到列线为低电平（按键按下）
			if (col_state == 0) {
				delay_ms(15); // 延时去抖[1](@ref)
				
				// 再次检查确认按键是否真的按下
				switch(col) {
					case 0: col_state = PAIn(COL0); break;
					case 1: col_state = PCIn(COL1); break;
					case 2: col_state = PCIn(COL2); break;
					case 3: col_state = PCIn(COL3); break;
				}
				
				if (col_state == 0) { // 确认按键按下
					current_key = KeyMap[row][col];
					
					// === 新增：核心逻辑 - 对'#'键进行特殊处理 ===
					if (current_key == '#') {
						if (!hash_key_pressing) {
							// 首次检测到'#'键按下，开始计时
							hash_key_pressing = 1;
							hash_press_duration = 0;
						} else {
							// '#'键持续按下，增加计时
							hash_press_duration += SCAN_CYCLE_MS; // 累加一个扫描周期的时间
							// 检查是否达到长按阈值
							if (hash_press_duration >= LONG_PRESS_THRESHOLD_MS) {
								// 触发长按事件，返回特殊标识'L'
								hash_key_pressing = 0; // 重置状态
								hash_press_duration = 0;
								last_key = 0;
								return 'L'; // 返回长按信号
							}
						}
					} else {
						// 如果按下的不是'#'键，则确保长按状态被重置
						hash_key_pressing = 0;
						hash_press_duration = 0;
					}
					// === 新增代码结束 ===
					
					// 等待按键释放（您原有的逻辑）
					timeout = 0;
					do {
						delay_ms(1);
						timeout++;
						switch(col) {
							case 0: col_state = PAIn(COL0); break;
							case 1: col_state = PCIn(COL1); break;
							case 2: col_state = PCIn(COL2); break;
							case 3: col_state = PCIn(COL3); break;
						}
					} while (col_state == 0 && timeout < 200);
					
					if (timeout < 200) { // 正常释放
						delay_ms(10); // 释放去抖
						
						// === 新增：按键释放时的处理 ===
						if (current_key == '#' && hash_key_pressing) {
							// '#'键被释放，且未触发长按，则认为是短按
							hash_key_pressing = 0; // 重置状态
							if (hash_press_duration < LONG_PRESS_THRESHOLD_MS) {
								// 返回短按的'#'键值
								if (current_key != last_key) {
									last_key = current_key;
									return current_key;
								}
							}
							// 如果已经触发过长按，这里什么都不返回，避免短按长按都触发
							hash_press_duration = 0;
							return 0;
						}
						// === 新增代码结束 ===
						
						// 对于非'#'键，保持原有逻辑
						if (current_key != last_key) {
							last_key = current_key;
							return current_key;
						}
					}
					// 超时或异常，重置状态
					last_key = 0;
					// === 新增：任何按键异常释放都重置长按状态 ===
					hash_key_pressing = 0;
					hash_press_duration = 0;
					// === 新增代码结束 ===
					return 0;
				}
			}
		}
		// 扫描下一行前，将当前行恢复为高电平
		switch(row) {
			case 0: PAOut(ROW0) = 1; break;
			case 1: PAOut(ROW1) = 1; break;
			case 2: PAOut(ROW2) = 1; break;
			case 3: PAOut(ROW3) = 1; break;
		}
	}
	
	// === 新增：如果一轮扫描结束都没有检测到任何按键，确保长按状态被重置 ===
	hash_key_pressing = 0;
	hash_press_duration = 0;
	// === 新增代码结束 ===
	
	last_key = 0;
	return 0; // 没有按键按下
}



