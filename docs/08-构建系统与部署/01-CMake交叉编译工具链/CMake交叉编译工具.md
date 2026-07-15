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

* **方法一：**指定工具链文件

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



## 2.CMake文件设计范式

### 2.1 设计范式：变量与规则

#### 2.1.1 声明目标平台身份  

​	设置运行平台的目标系统与目标处理器。

```cmake
set(CMAKE_SYSTEM_NAME Linux)        # 目标系统为 Linux[reference:7]
set(CMAKE_SYSTEM_PROCESSOR arm)     # 目标处理器为 ARM[reference:9]
```



#### 2.1.2 **指定交叉编译工具**

​	必须明确指定 C 和 C++ 交叉编译器的路径或名称

```cmake
# 方式一：直接指定完整路径[reference:12]
set(CMAKE_C_COMPILER /opt/toolchain/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /opt/toolchain/bin/arm-linux-gnueabihf-g++)

# 方式二：指定前缀，让 CMake 在 PATH 中查找[reference:13]
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)
```



#### 2.1.3 **设置系统根目录** 

​	设置所有必要的头文件与库文件的目录，避免链接到编译平台的库文件。

```cmake
set(CMAKE_SYSROOT /opt/toolchain/arm-linux-gnueabihf/sysroot)
```



#### 2.1.4 **配置查找模式** 

​	指定头文件与库文件的查找目录，如 `find_package`, `find_library`等，避免错误链接到编译平台的库。

```cmake
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)  # 查找程序时，不搜索 sysroot
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)   # 查找库时，只在 sysroot 中搜索
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)   # 查找头文件时，只在 sysroot 中搜索
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)   # 查找包时，只在 sysroot 中搜索
```



### 2.2 实现方法

​	编译链文件本身为一个`.cmake`文件，使用使用流程为：

* **创建文件：**在根目录或者编译专用目录创建`.cmake`文件，例如`toolchain-arm.cmake`文件。
* **编写文件：**根据设计范式对文件进行编写，实现不同的编译要求。
* **使用文件：**在编译cmake阶段通过`CMAKE_TOOLCHAIN_FILE`参数指定编译时需要遵循的编译链文件。

```bash
mkdir build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/path/to/your/toolchain.cmake ..
make
```

在指定完对应的编译链文件且编译链文件生效之后，`CMakeLists.txt`文件中的标志命令会根据编译链文件中的要求自动适配为目标平台。



## 3.关键CMake变量与命令

​	在编译链文件中只能使用变量设置与少量非项目级命令来进行配置，运行使用的变量与命令如下：

| 类别     | 命令/变量                       | 说明                                            |
| -------- | ------------------------------- | ----------------------------------------------- |
| 系统标识 | `CMAKE_SYSTEM_NAME`             | 必填。Linux/Generic/Android/iOS/Windows         |
|          | `CMAKE_SYSTEM_PROCESSOR`        | 目标 CPU 架构 (aarch64, arm, riscv64, x86_64)   |
|          | `CMAKE_SYSTEM_VERSION`          | 目标系统版本号                                  |
| 编译器   | `CMAKE_C_COMPILER`              | C 编译器路径或名称                              |
|          | `CMAKE_CXX_COMPILER`            | C++ 编译器路径或名称                            |
|          | `CMAKE_ASM_COMPILER`            | 汇编编译器                                      |
|          | `CMAKE_AR` / `CMAKE_RANLIB`     | 归档与索引工具                                  |
|          | `CMAKE_LINKER`                  | 链接器 (仅在非默认链接器时需要)                 |
| 路径控制 | `CMAKE_SYSROOT`                 | 传递给 `--sysroot=` 并作为查找根                |
|          | `CMAKE_FIND_ROOT_PATH`          | 额外的查找根路径列表                            |
|          | `CMAKE_FIND_ROOT_PATH_MODE_*`   | PROGRAM/LIBRARY/INCLUDE/PACKAGE 的搜索策略      |
|          | `CMAKE_STAGING_PREFIX`          | `make install` 时的临时安装前缀                 |
| 编译控制 | `CMAKE_TRY_COMPILE_TARGET_TYPE` | `STATIC_LIBRARY` (裸机) 或 `EXECUTABLE` (默认)  |
|          | `CMAKE_C_FLAGS_INIT`            | 初始化 C 标志 (比 `add_compile_options` 更安全) |
|          | `CMAKE_CXX_FLAGS_INIT`          | 初始化 C++ 标志                                 |
|          | `add_compile_options()`         | 全局编译选项 (谨慎使用)                         |
|          | `add_link_options()`            | 全局链接选项 (谨慎使用)                         |
| 缓存     | `set(... CACHE ...)`            | 使变量可被命令行覆盖或在 GUI 中可见             |

​	不推荐使用的命令：

| 命令                                 | 原因                                                    |
| ------------------------------------ | ------------------------------------------------------- |
| `project()`                          | 工具链文件在 project() 之前加载，调用它会破坏初始化顺序 |
| `add_executable()` / `add_library()` | 工具链文件不定义构建目标                                |
| `target_*()`                         | 没有目标可供操作                                        |
| `find_package()` / `find_path()`     | 搜索行为尚未配置完成，结果不可靠                        |
| `install()`                          | 安装规则属于项目逻辑                                    |
| `include_directories()`              | 应通过 sysroot 或 target 属性管理                       |









