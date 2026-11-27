# 技术栈规范

本文记录项目的技术栈基线、版本要求和依赖选择。当前仓库主要保存规范文档，尚未包含实际构建配置。项目开始实现代码后，构建文件和锁定的依赖版本必须与本文保持一致。

## 基础技术栈

| 组件 | 基线要求 | 说明 |
| --- | --- | --- |
| 编程语言 | C++17 | 新代码必须使用 C++17 及以上语言特性。 |
| GUI 框架 | Qt 5.15.2 | 项目默认使用 Qt 5.15.2。Qt 6 项目必须在项目级文档中明确声明版本。 |
| 构建系统 | CMake 3.16 或更高版本 | 新项目的最低版本必须写入 `cmake_minimum_required()`。 |
| 编译器 | 支持 C++17 的项目指定编译器 | 实际编译器版本由 CI 和发布平台共同确定。 |
| 测试框架 | Qt Test 或项目指定的 C++ 测试框架 | 测试框架必须能覆盖 Domain、Presentation 和 Infrastructure。 |
| 日志 | spdlog | 业务代码通过项目统一的日志封装使用日志库。 |

## Qt 版本

- Qt 目标版本为 Qt 5.15.2，模块使用 `Qt5::Core`、`Qt5::Gui`、`Qt5::Widgets`、`Qt5::Network`、`Qt5::Sql` 等 CMake 目标。
- Qt 6 API 不得未经评估直接引入 Qt 5.15.2 项目。
- Qt 版本升级必须评估源码兼容性、ABI 兼容性、构建脚本、部署包和平台行为。
- Qt 代码遵循 [Qt 编码规范](qt-coding-guidelines.md)、[Qt API 设计规范](qt-api-design-guidelines.md) 和 [Qt 注释规范](qt-doxygen-guidelines.md)。

## C++ 标准

- CMake 必须显式设置 C++ 标准，例如 `CXX_STANDARD 17` 或 `target_compile_features(target PRIVATE cxx_std_17)`。
- 禁止依赖编译器默认语言标准。
- 新代码必须遵循 [C++ 编码规范](cpp-coding-guidelines.md) 和 [C++ API 设计规范](cpp-api-design-guidelines.md)。
- 需要更高语言标准时，必须在技术栈文档和构建配置中同时更新，并说明兼容性影响。

## 构建系统

- CMake 最低版本为 3.16。
- 依赖必须通过目标级别的 `target_link_libraries()` 和 `target_include_directories()` 声明。
- 禁止通过全局目录设置隐藏模块依赖。
- Qt 模块必须通过 `find_package(Qt5 5.15 REQUIRED COMPONENTS ...)` 声明。
- 第三方库必须记录来源、版本、许可证和引入原因。
- 构建目录、生成文件和本地工具缓存不得提交到仓库。

示例：

```cmake
cmake_minimum_required(VERSION 3.16)

project(example LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets)
find_package(spdlog REQUIRED)

add_executable(example
    src/app/main.cpp
)

target_link_libraries(example
    PRIVATE
        Qt5::Core
        Qt5::Widgets
        spdlog::spdlog
)
```

## 日志

- 日志实现使用 spdlog。
- 业务模块必须通过项目统一的日志封装记录日志，禁止在业务代码中散落初始化、格式配置和输出目标配置。
- 日志级别必须准确使用。调试信息使用 `debug`，普通运行信息使用 `info`，可恢复异常使用 `warn`，操作失败或系统异常使用 `error`。
- 禁止记录密码、令牌、密钥和完整个人隐私数据。
- 日志初始化、刷新策略、文件轮换和线程模型必须在应用启动和运维文档中明确。
- Domain 层不得直接依赖 GUI 日志输出。需要记录诊断信息时，使用注入的日志接口或项目统一日志封装。

## 第三方库清单

新增第三方库前必须补充下表，并同步更新构建配置：

| 库 | 用途 | 版本 | 来源 | 许可证 | 引入原因 |
| --- | --- | --- | --- | --- | --- |
| spdlog | 应用日志 | 接入时锁定 | [gabime/spdlog](https://github.com/gabime/spdlog) | [MIT](https://github.com/gabime/spdlog/blob/v1.x/LICENSE) | 统一各模块的日志级别、输出格式和文件轮转策略。 |
| nlohmann/json | 纯 C++ 核心层的 JSON 解析与序列化 | 接入时锁定 | [上游仓库](https://github.com/nlohmann/json) | MIT | 核心层可独立于 Qt 处理 JSON。 |
| Qt Advanced Docking System（ADS） | Qt Widgets 工作区的标签页 | 接入时锁定 | [上游仓库](https://github.com/githubuser0xFFFF/Qt-Advanced-Docking-System) | LGPL-2.1 | 管理文档和工具面板的标签分组、拖拽与停靠。 |
| FluentUIStyle | Windows 现代风格界面 | 接入时锁定 | [上游仓库](https://github.com/XHY-ChuJian/FluentUIStyle) | MIT | Qt Widgets 项目需要 Windows 11 Fluent 外观时使用该样式插件。 |

[Qt Advanced Stylesheets](https://github.com/githubuser0xFFFF/Qt-Advanced-Stylesheets) 用于 Qt 样式表和主题管理。标签页停靠功能由上表中的 ADS 提供。

FluentUIStyle 上游已测试 Qt 5.15.2。使用 CMake 3.16 构建时，关闭要求 CMake 3.19 及以上版本的可选无边框组件（`-DBUILD_FRAMELESS=OFF`）。详见[上游注意事项](https://github.com/XHY-ChuJian/FluentUIStyle#注意事项)。

依赖选择必须满足：

- 已有标准库或 Qt 能够满足需求时，不新增第三方库。
- 依赖必须支持项目的 C++17、Qt 5.15.2 和目标平台。
- 依赖必须有明确版本约束，禁止依赖未锁定的浮动版本。
- 依赖许可证必须符合项目发布许可。
- 依赖停止维护、存在高风险漏洞或无法满足平台要求时，必须记录替代方案和迁移计划。

## 工具和质量检查

项目应统一记录以下工具版本：

| 工具 | 用途 | 版本 | 官方链接 |
| --- | --- | --- | --- |
| CMake | 配置和生成构建文件 | `>= 3.16` | [CMake 文档](https://cmake.org/documentation/) |
| 编译器 | 编译 C++17 代码 | `<项目指定版本>` | [MSVC](https://learn.microsoft.com/zh-cn/cpp/) · [GCC](https://gcc.gnu.org/) · [Clang](https://clang.llvm.org/) |
| clang-format | C++/Qt 代码格式化 | `23.1.6` | [ClangFormat 手册](https://clang.llvm.org/docs/ClangFormat.html) |
| clang-tidy | 静态检查 | `<项目指定版本>` | [Clang-Tidy 手册](https://clang.llvm.org/extra/clang-tidy/) |
| Doxygen | API 文档生成 | `<项目指定版本>` | [Doxygen 手册](https://www.doxygen.nl/manual/) |

工具配置必须提交到仓库，并在 CI 中使用相同规则。具体代码、注释和 API 约束分别见 [C++ 编码规范](cpp-coding-guidelines.md)、[C++ 注释规范](cpp-doxygen-introduction.md) 和 [C++ API 设计规范](cpp-api-design-guidelines.md)。

## 变更规则

- 升级 C++ 标准、Qt、CMake 或第三方库时，必须更新本文、构建配置和 CI 配置。
- 影响 ABI、部署环境或公共 API 的升级必须补充迁移说明。
- 依赖版本变更必须在 PR 正文中说明变更原因、兼容性影响和验证结果。
- 技术栈变更遵循 [版本控制规范](../VERSIONING.md)。

