#ifndef	_fatFs_Spi_Flash_API_h_
#define _fatFs_Spi_Flash_API_h_

#include "Spi_Flash.h"
#include "ff.h"



//
//#define	FF_USE_MKFS 1
//#define FF_FS_READONLY 0
//
DSTATUS fatFs_Spi_Flash_Init(void);
DSTATUS fatFs_Spi_Flash_Status(void);
DRESULT fatFs_Spi_Flash_Ioctl(BYTE cmd,char *buff);
DRESULT fatFs_Spi_Flash_Read(BYTE *buff, DWORD sector, UINT count);
DRESULT fatFs_Spi_Flash_Write(const BYTE *buff, DWORD sector, UINT count);
void fatFs_Spi_Flash_Test(void);










#endif

