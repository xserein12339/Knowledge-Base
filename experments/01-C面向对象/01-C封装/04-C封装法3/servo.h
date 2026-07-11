/**
 * @file   servo.h
 * @author xLumina
 * @date   2026-04-19
 * @brief  不透明指针+函数指针表模拟面向对象的舵机驱动声明
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | xLumina | 初始版本创建
 *
 * @par Function List:  
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct servo_dev_s servo_dev_s;
typedef struct servo_dev_s *servo_handle_t;     ///< 不透明指针隐藏具体实现

/**
 * @brief 舵机配置结构体
 */
typedef struct servo_config_t {
    char            *name;          /* 舵机名 */
    /* 底层资源封装 */
    void            *timer_handle;   /* 底层定时器句柄 */
    uint16_t        channel;        /* 定时器通道 */
    uint8_t         pin;            /* pwm引脚 */
    /* 舵机固定参数封装 */
    float           min_pulse_ms;   /* 最小脉宽 */
    float           max_pulse_ms;   /* 最大脉宽 */
    float           min_angle;      /* 最小角度 */
    float           max_angle;      /* 最大角度 */
    /* 运行控制参数封装 */
    float           max_speed;      /* 最大速度 */
    float           acceleration;   /* 加速度 */
    float           dead_zone;      /* 死区阈值 */
} servo_config_t;

/**
 * @brief 舵机类行为函数指针结构体
 */
typedef struct servo_ops_t {
    /**
     * @brief 舵机初始化函数
     * 
     * @param[in]  handle 舵机实例
     * @param[in]  cfg 舵机配置
     * 
     * @return NULL
     * 
     */
    void (*init)(servo_handle_t handle, servo_config_t *cfg);

    /**
     * @brief 舵机反初始化函数
     * 
     * @param[in]  handle 舵机实例
     * 
     * @return NULL
     * handle
     */
    void (*deinit)(servo_handle_t handle);

    /**
     * @brief 舵机开启/停止函数
     * 
     * @param[in]  handle 舵机实例
     * @param[in]  status 舵机运行状态
     * 
     * @return NULL
     * 
     */
    void (*start_stop)(servo_handle_t handle, bool status);

    /**
     * @brief 设置舵机目标角度
     * 
     * @param[in]  handle 舵机实例
     * @param[in]  target_angle 设置目标角度
     * 
     * @return NULL
     * 
     */
    void (*set_target_angle)(servo_handle_t handle, float target_angle);

    /**
     * @brief 获取舵机当前角度
     * 
     * @param[in]  handle 舵机实例
     * @param[out]  current_angle 获取舵机当前角度
     * 
     * @return NULL
     * 
     */
    void (*get_current_angle)(servo_handle_t handle, float *current_angle);

    /**
     * @brief 更新舵机运行状态
     * 
     * @param[in]  handle 舵机实例
     * 
     * @return NULL
     * 
     */
    void (*update)(servo_handle_t handle);
} servo_ops_t;

/**
 * @brief  舵机类构造函数
 * 
 * @param[out]  handle 舵机实例
 * 
 * @return NULL
 * 
 */
void esp32_servo_create(servo_handle_t *handle);

/**
 * @brief  舵机类析构函数
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 */
void esp32_servo_delete(servo_handle_t *handle);



/**
 * @brief 舵机初始化函数
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  cfg 舵机配置
 * 
 * @return NULL
 * 
 */
void servo_init(servo_handle_t handle, servo_config_t *cfg);

/**
 * @brief  舵机类反初始化
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 */
void servo_deinit(servo_handle_t handle);

/**
 * @brief 舵机开启/停止函数
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  status 舵机运行状态
 * 
 * @return NULL
 * 
 */
void servo_start_stop(servo_handle_t handle, bool status);

/**
 * @brief 设置舵机目标角度
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  target_angle 设置目标角度
 * 
 * @return NULL
 * 
 */
void servo_set_target_angle(servo_handle_t handle, float target_angle);

/**
 * @brief 获取舵机当前角度
 * 
 * @param[in]  handle 舵机实例
 * @param[out]  current_angle 获取舵机当前角度
 * 
 * @return NULL
 * 
 */
void servo_get_current_angle(servo_handle_t handle, float *current_angle);

/**
 * @brief 更新舵机运行状态
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 * 
 */
void servo_update(servo_handle_t handle);