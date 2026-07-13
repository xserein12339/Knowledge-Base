# C宏使用技巧

​	C语言宏（Macro）是预处理器提供的强大工具，但也是一把“双刃剑”。用得好可以极大提升代码的灵活性和性能，用不好则会导致难以调试的Bug。

## 1.基础安全原则

### 1.1 参数加括号，防止运算符优先级问题。

```c
/**
 * @file    main.c
 * @brief   C宏使用技巧测试
 *
 * @details 
 *
 * @author  xserein
 * @version 1.0
 */
#include "stdlib.h"
#include "stdio.h"
#include "stdint.h"

/* 01-参数加括号，防止运算符优先级错误 */
#define MULTIPLY_FALSE(a, b) a * b          ///< 错误
#define MULTIPLY_TRUE(a, b) ((a) * (b))     ///< 正确  

int main(void)
{
    printf("value = %d\n", MULTIPLY_FALSE(3 + 4, 5));
    printf("value = %d\n", MULTIPLY_TRUE(3 + 4, 5));
    return 0;
}

```

* **结果**

```bash
value = 23
value = 35
```



### 1.2 多语句do { ... } while(0)封装宏

​	多语句`do { ... } while(0)`是 C 语言中封装多语句宏的黄金标准，尤其常见于 Linux 内核、嵌入式固件和各类底层库中。它的核心作用是：让宏在语法上表现为一条完整的语句，从而安全地嵌入` if-else `等控制流中。

```c
#define SWAP_FALSE(a, b) { int temp = (a); (a) = (b); (b) = temp; }   ///< 错误
#define SWAP_TRUE(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)              //正确  

int main(void)
{
    printf("01-参数加括号，防止运算符优先级错误\n");
    printf("value = %d\n", MULTIPLY_FALSE(3 + 4, 5));
    printf("value = %d\n", MULTIPLY_TRUE(3 + 4, 5));

    printf("02-多语句do {...} while (0)封装宏，确保宏在 if-else 等场景下语法安全\n");
    int a = 3, b = 10;
    if (1)
        SWAP_FALSE(a, b);
    else
        printf("xxx\n");

    if (1)
        SWAP_TRUE(a, b);
    else 
        printf("xxx\n");
    return 0;
}
```

* **错误宏结果**

main.c: In function ‘main’:
main.c:36:5: error: ‘else’ without a previous ‘if’
   36 |     else
      |     ^~~~



## 2.进阶使用技巧

### 2.1 字符串化 （#）与 令牌粘贴（##）

* `#`将参数转换为字符串字面量，常用于日志与断言。

```c
#define LOG_VAR(x) printf("%s = %d\n", #x, (x))

```

结果：count = 100

* `##`拼接标识符，用于生成动态变量名/函数名。

```c
#define MAKE_GETTER(type, name) \
    type get_##name(void) { type g_##name = 100; return g_##name; }

MAKE_GETTER(int, age)   ///< 生成对于函数
```

结果：age = 100

### 2.2 编译器特定优化与诊断

* **`__attribute__` / `_Pragma`**：控制编译器行为。

```c
#define DEPRECATED(msg) __attribute__((deprecated(msg)))

int connect_legacy(const char *host, int port)
{
    printf("connect legacy!");
    return 0;
}

DEPRECATED("Use connect_v2() instead")
int connect_legacy(const char *host, int port);

int main(void)
{
    connect_legacy("example.com", 80);
    return 0;
}
```

结果：

main.c: In function ‘main’:
main.c:71:5: warning: ‘connect_legacy’ is deprecated: Use connect_v2() instead [-Wdeprecated-declarations]
   71 |     connect_legacy("example.com", 80);
      |     ^~~~~~~~~~~~~~
main.c:36:5: note: declared here
   36 | int connect_legacy(const char *host, int port)
      |     ^~~~~~~~~~~~~~

* **静态断言**

```c
#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

typedef struct packet_t {
    uint8_t  type;
    uint32_t len;
    uint8_t  payload[0];
} packet_t;

STATIC_ASSERT(sizeof(struct packet_t) == 5, packet_header_size_mismatch);   ///< 错误
STATIC_ASSERT(sizeof(struct packet_t) == 8, packet_header_size_mismatch);   ///< 正确

```

错误输出:

main.c:46:18: error: size of array ‘static_assert_packet_header_size_mismatch’ is negative
   46 |     typedef char static_assert_##msg[(cond) ? 1 : -1]
      |                  ^~~~~~~~~~~~~~
main.c:54:1: note: in expansion of macro ‘STATIC_ASSERT’
   54 | STATIC_ASSERT(sizeof(struct packet_t) == 5, packet_header_size_mismatch);
      | ^~~~~~~~~~~~~

* **可变参数宏 (`__VA_ARGS__`)**：实现类似 `printf` 的可变参接口。

```C
#define LOG(fmt, ...) \
    printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)

int main(void)
{
    
    int x = 42;
    LOG("Starting up");                    
    LOG("x = %d, hex = 0x%x", x, x); 

    return 0;
}

```

结果：

[main.c:90] Starting up
[main.c:91] x = 42, hex = 0x2a



## 3.结构化宏模式

### 3.1 **X-Macro 模式**：解决"数据定义与使用分离"的问题，消除重复代码。

```c
/* 定义一次数据表 */
#define COLOR_LIST(X) \
    X(RED,   0xFF0000) \
    X(GREEN, 0x00FF00) \
    X(BLUE,  0x0000FF)

/* 自动生成枚举 */
enum Color { 
    #define GEN_ENUM(name, val) COLOR_##name,
    COLOR_LIST(GEN_ENUM)
    #undef GEN_ENUM
};

/* 自动生成名称查找表 */
const char* color_names[] = {
    #define GEN_STR(name, val) #name,
    COLOR_LIST(GEN_STR)
    #undef GEN_STR
};
```

