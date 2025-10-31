#ifndef __BITBAND_H_
#define __BITBAND_H_

#define BIT_BAND(ADDR, BITNUM) ((ADDR & 0xF0000000) + 0x2000000 + ((ADDR & 0xFFFFF) << 5) + (BITNUM << 2))
#define MEM_ADDR(ADDR)  *(volatile unsigned int *)(ADDR)
	
#define BITBAND(ADDR, BITNUM) MEM_ADDR(BIT_BAND(ADDR, BITNUM))
#define GPIOA_IDR (GPIOA_BASE + 0x08)
#define GPIOA_ODR (GPIOA_BASE + 0x0C)
#define GPIOB_IDR (GPIOB_BASE + 0x08)
#define GPIOB_ODR (GPIOB_BASE + 0x0C)
#define GPIOC_IDR (GPIOC_BASE + 0x08)
#define GPIOC_ODR (GPIOC_BASE + 0x0C)
#define GPIOD_IDR (GPIOD_BASE + 0x08)
#define GPIOD_ODR (GPIOD_BASE + 0x0C)


#define PAIn(N) BITBAND(GPIOA_IDR, N)
#define PAOut(N) BITBAND(GPIOA_ODR, N)
#define PBIn(N) BITBAND(GPIOB_IDR, N)
#define PBOut(N) BITBAND(GPIOB_ODR, N)
#define PCIn(N) BITBAND(GPIOC_IDR, N)
#define PCOut(N) BITBAND(GPIOC_ODR, N)
#define PDIn(N) BITBAND(GPIOD_IDR, N)
#define PDOut(N) BITBAND(GPIOD_ODR, N)

#endif





