/**
 * @file    derived_class.c
 * @brief   派生类实现
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */

#include "derived_class.h"


/**
 * @brief circle重写area
 */
static void circle_area(void *self)
{
    if (NULL == self) {
        return;
    }
    (void)self;
    printf("circle area!\n");
}

/**
 * @brief circle重写draw
 */
static void circle_draw(void *self)
{
    if (NULL == self) {
        return;
    }
    circle_t *c = (circle_t*)self;
    printf("circle draw color = %d, radius = %d\n!",c->base.color, c->radius);
}

/* 派生类虚函数表绑定 */
static const shape_ops_t circle_ops = {
    .area = circle_area,
    .draw = circle_draw,
};

void circle_create(circle_t *circle, uint16_t color, uint16_t radius)
{
    if (NULL == circle) {
        return;
    }
    circle->base.shape_ops = &circle_ops;
    circle->base.color = color;
    circle->radius = radius;
}


void circle_destroy(circle_t *circle)
{
    if (NULL == circle) {
        return;
    }
    circle->base.shape_ops = NULL;
}