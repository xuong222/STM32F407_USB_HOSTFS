/* 包含头文件 *****************************************************************/
#include "download.h"
#include <string.h>
#include "ff.h"
#include "exfuns.h"
#include "flash_if.h"
#include "LED_Driver.h"
/********************************************************************************
* 函数名 : IAP_Close
* 功  能 : IAP功能关闭
* 说  明 : none
* 入  参 : none
* 返  回 : none
* 设  计 : Shatang                    时  间 : 2020.06.18
* 修  改 : none                       时  间 : none
********************************************************************************/
void IAP_Close(void)
{
    TIM_Cmd(TIM3, DISABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,  DISABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_OTG_FS, DISABLE);
}




char *m_file = "2:Shatang.BIN"; //APP 文件名称 默认起始为 "2:"后跟文件名，文件名不能含中文
#define READ_COUNT 512
uint8_t   ReadAppBuffer[READ_COUNT];
extern uint8_t IAPdownload;

uint8_t UDisk_Download(void)
{
	FIL    FP_Struct;   //用于打开文件
    FRESULT Result;
    UINT ReadNum = 0;
    uint8_t gCheckFat = 0;
    uint32_t total, free;
    uint32_t i = 0;
    uint32_t APP_Sector = 0;
    uint16_t APP_Byte = 0;
    uint32_t FlashDestination = APPLICATION_START_ADDRESS;//初始化写入Flash地址变量

    gCheckFat = exf_getfree("2:", &total, &free);
    if(gCheckFat != 0)
    {
        return 1;
    }

    Result = f_open(&FP_Struct, m_file, FA_READ);
    if(Result != FR_OK)
    {
        return 2;
    }

    if(FP_Struct.fsize == 0 || FP_Struct.fsize > APPLICATION_SIZE)
    {
        f_close(&FP_Struct);
        return 3;
    }

    APP_Sector = FP_Struct.fsize / READ_COUNT;
    APP_Byte = FP_Struct.fsize % READ_COUNT;

    LED_1(1);
    LED_2(1);//成功识别到U盘对应文件，准备擦除芯片Flash
    LED_3(0);

    if(FLASH_If_Erase(APPLICATION_START_ADDRESS) != 0)
    {
        f_close(&FP_Struct);
        return 4;
    }

    LED_1(1);
    LED_2(1);
    LED_3(1);//芯片Flash擦除成功，准备烧录升级文件

    for(i = 0; i < APP_Sector; i++)
    {
        Result = f_read(&FP_Struct, ReadAppBuffer, READ_COUNT, &ReadNum);
        if((Result != FR_OK) || (ReadNum != READ_COUNT))
        {
            f_close(&FP_Struct);
            return 5;
        }

        if(FLASH_If_Write(&FlashDestination, (u32*)ReadAppBuffer, READ_COUNT / 4) != 0)
        {
            f_close(&FP_Struct);
            return 6;
        }
    }

    if(APP_Byte != 0)
    {
        memset(ReadAppBuffer, 0xFF, READ_COUNT);
        Result = f_read(&FP_Struct, ReadAppBuffer, APP_Byte, &ReadNum);
        if((Result != FR_OK) || (ReadNum != APP_Byte))
        {
            f_close(&FP_Struct);
            return 7;
        }

        if(FLASH_If_Write(&FlashDestination, (u32*)ReadAppBuffer, (APP_Byte + 3) / 4) != 0)
        {
            f_close(&FP_Struct);
            return 8;
        }
    }

    f_close(&FP_Struct);
    IAPdownload = 1;//文件更新成功
    return 0;
}
