/**
 * @file    base_class.c
 * @brief   实现带虚函数表的继承与多态
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#include "base_class.h"

/* ----- 基类默认虚函数实现 ----- */
/**
 * @brief 基类area默认虚函数实现
 */
static void shape_area_default(void *self)
{
    (void)self;
    printf("shape area deafault!\n");
}

/**
 * @brief 基类draw默认虚函数实现
 */
static void shape_draw_default(void *self)
{
    shape_t *s = (shape_t*)self;    ///< 安全转型
    printf("shape draw default,draw %d\n", s->color);
}

/* 基类虚函数表单例绑定 */
static const shape_ops_t shape_ops_default = {
    .area = shape_area_default,
    .draw = shape_draw_default,
};


void shape_create(shape_t *shape, uint16_t color)
{
    if (NULL == shape) {
        return;
    }
    shape->color     = color;
    shape->shape_ops = &shape_ops_default;
}


void shape_destroy(shape_t *shape)
{
    if (NULL == shape || NULL == shape->shape_ops) {
        return;
    }
    shape->shape_ops = NULL;
}

void shape_area(shape_t *shape)
{
    if (NULL == shape || NULL == shape->shape_ops) {
        return;
    }
    shape->shape_ops->area(shape);
}

void shape_draw(shape_t *shape)
{
    if (NULL == shape || NULL == shape->shape_ops) {
        return;
    }
    shape->shape_ops->draw(shape);
}