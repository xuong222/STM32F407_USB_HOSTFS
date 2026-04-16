/*-----------------------------------------------------------------------*/
/* FatFs 底层磁盘 I/O 模块骨架     (C)ChaN, 2013                         */
/*-----------------------------------------------------------------------*/
/* 若已有可用的存储控制模块，应通过粘合函数接到 FatFs，而不是改 FatFs 本身。 */
/* 本文件示例：用约定 API 将各类已有存储控制模块接到 FatFs。             */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs 底层 API */
//#include "sdio_sdcard.h"
#include "malloc.h"
#include "usbh_usr.h"


#define SD_CARD	 0  //SD卡,卷标为0
#define USB_DISK 2  //U盘，卷标为2


//初始化磁盘
DSTATUS disk_initialize (
	BYTE pdrv				/* 物理驱动器号（0 起） */
)
{
	u8 res=0;	    
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res = SD_Init();//SD卡初始化 
  			break;
        case USB_DISK://U盘
            if(USBH_UDISK_Status())return 0;	//U盘连接成功,则返回1.否则返回0		  
			else return 1;	 
		default:
			res=1; 
	}		 
	if(res)return  STA_NOINIT;
	else return 0; //初始化成功
}  

//获得磁盘状态
DSTATUS disk_status (
	BYTE pdrv		/* 物理驱动器号（0 起） */
)
{ 
	return 0;
} 

//读扇区
//drv:磁盘编号0~9
//*buff:数据接收缓冲首地址
//sector:扇区地址
//count:需要读取的扇区数
DRESULT disk_read (
	BYTE pdrv,		/* 物理驱动器号（0 起） */
	BYTE *buff,		/* 存放读出数据的缓冲区 */
	DWORD sector,	/* 扇区地址（LBA） */
	UINT count		/* 要读的扇区数（1..128） */
)
{
	u8 res=0; 
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res=SD_ReadDisk(buff,sector,count);	 
//			while(res)//读出错
//			{
//				SD_Init();	//重新初始化SD卡
//				res=SD_ReadDisk(buff,sector,count);	
//				//printf("sd rd error:%d\r\n",res);
//			}
			break;
        case USB_DISK://U盘 
			res = USBH_UDISK_Read(buff,sector,count);	  
			break;
		default:
			res=1; 
	}
   //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res==0x00)return RES_OK;	 
    else return RES_ERROR;	   
}

//写扇区
//drv:磁盘编号0~9
//*buff:发送数据首地址
//sector:扇区地址
//count:需要写入的扇区数
#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* 物理驱动器号（0 起） */
	const BYTE *buff,	/* 待写入数据 */
	DWORD sector,		/* 扇区地址（LBA） */
	UINT count			/* 要写的扇区数（1..128） */
)
{
	u8 res=0;  
    if (!count)return RES_PARERR;//count不能等于0，否则返回参数错误		 	 
	switch(pdrv)
	{
		case SD_CARD://SD卡
//			res = SD_WriteDisk((u8*)buff,sector,count);
//			while(res)//写出错
//			{
//				SD_Init();	//重新初始化SD卡
//				res=SD_WriteDisk((u8*)buff,sector,count);	
//				//printf("sd wr error:%d\r\n",res);
//			}
			break;
        case USB_DISK://U盘
			res = USBH_UDISK_Write((u8*)buff,sector,count); 
			break;
		default:
			res=1; 
	}
    //处理返回值，将SPI_SD_driver.c的返回值转成ff.c的返回值
    if(res == 0x00)return RES_OK;	 
    else return RES_ERROR;	
}
#endif


//其他表参数的获得
 //drv:磁盘编号0~9
 //ctrl:控制代码
 //*buff:发送/接收缓冲区指针
#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* 物理驱动器号（0 起） */
	BYTE cmd,		/* 控制码 */
	void *buff		/* 发送/接收控制数据的缓冲区 */
)
{
	DRESULT res;						  			     
	if(pdrv == SD_CARD)//SD卡
	{
//	    switch(cmd)
//	    {
//		    case CTRL_SYNC:
//				res = RES_OK; 
//		        break;	 
//		    case GET_SECTOR_SIZE:
//				*(DWORD*)buff = 512; 
//		        res = RES_OK;
//		        break;	 
//		    case GET_BLOCK_SIZE:
//				*(WORD*)buff = SDCardInfo.CardBlockSize;
//		        res = RES_OK;
//		        break;	 
//		    case GET_SECTOR_COUNT:
//		        *(DWORD*)buff = SDCardInfo.CardCapacity/512;
//		        res = RES_OK;
//		        break;
//		    default:
//		        res = RES_PARERR;
//		        break;
//	    }
	}
    else if(pdrv == USB_DISK)	//U盘
	{
	    switch(cmd)
	    {
		    case CTRL_SYNC:
				res = RES_OK; 
		        break;	 
		    case GET_SECTOR_SIZE:
		        *(WORD*)buff = 512;
		        res = RES_OK;
		        break;	 
		    case GET_BLOCK_SIZE:
		        *(WORD*)buff = 512;
		        res = RES_OK;
		        break;	 
		    case GET_SECTOR_COUNT:
		        *(DWORD*)buff = USBH_MSC_Param.MSCapacity;
		        res = RES_OK;
		        break;
		    default:
		        res = RES_PARERR;
		        break;
	    }		
	}
	else res=RES_ERROR;//其他的不支持
    return res;
}
#endif
//获得时间
//用户自定义：向 FatFs 提供当前时间（打包为 DWORD）
//31-25：年(0-127，基准 1980)，24-21：月(1-12)，20-16：日(1-31)
//15-11：时(0-23)，10-5：分(0-59)，4-0：秒(0-29，实际秒数需×2)
DWORD get_fattime (void)
{				 
	return 0;
}
//动态分配内存
void *ff_memalloc (UINT size)			
{
	return (void*)mymalloc(SRAMIN,size);
}
//释放内存
void ff_memfree (void* mf)		 
{
	myfree(SRAMIN,mf);
}
