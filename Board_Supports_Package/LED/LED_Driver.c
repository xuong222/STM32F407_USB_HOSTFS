#include "LED_Driver.h"

/********************************************************************************
* 函数名 : LED_PortInit
* 功  能 : LED初始化
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.07.06
* 修  改 : none                       时  间 : none
********************************************************************************/
void LED_PortInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;        //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);         //初始化对应GPIO
    
    //低电平点亮，默认先全部熄灭(输出高电平)
    GPIO_SetBits(GPIOD, GPIO_Pin_14|GPIO_Pin_12|GPIO_Pin_13);
}