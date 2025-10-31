#include "menu.h"
#include "oled.h"
#include "calculator.h"
#include "stopwatch.h"
#include <string.h>

MenuSystem menu;
// 修改菜单项为更有趣的描述
const char* menu_items[] = {"1. dogjisuan", "2. dogshushu"};

void Menu_Init(void) {
    menu.current_state = MENU_STATE;
    menu.menu_selection = 0;
    menu.menu_item_count = 2;
}

void Menu_ProcessKey(uint8_t key) {
    switch(key) {
        case '8': // 上键
            menu.menu_selection--;
            if (menu.menu_selection < 0) 
                menu.menu_selection = menu.menu_item_count - 1;
            break;
            
        case '2': // 下键
            menu.menu_selection++;
            if (menu.menu_selection >= menu.menu_item_count) 
                menu.menu_selection = 0;
            break;
            
        case '5': // 确定键
            if (menu.menu_selection == 0) {
                menu.current_state = CALCULATOR_STATE;
                Calculator_Init();
            } else if (menu.menu_selection == 1) {
                menu.current_state = STOPWATCH_STATE;
                Stopwatch_Init();
            }
            break;
            
        case 'U': // 返回键
            menu.current_state = MENU_STATE;
            break;
    }
}

void Menu_UpdateDisplay(void) {
    OLED_Clear();
    OLED_ShowString(0, 0, "=== doghome ===", 16);
    
    for (int i = 0; i < menu.menu_item_count; i++) {
        if (i == menu.menu_selection) {
            OLED_ShowString(0, 2 + i * 2, ">", 16);
        }
        OLED_ShowString(10, 2 + i * 2, (u8*)menu_items[i], 16);
    }
    
    OLED_ShowString(0, 6, "8up2dw5yes", 12);
}

AppState Menu_GetCurrentState(void) {
    return menu.current_state;
}

void Menu_SetCurrentState(AppState state) {
    menu.current_state = state;
}