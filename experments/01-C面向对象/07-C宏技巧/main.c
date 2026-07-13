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

/* 02-多语句do {...} while (0)封装宏 */
#define SWAP_FALSE(a, b) { int temp = (a); (a) = (b); (b) = temp; }   ///< 错误
#define SWAP_TRUE(a, b) do { \
    int temp = (a); \
    (a) = (b); \
    (b) = temp; \
} while(0)              //正确    

/* 03-字符串化（#）与令牌粘贴（##）*/
#define LOG_VAR(x) printf("%s = %d\n", #x, (x))
#define MAKE_GETTER(type, name) \
    type get_##name(void) { type g_##name = 100; return g_##name; }

MAKE_GETTER(int, age)   ///< 生成对于函数

/* 04-编译器特定优化与诊断 */
#define DEPRECATED(msg) __attribute__((deprecated(msg)))

int connect_legacy(const char *host, int port)
{
    printf("connect legacy!");
    return 0;
}

DEPRECATED("Use connect_v2() instead")
int connect_legacy(const char *host, int port);

#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

typedef struct packet_t {
    uint8_t  type;
    uint32_t len;
    uint8_t  payload[0];
} packet_t;

// STATIC_ASSERT(sizeof(struct packet_t) == 5, packet_header_size_mismatch);   ///< 错误
STATIC_ASSERT(sizeof(struct packet_t) == 8, packet_header_size_mismatch);   ///< 正确



#define LOG(fmt, ...) \
    printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__)


int main(void)
{
    // printf("01-参数加括号，防止运算符优先级错误\n");
    // printf("value = %d\n", MULTIPLY_FALSE(3 + 4, 5));
    // printf("value = %d\n", MULTIPLY_TRUE(3 + 4, 5));

    // printf("02-多语句do {...} while (0)封装宏，确保宏在 if-else 等场景下语法安全\n");
    // int a = 3, b = 10;
    // if (1)
    //     SWAP_FALSE(a, b);
    // else
    //     printf("xxx\n");

    // if (1)
    //     SWAP_TRUE(a, b);

    // else 
    //     printf("xxx\n");

    // printf("03-字符串化（#）与令牌粘贴（##）\n");
    // int count = 100;
    // LOG_VAR(count);
    // printf("age = %d\n", get_age());

    // printf("04-编译器特定优化与诊断\n");
    // connect_legacy("example.com", 80);
    int x = 42;
    LOG("Starting up");                 
    LOG("x = %d, hex = 0x%x", x, x);     

    return 0;
}
