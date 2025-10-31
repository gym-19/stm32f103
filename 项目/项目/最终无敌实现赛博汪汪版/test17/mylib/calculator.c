#include "calculator.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "oled.h"

Calculator calc;

// 静态函数声明 - 放在文件开头
static void handle_digit_input(uint8_t digit);
static void handle_decimal_input(void);
static void handle_operator_input(char op);
static void handle_equals_input(void);
static void handle_clear_input(void);

static uint8_t need_full_refresh = 1;
static char last_display[32] = "";
static char last_expression[64] = "";
static char last_state_str[32] = "";
static char last_op_str[2] = "";
/**
 * @brief  计算器初始化
 */
void Calculator_Init(void)
{
	memset(&calc, 0, sizeof(Calculator));
	calc.state = CALC_STATE_INPUT_FIRST;
	strcpy(calc.display, "0");
	strcpy(calc.expression, "");
	calc.has_decimal = 0;
	calc.decimal_digits = 0;
	calc.is_negative = 0;
}

/**
 * @brief  处理数字输入
 * @param  digit: 数字(0-9)
 */
static void handle_digit_input(uint8_t digit)
{
    char temp_str[2];
    
    // 如果当前显示的是结果或错误，重新开始
    if (calc.state == CALC_STATE_SHOW_RESULT || calc.state == CALC_STATE_ERROR) {
        Calculator_Init();
        need_full_refresh = 1; // 需要全屏刷新
    }
    
    // 如果当前是运算符状态，开始输入第二个数字
    if (calc.state == CALC_STATE_INPUT_OP) {
        calc.state = CALC_STATE_INPUT_SECOND;
        strcpy(calc.display, "0");  // 确保从0开始
        calc.has_decimal = 0;
        calc.decimal_digits = 0;
        calc.is_negative = 0;
        
        // 不需要全屏刷新，只需要局部刷新显示区域
        printf("Starting second number input: state=%d\n", calc.state);
    }
    
    // 避免多个前导零
    if (strcmp(calc.display, "0") == 0 && digit == 0 && !calc.has_decimal) {
        return;
    }
    
    // 限制输入长度
    if (strlen(calc.display) < 15) {
        if (strcmp(calc.display, "0") == 0 && digit != 0) {
            sprintf(calc.display, "%d", digit);
        } else {
            temp_str[0] = '0' + digit;
            temp_str[1] = '\0';
            if (strlen(calc.display) + 1 < sizeof(calc.display)) {
                strcat(calc.display, temp_str);
            }
        }
    }
    
    printf("Digit input: digit=%d, display=%s\n", digit, calc.display);
}


/**
 * @brief  处理小数点输入
 */
static void handle_decimal_input(void)
{
	if (calc.state == CALC_STATE_SHOW_RESULT || calc.state == CALC_STATE_ERROR) {
		Calculator_Init();
	}
	
	if (calc.state == CALC_STATE_INPUT_OP) {
		calc.state = CALC_STATE_INPUT_SECOND;
		strcpy(calc.display, "0");
		calc.has_decimal = 0;
		calc.decimal_digits = 0;
	}
	
	if (!calc.has_decimal && strlen(calc.display) < 14) {
		if (strchr(calc.display, '.') == NULL) {
			strcat(calc.display, ".");
			calc.has_decimal = 1;
			calc.decimal_digits = 0;
		}
	}
}

/**
 * @brief  处理运算符输入
 * @param  op: 运算符
 */
static void handle_operator_input(char op)
{
    if (calc.state == CALC_STATE_INPUT_FIRST || calc.state == CALC_STATE_SHOW_RESULT) {
        // 保存第一个操作数
        calc.operand1 = atof(calc.display);
        calc.current_op = op;
        calc.state = CALC_STATE_INPUT_OP;
        
        // 请求全屏刷新（因为状态和表达式都改变了）
        need_full_refresh = 1;
        
        // 更新表达式
        sprintf(calc.expression, "%.10g %c", calc.operand1, calc.current_op);
        
        printf("Operator input: state=%d, op=%c\n", calc.state, op);
    }
    else if (calc.state == CALC_STATE_INPUT_SECOND) {
        // 连续运算
        handle_equals_input();
        if (calc.state != CALC_STATE_ERROR) {
            calc.operand1 = calc.result;
            calc.current_op = op;
            calc.state = CALC_STATE_INPUT_OP;
            
            // 请求全屏刷新
            need_full_refresh = 1;
            
            sprintf(calc.expression, "%.10g %c", calc.operand1, calc.current_op);
        }
    }
}



/**
 * @brief  处理等号输入
 */
static void handle_equals_input(void)
{
	if (calc.state == CALC_STATE_INPUT_SECOND) {
		calc.operand2 = atof(calc.display);
		
		// 执行计算
		switch(calc.current_op) {
		case '+':
			calc.result = calc.operand1 + calc.operand2;
			break;
		case '-':
			calc.result = calc.operand1 - calc.operand2;
			break;
		case '*':
			calc.result = calc.operand1 * calc.operand2;
			break;
		case '/':
			if (calc.operand2 != 0) {
				calc.result = calc.operand1 / calc.operand2;
			} else {
				strcpy(calc.display, "Error:Div by 0");
				calc.state = CALC_STATE_ERROR;
				return;
			}
			break;
		default:
			return;
		}
		
		// 格式化结果显示
		if (fabs(calc.result) > 1e10 || (fabs(calc.result) < 1e-10 && calc.result != 0)) {
			sprintf(calc.display, "%.6e", calc.result);
		} else {
			sprintf(calc.display, "%.10g", calc.result);
		}
		
		// 更新完整表达式
		sprintf(calc.expression, "%.10g %c %.10g =", 
				calc.operand1, calc.current_op, calc.operand2);
		
		calc.state = CALC_STATE_SHOW_RESULT;
	}
}

/**
 * @brief  处理清除输入
 */
static void handle_clear_input(void)
{
	Calculator_Init();
}

/**
 * @brief  处理按键输入
 * @param  key: 按键值
 */
void Calculator_ProcessKey(uint8_t key)
{
	switch(key) {
	case '0': case '1': case '2': case '3': case '4':
	case '5': case '6': case '7': case '8': case '9':
		handle_digit_input(key - '0');
		break;
		
	case '.':
		handle_decimal_input();
		break;
		
	case '+': case '-': case '*': case '/':
		handle_operator_input(key);
		break;
		
		case '#':  // 等号键
		handle_equals_input();
		break;
		
		case 'C':  // 清除键（如果映射了C键）
		handle_clear_input();
		break;
	}
}

/**
 * @brief  获取显示字符串
 * @return 显示字符串指针
 */
const char* Calculator_GetDisplay(void)
{
	return calc.display;
}

/**
 * @brief  获取表达式字符串
 * @return 表达式字符串指针
 */
const char* Calculator_GetExpression(void)
{
	return calc.expression;
}

/**
 * @brief  获取当前状态
 * @return 当前状态
 */
CalcState Calculator_GetState(void)
{
	return calc.state;
}

/**
 * @brief  获取当前运算符
 * @return 当前运算符
 */
char Calculator_GetCurrentOp(void)
{
	return calc.current_op;
}

/**
 * @brief  计算器主运行函数
 * @param  key: 按键值
 */
void Calculator_Run(uint8_t key)
{
	if (key == 'U' || key == 'L') { // 返回菜单
		Calculator_ReturnToMenu();
		return;
	} else {
		// 处理计算器按键
		Calculator_ProcessKey(key);
		// 更新计算器显示
		Calculator_UpdateDisplay();
	}
}

/**
 * @brief  计算器显示更新函数
 */
void Calculator_UpdateDisplay(void)
{
    const char* display_str;
    const char* expr_str;
    CalcState state;
    char current_op;
    const char* state_str;
    char op_str[2];
    
    // 获取计算器数据
    display_str = Calculator_GetDisplay();
    expr_str = Calculator_GetExpression();
    state = Calculator_GetState();
    current_op = Calculator_GetCurrentOp();
    
    // 根据状态获取状态字符串
    state_str = "";
    switch(state) {
        case CALC_STATE_INPUT_FIRST: state_str = "Input 1st Number"; break;
        case CALC_STATE_INPUT_OP: state_str = "Input Operator"; break;
        case CALC_STATE_INPUT_SECOND: state_str = "Input 2nd Number"; break;
        case CALC_STATE_SHOW_RESULT: state_str = "Show Result"; break;
        case CALC_STATE_ERROR: state_str = "Error"; break;
    }
    
    op_str[0] = current_op;
    op_str[1] = '\0';
    
    // 如果需要全屏刷新
    if (need_full_refresh || 
        strcmp(expr_str, last_expression) != 0 ||
        strcmp(state_str, last_state_str) != 0) {
        
        OLED_Clear();
        
        // 第一行：显示表达式
        OLED_ShowString(0, 0, (uint8_t*)expr_str, 12);
        
        // 第二行：显示当前输入或结果
        OLED_ShowString(0, 2, (uint8_t*)display_str, 16);
        
        // 第三行：显示当前运算符（如果有）
        if (state == CALC_STATE_INPUT_OP || state == CALC_STATE_INPUT_SECOND) {
            OLED_ShowString(110, 2, (uint8_t*)op_str, 16);
        }
        
        // 第四行：显示状态信息
        OLED_ShowString(0, 4, (uint8_t*)state_str, 12);
        
        // 更新最后显示的内容
        strcpy(last_display, display_str);
        strcpy(last_expression, expr_str);
        strcpy(last_state_str, state_str);
        strcpy(last_op_str, op_str);
        
        need_full_refresh = 0;
    } 
    // 只需要更新主显示区域
    else if (strcmp(display_str, last_display) != 0) {
        // 只更新主显示区域
        OLED_ShowString(0, 2, (uint8_t*)"                ", 16); // 清除区域
        OLED_ShowString(0, 2, (uint8_t*)display_str, 16); // 显示新内容
        
        // 更新运算符显示（如果需要）
        if (state == CALC_STATE_INPUT_OP || state == CALC_STATE_INPUT_SECOND) {
            OLED_ShowString(110, 2, (uint8_t*)" ", 16); // 清除运算符位置
            OLED_ShowString(110, 2, (uint8_t*)op_str, 16); // 显示运算符
        }
        
        strcpy(last_display, display_str);
        strcpy(last_op_str, op_str);
    }
    
    printf("Display update: expr=%s, display=%s, state=%d\n", expr_str, display_str, state);
}

void Calculator_RequestFullRefresh(void)
{
    need_full_refresh = 1;
}

/**
 * @brief  计算器返回菜单处理
 */
void Calculator_ReturnToMenu(void)
{
	// 这个函数主要是为了保持接口一致性
	// 实际的菜单切换逻辑还是在main.c中处理
}

