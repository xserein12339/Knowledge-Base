#include "Servo.hpp"
#include "algorithm"
#include "string.h"



void Servo::servo_init(timer_handale_t *handle, uint16_t channel, uint8_t pin)
{
    this->handle = handle;
    this->channel = channel;
    this->pin = pin;
    /* 对应硬件初始化 */


    this->is_started = true;

}

void Servo::servo_deinit(timer_handale_t handle)
{
    if (handle != NULL) {
        return ;
    }
    /* 对应硬件反初始化*/


    this->is_started = false;
}

void Servo::servo_set_param(float min_p, float max_p, float min_a, float max_a)
{
    /* 参数有效判断 */

    /* 设置舵机参数 */
    this->min_pulse_ms  = min_p;
    this->max_pulse_ms  = max_p;
    this->min_angle     = min_a;
    this->max_angle     = max_a;
}

void Servo::servo_set_contral(float max_speed, float acc, float dead)
{
    /* 参数有效判断 */

    /* 设置舵机控制参数 */
    this->max_speed     = max_speed;
    this->acceleration  = acc;
    this->dead_zone     = dead;
}

void Servo::servo_set_target_angle(float angle)
{
    /* 参数有效判断 */

    /* 设置舵机目标角度 */
    this->target_angle = angle;
}

void Servo::servo_get_current_angle(float *angle)
{
    /* 参数有效判断 */

    /* 获取舵机当前角度 */
    *angle = this->current_angle;

}

void Servo::servo_update_angle()
{

    /* 更新逻辑 */

}