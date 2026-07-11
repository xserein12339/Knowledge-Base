#include "stdint.h"


typedef void *timer_handale_t;

/**
 * @brief 舵机类 抽象
 */
class Servo
{
private:
    /* 底层资源封装 */
    timer_handale_t handle;         /* 底层定时器句柄 */
    uint16_t        channel;        /* 定时器通道 */
    uint8_t         pin;            /* pwm引脚 */
    /* 舵机参数封装 */
    float           min_pulse_ms;   /* 最小脉宽 */
    float           max_pulse_ms;   /* 最大脉宽 */   
    float           min_angle;      /* 最小角度 */
    float           max_angle;      /* 最大角度 */
    /* 运行控制参数封装 */
    float           max_speed;      /* 最大速度 */   
    float           acceleration;   /* 加速度 */  
    float           dead_zone;      /* 死区阈值 */
    /* 运行数据封装 */
    float           target_angle;   /* 目标角度 */
    float           current_angle;  /* 当前角度 */
    bool            is_started;     /* 运行状态 */

    /*============== 内部使用方法 ==============*/
    /**
     * @brief 根据角度计算脉宽
     * 
     * @param[in] angle 角度
     * 
     * @return
     *      - 对应角度脉宽
     */
    float cacupulse(float angle) {
        if (max_angle == min_angle) {
            return min_pulse_ms; 
        }
        float pulse_width = min_pulse_ms + (angle - min_angle) * (max_pulse_ms - min_pulse_ms) / (max_angle - min_angle);
        return pulse_width;
    }

public:
    /**
     * @brief 构造函数
     * 
     * @param NULL
     * 
     * @return  NULL
     * 
     */
    Servo() {
        min_pulse_ms    = 0.5;  /* 最小脉宽 */
        max_pulse_ms    = 2.5;  /* 最大脉宽 */   
        min_angle       = 0;    /* 最小角度 */
        max_angle       = 180;  /* 最大角度 */
        max_speed       = 100;  /* 最大速度 */   
        acceleration    = 200;  /* 加速度 */  
        dead_zone       = 0.5;  /* 死区阈值 */
        target_angle    = 0;    /* 目标角度 */
        current_angle   = 100;    /* 当前角度 */
        is_started      = false;/* 运行状态 */

    };

    /**
     * @brief 析构函数
     * 
     * @param NULL
     * 
     * @return  NULL
     * 
     */
    ~Servo() {

    }

    /**
     * @brief 舵机硬件初始化
     * 
     * @param[in] handle 定时器句柄
     * @param[in] channel 定时器pwm输出通道
     * @param[in] pin
     * 
     * @return  NULL
     * 
     */
    void servo_init(timer_handale_t *handle, uint16_t channel, uint8_t pin);

    /**
     * @brief 舵机硬件反初始化
     * 
     * @param[in] handle 定时器句柄
     * 
     * @return  NULL
     * 
     */
    void servo_deinit(timer_handale_t handle);

    /**
     * @brief 设置舵机参数
     * 
     * @param[in] min_p 舵机最小脉宽
     * @param[in] max_p 舵机最大脉宽
     * @param[in] min_a 舵机最小角度
     * @param[in] max_a 舵机最大角度
     * 
     * @return  NULL
     * 
     */
    void servo_set_param(float min_p, float max_p, float min_a, float max_a);

    /**
     * @brief 设置舵机控制参数
     * 
     * @param[in] max_speed 舵机最大速度
     * @param[in] acc 舵机加速度
     * @param[in] dead 舵机死区
     * 
     * @return  NULL
     * 
     */
    void servo_set_contral(float max_speed, float acc, float dead);

    /**
     * @brief 设置舵机目标角度
     * 
     * @param[in] angle 舵机目标角度
     * 
     * @return  NULL
     * 
     */
    void servo_set_target_angle(float angle);

    /**
     * @brief 获取舵机当前角度
     * 
     * @param[out] angle 舵机当前角度
     * 
     * @return  NULL
     * 
     */
    void servo_get_current_angle(float *angle);

    /**
     * @brief 更新舵机角度
     * 
     * @param NULL
     * 
     * @return  NULL
     * 
     */
    void servo_update_angle(void);
};
