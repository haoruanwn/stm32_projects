#include "main.h"

#ifndef _SPI_H_
#define _SPI_H_

#define u8 uint8_t


//本测试程序使用的是模拟SPI接口驱动



//液晶控制口置1操作语句宏定义

#define	SPI_MOSI_SET  	 SDA_GPIO_Port->BSRR= SDA_Pin  
#define	SPI_SCLK_SET  	 SCL_GPIO_Port->BSRR= SCL_Pin   


//液晶控制口置0操作语句宏定义

#define	SPI_MOSI_CLR  	SDA_GPIO_Port->BSRR=(uint32_t)SDA_Pin << 16U    
#define	SPI_SCLK_CLR  	SCL_GPIO_Port->BSRR=(uint32_t)SCL_Pin << 16U 

void  SPIv_WriteData(u8 Data);

#endif