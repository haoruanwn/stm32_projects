#ifndef LED_H
#define LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

// C 兼容的类型声明（C 代码只能看到这个）
typedef struct Led Led;

// C 可调用的接口函数
Led* Led_create(GPIO_TypeDef* port, uint16_t pin);  // 构造函数
void Led_toggle(Led* led);                          // 反转 LED
void Led_delete(Led* led);                          // 析构函数
void Led_test();


#ifdef __cplusplus
}
#endif



#endif // LED_H