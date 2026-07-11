/**
 * @file   main.c
 * @author xLumina
 * @date   2026-04-19
 * @brief  不透明指针+普通函数模拟面向对象的舵机驱动使用
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | xLumina | 初始版本创建
 *
 * @par Function List:	功能函数
 * 
 */

#include "servo.h"
#include "stdio.h"


int main(void) 
{
    timer_handle_t timer_handle     = NULL;
    servo_handle_t servo1_handle    = NULL;
    servo_create(&servo1_handle);
    servo_config_t cfg = {0};
    servo_init(servo1_handle, &cfg);
    float curr_angle;
    servo_get_current_angle(servo1_handle, &curr_angle);
    printf("current angle = [%f]", curr_angle);

}

