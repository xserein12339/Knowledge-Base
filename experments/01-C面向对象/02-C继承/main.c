/**
 * @file    main.c
 * @brief   虚函数表加嵌套实现继承与多态示例
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */
#include "base_class.h"
#include "derived_class.h"


int main(void)
{
    /* 基类实例 */
    shape_t shape1;
    shape_create(&shape1, 0xFFFF);
    shape_area(&shape1);
    shape_draw(&shape1);
    /* 派生类实例 */
    circle_t circle1;
    circle_create(&circle1, 0xAAAA, 3);
    shape_area((shape_t*)&circle1);
    shape_draw((shape_t*)&circle1);

    shape_destroy((shape_t *)&circle1);
    shape_destroy(&shape1);

    return 0;
}