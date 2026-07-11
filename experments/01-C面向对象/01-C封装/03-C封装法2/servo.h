/**
 * @file   servo.h
 * @author xLumina
 * @date   2026-04-19
 * @brief  结构体内部包含函数指针接口模拟面向对象的舵机驱动声明
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | xLumina | 初始版本创建
 *
 * @par Function List:  
 *      - servo_create() 外部调用，创建实例对象
 *      - servo_delete() 外部调用，删除实例对象
 * 
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef void *timer_handle_t;       ///< 模拟底层句柄

/* 前向声明 */
typedef struct servo_dev_s *servo_dev_t;
typedef struct servo_config_t servo_config_t;

/**
 * @brief 舵机配置结构体
 */
typedef struct servo_config_t {
    /* 底层资源封装 */
    timer_handle_t  timer_handle;   /* 底层定时器句柄 */
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
 * @brief 舵机类
 */
typedef struct servo_dev_s {
    /* 舵机参数封装 */
    const servo_config_t  *servo_cfg; 
    /* 运行状态封装 */
    float           target_angle;   /* 目标角度 */
    float           current_angle;  /* 当前角度 */
    bool            is_started;     /* 运行状态 */
    bool            is_inited;      /* 初始化状态 */
    /* 函数指针模拟类方法 */
    /**
     * @brief 舵机初始化函数
     * 
     * @param[in]  self 舵机实例
     * @param[in]  cfg 舵机配置
     * 
     * @return NULL
     * 
     */
    void (*init)(servo_dev_t self, const servo_config_t *cfg);

    /**
     * @brief 舵机反初始化函数
     * 
     * @param[in]  self 舵机实例
     * 
     * @return NULL
     * handle
     */
    void (*deinit)(servo_dev_t self);

    /**
     * @brief 舵机开启/停止函数
     * 
     * @param[in]  self 舵机实例
     * @param[in]  status 舵机运行状态
     * 
     * @return NULL
     * 
     */
    void (*start_stop)(servo_dev_t self, bool status);

    /**
     * @brief 设置舵机目标角度
     * 
     * @param[in]  self 舵机实例
     * @param[in]  target_angle 设置目标角度
     * 
     * @return NULL
     * 
     */
    void (*set_target_angle)(servo_dev_t self, float target_angle);

    /**
     * @brief 获取舵机当前角度
     * 
     * @param[in]  self 舵机实例
     * @param[out]  current_angle 获取舵机当前角度
     * 
     * @return NULL
     * 
     */
    void (*get_current_angle)(servo_dev_t self, float *current_angle);

    /**
     * @brief 更新舵机运行状态
     * 
     * @param[in]  self 舵机实例
     * 
     * @return NULL
     * 
     */
    void (*update)(servo_dev_t self);  
} servo_dev_s;


/**
 * @brief  舵机构造函数
 * 
 * @param[out]  self 舵机实例
 * 
 * @return NULL
 * 
 * 
 */
void  servo_create(servo_dev_t *self);

/**
 * @brief  舵机析构函数
 * 
 * @param[in]  self 舵机实例
 * 
 * @return NULL
 * 
 * 
 */
void  servo_delete(servo_dev_t *self);


