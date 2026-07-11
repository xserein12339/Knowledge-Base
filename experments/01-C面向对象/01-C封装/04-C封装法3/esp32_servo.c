/**
 * @file   esp32_servo.c
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
#include "stdio.h"

typedef void *timer_handle_t;       ///< 模拟底层句柄


/**
 * @brief  舵机派生类
 */
typedef struct esp32_servo_dev_s {
    servo_dev_s             base;
    /* 派生类数据 */
    /* 舵机参数封装 */
    servo_config_t          *servo_cfg;
    /* 运行状态封装 */
    float                   target_angle;   /* 目标角度 */
    float                   current_angle;  /* 当前角度 */
    bool                    is_started;     /* 运行状态 */
    bool                    is_inited;      /* 初始化状态 */
}esp32_servo_dev_s;

/**
 * @brief 舵机初始化函数
 * 
 * @param[in]  handle 舵机实例对象
 * @param[in]  cfg 舵机实例对象配置
 * 
 * @return NULL
 * 
 */
void esp32_servo_init(servo_handle_t handle, servo_config_t *cfg)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self || NULL == self->servo_cfg) {
        /* 错误处理 */   
    }
    if (true == self->is_inited) {
        /* 错误处理 */
    }
    /* 底层资源初始化 */
    printf("init success [%s]\r\n", cfg->name);


    /* 保存配置与初始化运行参数 */
    self->servo_cfg = cfg;
    self->current_angle = 0;
    self->target_angle = 0;
    self->is_started = false;
    self->is_inited = true;
}

/**
 * @brief 舵机反初始化函数
 *
 * @param[in]  handle 舵机实例对象
 * 
 * @return NULL
 */
void esp32_servo_deinit(servo_handle_t handle)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self) {
        /* 错误处理 */   
    }

    /* 释放底层资源 */
    printf("init desuccess [%s]\r\n", self->servo_cfg->name);

    /* 释放运行参数与配置 */
    self->is_inited = false;
    self->is_started = false;
    self->target_angle = 0;
    self->current_angle = 0;
    self->servo_cfg = NULL;
}

/**
 * @brief 舵机实例对象开启/停止函数
 * 
 * @param[in]  handle 舵机实例对象
 * @param[in]  status 舵机运行状态
 * 
 * @return NULL
 * 
 */
void esp32_servo_start_stop(servo_handle_t handle, bool status)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self) {
        /* 错误处理 */   
    }

    /* 调用底层驱动与逻辑 */
    
    printf("start or stop [%s] success\r\n", self->servo_cfg->name);

}

/**
 * @brief 设置舵机实例目标角度
 * 
 * @param[in]  handle 舵机实例对象
 * @param[in]  target_angle 设置目标角度
 * 
 * @return NULL
 * 
 */
void esp32_servo_set_target_angle(servo_handle_t handle, float target_angle)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self) {
        /* 错误处理 */   
    }

    /* 调用底层驱动 */
    self->target_angle = target_angle;
    printf("set [%s] target angle [%f] success\r\n", self->servo_cfg->name, self->target_angle);
}

/**
 * @brief 获取舵机实例当前角度
 * 
 * @param[in]  handle 舵机实例
 * @param[out]  current_angle 获取舵机当前角度
 * 
 * @return NULL
 * 
 */
void esp32_servo_get_current_angle(servo_handle_t handle, float *current_angle)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self) {
        /* 错误处理 */   
    }

    /* 调用底层驱动 */
    *current_angle = self->current_angle;
    printf("get [%s] current angle [%f] success\r\n", self->servo_cfg->name, self->current_angle);
}

/**
 * @brief 更新舵机运行状态
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 * 
 */
void esp32_servo_update(servo_handle_t handle)
{
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    if (NULL == self) {
        /* 错误处理 */   
    }

    /* 调用底层驱动 */
    printf("update [%s] success\r\n", self->servo_cfg->name);
}

/**
 * @brief  舵机派生类方法表
 */
static const servo_ops_t esp32_servo_ops = {
    .init               = esp32_servo_init,
    .deinit             = esp32_servo_deinit,
    .start_stop         = esp32_servo_start_stop,
    .set_target_angle   = esp32_servo_set_target_angle,
    .get_current_angle  = esp32_servo_get_current_angle,
    .update             = esp32_servo_update,
};

/**
 * @brief  舵机类构造函数
 * 
 * @param[out]  handle 舵机实例
 * 
 * @return NULL
 * 
 */
void esp32_servo_create(servo_handle_t *handle)
{
    *handle = NULL;
    if (NULL == handle) {
        /* 错误处理 */
    }
    esp32_servo_dev_s *ret_handle = malloc(sizeof(esp32_servo_dev_s));
    if (NULL == ret_handle) {
        /* 错误处理 */
    }
    /* 构造实例对象 */
    ret_handle->base.servo_ops  = &esp32_servo_ops;
    ret_handle->target_angle    = 0;
    ret_handle->current_angle   = 0;
    ret_handle->is_inited       = false;
    ret_handle->is_started      = false;
    ret_handle->servo_cfg       = NULL;
    /* 返回实例给用户 */
    *handle = (servo_handle_t)ret_handle;

}

/**
 * @brief  舵机类析构函数
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 */
void esp32_servo_delete(servo_handle_t *handle)
{
    if (NULL == handle || NULL == *handle) {
        /* 错误处理 */
    }
    esp32_servo_dev_s *self = (esp32_servo_dev_s*)handle;
    self->base.servo_ops = NULL;
    free(*handle);
    *handle = NULL;
}