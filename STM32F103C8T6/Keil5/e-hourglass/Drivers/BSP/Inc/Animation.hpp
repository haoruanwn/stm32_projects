#ifndef __ANIMATION_H
#define __ANIMATION_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "main.h"
#include "8x8led.h"
#include <stdarg.h>
#include "stdio.h"
//#include <vector>



using namespace std;




//声明一个类
class LEDMatrix {
public:
    LEDMatrix();         // 构造函数
    void clear();        // 清空所有 LED
    void setLED(int row, int col);  // 点亮指定的 LED
    void clearLED(int row, int col); // 关闭指定的 LED
    void refresh();      // 刷新显示，将 LED 状态写入硬件
    bool isLEDOn(int row, int col);  // 检查特定 LED 是否点亮

private:
    uint8_t rows1[8];  // 存储每一行的 LED 状态
    uint8_t rows2[8];  // 存储每一行的 LED 状态
};

class Sand {
private:
    int x;
    int y;
    LEDMatrix* matrix;

public:
    Sand(int startX, int startY, LEDMatrix* ledMatrix);
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    void moveUpLeft();
    void moveUpRight();
    void moveDownLeft();
    void moveDownRight();
    void updatePosition();
    int getX() const;
    int getY() const;
};


#ifdef __cplusplus
}
#endif

#endif // !__SHALOU_HPP
