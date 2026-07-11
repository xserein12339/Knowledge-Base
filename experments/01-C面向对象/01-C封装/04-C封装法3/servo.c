/**
 * @file   servo.c
 * @author xLumina
 * @date   2026-04-19
 * @brief  不透明指针+函数指针表模拟面向对象的舵机驱动实现
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | xLumina | 初始版本创建
 *
 * @par Function List:  
 * 
 */

#include "servo_private.h"



/**
 * @brief  舵机类初始化
 */
void servo_init(servo_handle_t handle, servo_config_t *cfg)
{
    if (NULL == handle || NULL == cfg) {
        /* 错误处理 */
    }
    if (NULL == handle->servo_ops) {
        /* 错误处理 */
    }
    handle->servo_ops->init(handle, cfg);

}

/**
 * @brief  舵机类反初始化
 */
void servo_deinit(servo_handle_t handle)
{
    if (NULL == handle) {
        /* 错误处理 */
    }
    /* 释放底层资源 */

    handle->servo_ops->deinit(handle);
}

/**
 * @brief  舵机类启动与停止
 */
void servo_start_stop(servo_handle_t handle, bool status)
{
    if (NULL == handle) {
        /* 错误处理 */
    }
    handle->servo_ops->start_stop(handle, status);
}

/**
 * @brief  舵机类设置目标角度
 */
void servo_set_target_angle(servo_handle_t handle, float target_angle)
{
    if (NULL == handle) {
        /* 错误处理 */
    }
    handle->servo_ops->set_target_angle(handle, target_angle);
}


/**
 * @brief  舵机类获取当前角度
 */
void servo_get_current_angle(servo_handle_t handle, float *current_angle)
{
    if (NULL == handle) {
        /* 错误处理 */
    }
    handle->servo_ops->get_current_angle(handle, current_angle);
}

/**
 * @brief  舵机类更新角度
 */
void servo_update(servo_handle_t handle)
{
    if (NULL == handle) {
        /* 错误处理 */
    }
    handle->servo_ops->update(handle);
}

