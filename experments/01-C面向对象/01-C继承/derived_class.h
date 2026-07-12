/**
 * @file    derived_class.h
 * @brief   派生类头文件
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#ifndef DERIVED_CLASS_H
#define DERIVED_CLASS_H


#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "base_class.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief 派生类
 */
typedef struct {
    shape_t base;   ///< 嵌套继承基类对应属性与方法
    uint16_t radius;    ///< 派生类属性
} circle_t;

/**
 * @brief 派生类构造函数
 * 
 * @param[out] circle 实例对象
 * @param[in] color 颜色
 * @param[in] color 颜色
 * 
 */
void circle_create(circle_t *circle, uint16_t color, uint16_t radius);

/**
 * @brief 派生类析构函数
 * 
 * @param[in] circle 基类实例对象
 * 
 */
void circle_destroy(circle_t *circle);


#ifdef __cplusplus
}
#endif



#endif /* DERIVED_CLASS_H */