#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H
#include "stm32f4xx.h" 

/****************************** GLOBAL FUNCTIONS ******************************/
/* Board silk-screen mapping (active low):
 * PD14 -> LED2
 * PD12 -> LED3
 * PD13 -> LED4
 */
#define LED2_ON         GPIO_ResetBits(GPIOD,GPIO_Pin_14)
#define LED2_OFF        GPIO_SetBits(GPIOD,GPIO_Pin_14)
#define LED3_ON         GPIO_ResetBits(GPIOD,GPIO_Pin_12)
#define LED3_OFF        GPIO_SetBits(GPIOD,GPIO_Pin_12)
#define LED4_ON         GPIO_ResetBits(GPIOD,GPIO_Pin_13)
#define LED4_OFF        GPIO_SetBits(GPIOD,GPIO_Pin_13)
/****************************** GLOBAL FUNCTIONS ******************************/

#define LED_1(x)       ((x)?LED2_ON:LED2_OFF)
#define LED_2(x)       ((x)?LED3_ON:LED3_OFF)
#define LED_3(x)       ((x)?LED4_ON:LED4_OFF)

extern void LED_PortInit(void);

#endif
