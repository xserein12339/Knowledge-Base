/**
 * @file   servo.h
 * @author xLumina
 * @date   2026-04-19
 * @brief  不透明指针+普通函数模拟面向对象的舵机驱动声明
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | xLumina | 初始版本创建
 *
 * @par Function List:
 *      - servo_create() 外部调用，创建舵机实例
 *      - servo_delete() 外部调用，删除舵机实例
 *      - servo_init() 外部调用，舵机实例初始化
 *      - servo_deinit() 外部调用，舵机实例反初始化
 *      - servo_start_stop() 外部调用，舵机开始与停止
 *      - servo_set_target_angle() 外部调用，设置舵机目标角度
 *      - servo_get_current_angle() 外部调用，获取舵机当前角度
 *      - servo_update() 外部调用，舵机运动更新   
 * 
 */


#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

typedef void *timer_handle_t;       ///< 模拟mcu定时器资源

/**
 * @brief 舵机对象前向声明
 */
typedef struct servo_t *servo_handle_t;

/**
 * @brief 舵机配置结构体
 */
typedef struct servo_config_t {
    /* 底层资源 */
    timer_handle_t  timer_handle;   ///< 定时器句柄
    uint16_t        channel;        ///< 定时器通道
    uint8_t         pin;            ///< pwm引脚
    /* 舵机默认参数 */
    float           min_pulse_ms;   ///< 最小脉宽
    float           max_pulse_ms;   ///< 最大脉宽  
    float           min_angle;      ///< 最小角度
    float           max_angle;      ///< 最大角度
    /* 运行控制参数 */
    float           max_speed;      ///< 最大速度   
    float           acceleration;   ///< 加速度 
    float           dead_zone;      ///< 死区阈值
} servo_config_t;



/*=============== 舵机方法 ===============*/
/**
 * @brief 舵机类构造函数
 * 
 * @param[out]  handle 舵机实例
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_create(servo_handle_t *handle);

/**
 * @brief 舵机类析构函数
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_delete(servo_handle_t *handle); 

/**
 * @brief 舵机初始化
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  cfg 舵机实例配置
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_init(servo_handle_t handle, const servo_config_t *cfg); 

/**
 * @brief 舵机反初始化
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_deinit(servo_handle_t handle); 

/**
 * @brief 启动/关闭舵机
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  level 开启或关闭
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_start_stop(servo_handle_t handle, bool level); 

/**
 * @brief 设置舵机目标角度
 * 
 * @param[in]  handle 舵机实例
 * @param[in]  target_angle 目标角度
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_set_target_angle(servo_handle_t handle, float target_angle);

/**
 * @brief 获取舵机当前角度
 * 
 * @param[in]  handle 舵机实例
 * @param[out]  current_angle 目标角度
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_get_current_angle(servo_handle_t handle, float *current_angle);

/**
 * @brief 更新舵机
 * 
 * @param[in]  handle 舵机实例
 * 
 * @return NULL
 * 
 * @note 
 * 
 */
void servo_update(servo_handle_t handle);
