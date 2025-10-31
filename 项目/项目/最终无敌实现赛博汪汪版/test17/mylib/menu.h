#ifndef __MENU_H
#define __MENU_H

#include "stm32f10x_conf.h" 

// 应用状态
typedef enum {
    MENU_STATE,
    CALCULATOR_STATE,
    STOPWATCH_STATE
} AppState;

// 菜单结构体
typedef struct {
    AppState current_state;
    int8_t menu_selection;
    uint8_t menu_item_count;
} MenuSystem;

// 函数声明
void Menu_Init(void);
void Menu_ProcessKey(uint8_t key);
void Menu_UpdateDisplay(void);
AppState Menu_GetCurrentState(void);
void Menu_SetCurrentState(AppState state);

extern MenuSystem menu;

#endif