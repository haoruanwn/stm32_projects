#include "main.h"
#include "8x8led.h"

// 将数据写入模块内部移位寄存器
void GC7219_Write_byte(uint8_t data) // 单字节写入
{
	uint8_t i = 0;
	for (i = 8; i > 0; i--)
	{
		HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_RESET); // MAX7219_CLK=0;
		if ((data & 0x80) == 0)									   // 高位先行
		{
			HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(DIN_GPIO_Port, DIN_Pin, GPIO_PIN_SET);
		}
		data = data << 1;
		HAL_GPIO_WritePin(CLK_GPIO_Port, CLK_Pin, GPIO_PIN_SET);
	}
}
// 将移位寄存器内的值写入控制寄存器
void GC7219_Write_Command(uint8_t addr, uint8_t data) // 写命令，高八位地址，低八位数据
{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	GC7219_Write_byte(addr); // 寄存器地址
	GC7219_Write_byte(data); // 要写入的数据
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

// 同时写入两个MAX7219芯片
void GC7219_Write_Command_Dual(uint8_t addr1, uint8_t data1, uint8_t addr2, uint8_t data2)
{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	GC7219_Write_byte(addr1); // 第一个MAX7219的寄存器地址
	GC7219_Write_byte(data1); // 第一个MAX7219的数据
	GC7219_Write_byte(addr2); // 第二个MAX7219的寄存器地址
	GC7219_Write_byte(data2); // 第二个MAX7219的数据
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
}

/************宏定义方便修改和观察*******************/
// 地址信息
#define DECODEMODE 0X09	 // 编码模式
#define INTENSITY 0X0A	 // 亮度
#define SCANLIMT 0x0B	 // 扫描寄存器个数
#define SHUTDOWN 0X0C	 // 关闭寄存器
#define DISPLAYTEST 0X0F // 显示测试
/*************************************************/
void GC7219_INIT(void)
{
	/*译码寄存器：1使用BCD码   0不使用BCD*/
	GC7219_Write_Command(DECODEMODE, 0X00);
	/*亮度控制 0x00~0x0F --> 最暗~最亮*/
	GC7219_Write_Command(INTENSITY, 0X01);
	/*点阵屏的行数  数码管的段位  0时1行，7是8行*/
	GC7219_Write_Command(SCANLIMT, 0X07);
	/*掉电模式：0掉电模式  1正常模式*/
	GC7219_Write_Command(SHUTDOWN, 0X01);
	/*显示寄存器：0普通模式 1测试模式*/
	GC7219_Write_Command(DISPLAYTEST, 0X00);
}