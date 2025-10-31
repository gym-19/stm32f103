#ifndef __CALCULATOR_H
#define __CALCULATOR_H

#include "stm32f10x.h"

// 计算器状态定义
typedef enum {
	CALC_STATE_INPUT_FIRST = 0,
	CALC_STATE_INPUT_OP,
	CALC_STATE_INPUT_SECOND,
	CALC_STATE_SHOW_RESULT,
	CALC_STATE_ERROR
} CalcState;

// 计算器结构体
typedef struct {
	double operand1;
	double operand2;
	double result;
	char current_op;
	char display[32];
	char expression[64];
	CalcState state;
	uint8_t has_decimal;
	uint8_t decimal_digits;
	uint8_t is_negative;
} Calculator;

// 函数声明
void Calculator_Init(void);
void Calculator_ProcessKey(uint8_t key);
const char* Calculator_GetDisplay(void);
const char* Calculator_GetExpression(void);
CalcState Calculator_GetState(void);
char Calculator_GetCurrentOp(void);
void Calculator_UpdateDisplay(void);  // 修改这个函数，而不是重新定义
void Calculator_Run(uint8_t key);
void Calculator_ReturnToMenu(void);
void Calculator_RequestFullRefresh(void);

#endif
