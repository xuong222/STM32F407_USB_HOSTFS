#include "stm32f4xx.h" 
#include "Key.h"
#include "delay.h"
#include "Timer_Driver.h"

#define Key1 	 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define Key2     GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13)

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOA 和 GPIOC 时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    /* PA0 输入*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;       // 输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;    
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* PC13 输入*/
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;      
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief  按键扫描函数
 * @return 0 = 无按键按下
 *         1 = PA0 按键按下
 *         2 = PC13 按键按下
 *         3 = 两个按键同时按下
 */
uint8_t KEY_Scan(void)
{
    static uint8_t key_up = 1;   // 按键松开标志位，防止连续触发

    if ((Key1==Bit_SET || Key2==Bit_SET) && key_up)
    {
        delay_ms(20);            // 消抖
        key_up = 0;

        if (Key1 == Bit_SET)	return 1; // PA0 被按下
        if (Key2 == Bit_SET)	return 2; // PC13 被按下
    }

    // 按键全部松开后，允许下一次触发
    if (Key1 == Bit_RESET && Key2 == Bit_RESET)
    {
        key_up = 1;
    }

    return 0;
}

