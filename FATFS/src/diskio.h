/*-----------------------------------------------------------------------/
/  底层磁盘接口模块头文件   (C)ChaN, 2013                              /
/-----------------------------------------------------------------------*/

#ifndef _DISKIO_DEFINED
#define _DISKIO_DEFINED

#ifdef __cplusplus
extern "C" {
#endif

#define _USE_WRITE	1	/* 1：启用 disk_write 函数 */
#define _USE_IOCTL	1	/* 1：启用 disk_ioctl 函数 */

#include "integer.h"


/* 磁盘函数状态 */
typedef BYTE	DSTATUS;

/* 磁盘函数结果 */
typedef enum {
	RES_OK = 0,		/* 0：成功 */
	RES_ERROR,		/* 1：读/写错误 */
	RES_WRPRT,		/* 2：写保护 */
	RES_NOTRDY,		/* 3：未就绪 */
	RES_PARERR		/* 4：无效参数 */
} DRESULT;


/*---------------------------------------*/
/* 磁盘控制函数原型 */


DSTATUS disk_initialize (BYTE pdrv);
DSTATUS disk_status (BYTE pdrv);
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count);
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count);
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff);


/* 磁盘状态位（DSTATUS） */

#define STA_NOINIT		0x01	/* 驱动器未初始化 */
#define STA_NODISK		0x02	/* 驱动器中无介质 */
#define STA_PROTECT		0x04	/* 写保护 */


/* disk_ioctl 函数命令码 */

/* 通用命令（FatFs 使用） */
#define CTRL_SYNC			0	/* 刷新磁盘缓存（用于写操作） */
#define GET_SECTOR_COUNT	1	/* 获取介质容量（仅 f_mkfs()） */
#define GET_SECTOR_SIZE		2	/* 获取扇区大小（多扇区大小时 _MAX_SS >= 1024） */
#define GET_BLOCK_SIZE		3	/* 获取擦除块大小（仅 f_mkfs()） */
#define CTRL_ERASE_SECTOR	4	/* 强制擦除一块扇区（仅 _USE_ERASE） */

/* 通用命令（FatFs 不使用） */
#define CTRL_POWER			5	/* 获取/设置电源状态 */
#define CTRL_LOCK			6	/* 锁定/解锁介质弹出 */
#define CTRL_EJECT			7	/* 弹出介质 */
#define CTRL_FORMAT			8	/* 在介质上创建物理格式 */

/* MMC/SDC 专用 ioctl 命令 */
#define MMC_GET_TYPE		10	/* 获取卡类型 */
#define MMC_GET_CSD			11	/* 获取 CSD */
#define MMC_GET_CID			12	/* 获取 CID */
#define MMC_GET_OCR			13	/* 获取 OCR */
#define MMC_GET_SDSTAT		14	/* 获取 SD 状态 */

/* ATA/CF 专用 ioctl 命令 */
#define ATA_GET_REV			20	/* 获取固件版本 */
#define ATA_GET_MODEL		21	/* 获取型号名称 */
#define ATA_GET_SN			22	/* 获取序列号 */

#ifdef __cplusplus
}
#endif

#endif
