#include "led.h"
#include "gpio.h"
#include "stm32f407xx.h"
#include "stm32f4xx_hal_gpio.h"

// C++ 类实现
class Led {
public:
    Led(GPIO_TypeDef* port, uint16_t pin) : port_(port), pin_(pin) {}
    void toggle() { HAL_GPIO_TogglePin(port_, pin_); }

private:
    GPIO_TypeDef* port_;
    uint16_t pin_;
};

// C 接口的实现
extern "C" {
    Led* Led_create(GPIO_TypeDef* port, uint16_t pin) {
        return new Led(port, pin);  // 调用构造函数
    }

    void Led_toggle(Led* led) {
        if (led) led->toggle();    // 调用成员函数
    }

    void Led_delete(Led* led) {
        delete led;                // 调用析构函数
    }

    void Led_test()
    {
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}