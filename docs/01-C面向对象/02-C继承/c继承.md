# C实现面向对象继承

​	C语言是一门面向过程的编程语言，虽然没有面向对象的语法，但是可以通过使用结构体嵌套来魔力基类与派生类的内存布局从而在C 语言中实现面向对象编程（OOP）的**继承**。

## 1.实现方案

​	因为C语言没有类与继承的关键字，需要手动进行内存布局与类型转换，所以有几种实现方案。

### 1.1 结构体嵌套+类型转换

​	C语言标准保证：结构体指针的地址与其结构体中首个成员的地址相同，所以可以安全的进行类型转换，在子类结构体中的首个成员定义为基类类型，此时将子类转换为基类就可以实现安全的转换了。

```c
#include "stdio.h"

/**
 * @brief 基类封装
 */
typedef struct {
    int x;
    int y;
    void (*draw)(void *self);
} point_t;

/**
 * @brief 圆形子类
 */
typedef struct {
    point_t base;  ///< 必须为子类结构体的首位
    int radius;
} circle_t;

/**
 * @brief 基类方法实现
 */
void point_draw(void *self)
{
    /* 安全向上转型 */
    point_t *p = (point_t*)self;
    printf("X = %d, Y = %d\n", p->x, p->y);
}

int main(void)
{
    /* 构造子类实例 */
    circle_t cir1 = {
        .base = {
            .x = 10,
            .y = 20,
            .draw = point_draw,
        },
        .radius = 6,
    };

    /* 属性访问与方法调用 */
    cir1.base.draw(&cir1);
    cir1.base.x = 1,
    cir1.base.y =2,
    cir1.base.draw(&cir1);

}

```

### 1.2 带虚函数表的继承与多态

​	在面向对象的语言中，类中需要有属性与方法，上面的实现方法只是对类的属性进行继承，没有进行类的方法，如果直接在基类中进行方法的定义的话，子类就实现不了方法的重定义了，只能使用父类的方法。所以需要在基类中添加一个虚函数表，即函数指针结构体，这样就可以在实现类的继承的情况下同时可以实现多态实现。

* **base_class.h**

```c
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
```

* **base_clase.c**

```c
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
```



* **derived_class.h**

```c
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
```



* **derived_class.c**

```c
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
```



* **main.c**

```c
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
```

* **运行结果**

```txt
shape area deafault!
shape draw default,draw 65535
circle area!
circle draw color = 43690, radius = 3
```

