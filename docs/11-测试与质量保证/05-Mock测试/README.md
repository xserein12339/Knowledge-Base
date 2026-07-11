# Mock 测试 — 嵌入式 C 的宿主机单元测试方案

## 这是什么

Mock（模拟/替身）测试是一种**不依赖真实硬件**的单元测试方法。通过用假函数替换 MCU 外设驱动（GPIO、I2C、SPI 等），在 PC 上编译运行，验证业务逻辑的正确性。

## 解决什么痛点

| 痛点 | Mock 测试的解法 |
|------|----------------|
| 每次测都要烧录、等硬件就绪 | PC 上秒级运行，CI 友好 |
| 硬件故障阻塞测试（传感器坏了） | 无硬件依赖 |
| "改一行代码要上板跑一遍" | `cmake --build && ./test` |
| 异常路径覆盖难（I2C NACK、超时） | Mock 函数直接返回错误码 |

## 文档索引

| 文档 | 内容 |
|------|------|
| [01-概念与原理](./01-概念与原理.md) | 什么是 Mock、Stub、Fake；嵌入式 C 的 Mock 如何工作 |
| [02-框架选型与搭建](./02-框架选型与搭建.md) | Unity/CMock/FFF 对比；CMake 构建；Mock 头文件结构 |
| [03-编写测试与案例](./03-编写测试与案例.md) | 基于 pal_gpio 的完整测试编写流程；断言使用；FFF API |
| [04-进阶与注意事项](./04-进阶与注意事项.md) | 静态状态清理；指针参数陷阱；扩展 Mock 模块；持续集成 |

## 推荐工具栈

```
Unity (断言)  +  FFF (Mock 框架)  +  CMake (构建)  +  宿主机 GCC
```

## 快速体验

```bash
# 以本项目 PAL 层为例
cd tests
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/pal_test -v
# 输出: 22 Tests 0 Failures 0 Ignored OK
```