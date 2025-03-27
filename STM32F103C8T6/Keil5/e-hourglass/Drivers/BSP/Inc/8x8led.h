#ifndef __8X8LED_H
#define __8X8LED_H


#ifdef __cplusplus
extern "C"
{
#endif


#include <stdint.h>
#include "main.h"


void GC7219_Write_byte(uint8_t data); //单字节写入
void GC7219_Write_Command(uint8_t addr,uint8_t data); //写命令，高八位地址，低八位数据
void GC7219_INIT(void); //初始化
void GC7219_Write_Command_Dual(uint8_t addr1, uint8_t data1, uint8_t addr2, uint8_t data2);

#ifdef __cplusplus
}
#endif

  
#endif