#include "delay.h"

#include <stdint.h>
#include <stdio.h>
static uint32_t  fac_us=0;
static uint32_t fac_ms=0;

void delay_init()
{
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK_DIV8);
	fac_us = SystemCoreClock/8000000;
	fac_ms = (uint16_t)fac_us*1000;
}


//
//void delay_us(uint32_t nus)
//{
//	uint32_t temp;
//	SysTick->LOAD = nus*fac_us;
//	SysTick->VAL = 0x00;
//	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));
//	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//	SysTick->VAL =0X00;
//}
//


extern TIM_HandleTypeDef htim1;

void delay_us(uint32_t nus)
{
	__HAL_TIM_SET_COUNTER(&htim1,0);  // set the counter value a 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < nus) asm("");
}


void delay_ms(uint16_t nms) { for (uint32_t i = 0; i < nms; i++) { delay_us(1000); } }

//#define SYSTICK_LOAD (SystemCoreClock/1000000U)
//#define SYSTICK_DELAY_CALIB (SYSTICK_LOAD >> 1)
//
//#define DELAY_US(us) \
//    do { \
//         uint32_t start = SysTick->VAL; \
//         uint32_t ticks = (us * SYSTICK_LOAD)-SYSTICK_DELAY_CALIB;  \
//         while((start - SysTick->VAL) < ticks); \
//    } while (0)
//
//#define DELAY_MS(ms) \
//    do { \
//        for (uint32_t i = 0; i < ms; ++i) { \
//            DELAY_US(1000); \
//        } \
//    } while (0)
//
//void delay_us(uint32_t nus)
//{
//	DELAY_US(nus);
//}
//
//void delay_ms(uint16_t nms)
//{
//	DELAY_MS(nms);
//}
