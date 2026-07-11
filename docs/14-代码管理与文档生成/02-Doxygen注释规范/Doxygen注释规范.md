# Doxygen注释规范

## 一、注释风格

### 1、最常用风格

```c
/**
 * ......内容......
 */
```

### 2、Qt风格

```c
/*!
 * ... 文档内容 ...
 */
```

### 3、行内简写

```c
/// 简要说明
/** 简要说明 */
```



## 二、文件头注释

```c
/**
 * @file   example.cpp
 * @author 作者名 <邮箱>
 * @date   2026-04-19
 * @brief  文件简介
 * @copyright 版权
 * 
 * @par History:	修改历史
 *   - V1.0.0 | 2026-4-19 | 作者 | 初始版本创建
 *
 * @par Function List:	功能函数
 * 
 */
```



## 三、类/结构体注释

### 1、类注释

```c
/**
 * @class ClassName
 * @brief 类的简要说明
 * @ingroup 分组名
 * 
 * 类的详细说明，可以跨多行。
 * 描述类的功能、使用场景和注意事项。
 */
```

### 2、结构体注释

```c
/**
 * @struct structName
 * @brief 结构体的简要说明
 * @ingroup 分组名
 * 
 * 结构体的详细说明，可以跨多行。
 * 描述结构体的功能、使用场景和注意事项。
 * 
 */
```



## 四、函数方法注释

```c
/**
 * @brief 函数的简要说明
 * 
 * 函数的详细说明。描述函数的功能、算法、边界条件等。
 * 可以包含多段落。
 * 
 * @param[in]   input_param  输入参数说明
 * @param[out]  output_param 输出参数说明
 * @param[in,out] io_param   输入输出参数说明
 * 
 * @return 返回值说明
 * @retval 具体值  该值对应的含义（如 @retval true 成功）
 * 
 * @note 注意事项
 * @warning 警告信息
 * @exception 异常说明
 * 
 * @par 示例:
 * @code
 * int result = myFunction(10, nullptr);
 * @endcode
 * 
 * @see 相关函数或类
 * @since 版本 1.2
 * @deprecated 已废弃，建议使用 newFunction()
 */
```



## 五、变量与常量注释

### 1、行尾注释

```c
int globalVar;  ///< 行尾注释：变量的简要说明
```

### 2、常量详细说明

```cpp
/**
 * @brief 常量的详细说明
 * @details 如果常量含义复杂，可以使用块注释
 */
const int MAX_BUFFER_SIZE = 1024;
```

### 3、枚举说明

```c
/** 枚举说明 */
enum color {
    RED,    ///< 红色
    GREEN,  ///< 绿色
    BLUE    ///< 蓝色
};
```



## 六、常用注释命令

| 命令             | 说明       | 示例                       |
| ---------------- | ---------- | -------------------------- |
| `@brief`         | 简要说明   | `@brief 获取用户名`        |
| `@param`         | 参数说明   | `@param[in] name 用户名`   |
| `@return`        | 返回值     | `@return 成功返回0`        |
| `@retval`        | 特定返回值 | `@retval -1 参数错误`      |
| `@note`          | 注意       | `@note 线程不安全`         |
| `@warning`       | 警告       | `@warning 会修改全局状态`  |
| `@see`           | 参考       | `@see relatedFunc()`       |
| `@deprecated`    | 已废弃     | `@deprecated 使用 v2 替代` |
| `@code/@endcode` | 代码示例   |                            |
| `@pre`           | 前置条件   | `@pre ptr != nullptr`      |
| `@post`          | 后置条件   | `@post 状态已更新`         |
| `@invariant`     | 不变式     | `@invariant size >= 0`     |
| `@todo`          | 待办       | `@todo 添加异常处理`       |
| `@bug`           | 已知缺陷   | `@bug 内存泄漏`            |