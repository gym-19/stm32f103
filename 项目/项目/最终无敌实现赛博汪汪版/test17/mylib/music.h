#ifndef __MUSIC_H
#define __MUSIC_H

#include <stdint.h>

//音乐功能结构体声明
typedef struct {
    void (*Sound)(uint16_t frq);
    void (*play_music)(void);
} Music_t;


extern Music_t Music;

// 函数声明
void Sound(uint16_t frq);
void play_music(void);
void beep_init(void);
void beep_on(void);
void beep_off(void);
#endif 
