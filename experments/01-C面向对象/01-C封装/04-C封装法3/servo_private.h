/**
 * @file   servo_private.h
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

 #include "servo.h"


/**
 * @brief 舵机抽象类
 */
typedef struct servo_dev_s {
    /* 舵机行为函数指针表 */
    const servo_ops_t       *servo_ops;
} servo_dev_s;
