/**
 * @file    base_class.h
 * @brief   实现带虚函数表的继承与多态
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#ifndef BASE_CLASS_H
#define BASE_CLASS_H


#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 虚函数表
 */
typedef struct {
    void (*area)(void *self);
    void (*draw)(void *self);
} shape_ops_t;

/**
 * @brief 基类
 */
typedef struct {
    const shape_ops_t *shape_ops;   ///< 每个对象都持有一个虚函数表
    uint16_t color;
} shape_t;

/**
 * @brief 基类构造函数
 * 
 * @param[out] shape 实例对象
 * @param[in] color 颜色
 */
void shape_create(shape_t *shape, uint16_t color);

/**
 * @brief 基类析构函数
 * 
 * @param[in] shape 基类实例对象
 * 
 */
void shape_destroy(shape_t *shape);

/* ----- 公共API ----- */
/**
 * @brief 多态area方法调用包装器
 */
void shape_area(shape_t *shape);

/**
 * @brief 多态draw方法调用包装器
 */
void shape_draw(shape_t *shape);



#ifdef __cplusplus
}
#endif



#endif /* BASE_CLASS_H */