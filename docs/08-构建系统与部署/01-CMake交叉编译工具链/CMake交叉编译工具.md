# CMake 交叉编译机制与工具链文件

## 1. CMake 的元构建系统模型及交叉编译模式的触发条件

### 1.1 CMake元构建系统模型

#### 1.1.1 构建系统与元构建系统

* **构建系统是直接负责调用编译器、链接器，将源代码转化为可执行文件或库的工具**。

  - **核心职责**：监控文件变更、处理编译顺序、执行编译和链接命令。

  - **典型代表**：Unix/Linux 下的 `Make`、Google 主导的高效构建工具 `Ninja`、微软的 `MSBuild`（用于 Visual Studio）、苹果的 `Xcode` 内部构建系统。

  - **局限性**：它们通常与特定平台或编译器绑定。例如，`Makefile` 的语法和 Shell 命令在 Windows 上可能无法直接运行，这导致跨平台项目需要维护多套构建脚本。

* **元构建系统不直接编译代码，而是作为一层抽象，读取高级别的项目描述，然后翻译成底层构建系统能理解的输入文件。**

  - **核心职责**：屏蔽平台和工具链差异，自动检测系统库文件位置，生成与底层构建系统匹配的构建文件。
  - **典型代表**：**CMake**（生成 Makefile/Ninja/VS 工程）、**Meson**（生成 Ninja 工程）、**Autotools**（生成 Makefile）。



#### 1.1.2 元构建模型的核心组件

* **生成器：**CMake 通过**生成器**将内部抽象模型转换为具体的构建系统语法：

| 生成器类型              | 输出格式                   | 典型场景               |
| ----------------------- | -------------------------- | ---------------------- |
| `Unix Makefiles`        | `Makefile`                 | Linux/Unix 命令行      |
| `Ninja`                 | `build.ninja`              | 追求极速构建           |
| `Visual Studio 16 2019` | `.sln` + `.vcxproj`        | Windows 开发           |
| `Xcode`                 | `.xcodeproj`               | macOS/iOS 开发         |
| `Ninja Multi-Config`    | `build.ninja` + 多配置支持 | 替代 Makefile 的多配置 |

* **两阶段模型：**

  * **阶段一configure：**

    * **输入**：`CMakeLists.txt` + 缓存变量（`CMakeCache.txt`）
    * **动作**：
      - 执行`CMakeLists.txt`中的命令
      - 构建**内部抽象表示**（Target Graph、Directory Scope、Property 系统）
      - 检测编译器、系统特性、依赖库
    * **输出**：`CMakeCache.txt`（持久化配置）+ 内存中的元模型

  * **阶段二Generate：**

    * **输入**：Configure 阶段构建的内存元模型

      **动作**：

      - 生成器遍历内部 Target Graph
      - 将抽象目标、依赖、规则转换为具体构建系统的语法

    * **输出**：`Makefile` / `build.ninja` / `.vcxproj`等





### 1.2 CMake交叉编译模式的触发条件

​	**在 CMake 中，交叉编译的触发条件非常明确且唯一，核心是一个变量值的“不匹配”。**

#### 1.2.1 核心触发条件：变量值对比

CMake 内部有两个关键变量来区分环境：

- **`CMAKE_HOST_SYSTEM_NAME`**：由CMake自动探测，表示当前开发机（构建平台）的操作系统名（如`Linux`、`Windows`、`Darwin`）。
- **`CMAKE_SYSTEM_NAME`**：表示目标机（运行平台）的操作系统名。如果未显式设置，其值默认与宿主机相同。

**触发判据**：在 CMake 加载并执行到`project()`命令时，如果发现`CMAKE_SYSTEM_NAME`的值与 `CMAKE_HOST_SYSTEM_NAME` 不一致，便会自动触发交叉编译模式，并将内部全局变量`CMAKE_CROSSCOMPILING` 设置为`TRUE`。



#### 1.2.2 触发实现

​	既然默认值相同，要达到触发条件，开发者必须显式地告诉CMake目标系统是什么。这通常通过两种方式实现：

* **方法一：****指定工具链文件**

​	在命令行传入 `CMAKE_TOOLCHAIN_FILE`，对应的工具链文件中通过 `set(CMAKE_SYSTEM_NAME Linux)` 、`set(CMAKE_SYSTEM_PROCESSOR arm)   `明确指定目标系统与架构

```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../my_toolchain.cmake ..
make
```

* **方式二（简单测试）：**命令行直接赋值
  直接在 `cmake` 命令中通过`DCMAKE_SYSTEM_NAME`指定目标系统，无需工具链文件（适合简单测试）。

```bash
cmake -DCMAKE_SYSTEM_NAME=Windows ..   # 在 Linux 上构建 Windows 程序
```







