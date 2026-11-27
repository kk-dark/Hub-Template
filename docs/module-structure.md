# 目录结构

本文说明仓库当前的目录和文件职责。新增文件时，应放入与其用途对应的目录，并同步更新本文的目录树和说明。

当前仓库以项目规范和协作配置为主，尚未设置 `src/`、`include/` 等源码目录。`test/format_check.cpp`、`test/qt_format_check.cpp` 和 `test/qt_special_format_check.cpp` 是用于验证 CI 格式检查的样例，不属于产品实现或正式测试代码。

## 目录树

```text
.
├── .clang-format
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── Bug提交.md
│   │   ├── 功能建议.md
│   │   ├── 维护任务.md
│   │   └── 文档更新.md
│   ├── workflows/
│   │   └── clang-format.yml
│   └── pull_request_template.md
├── docs/
│   ├── architecture.md
│   ├── changelog-template.md
│   ├── clang-format.md
│   ├── commit-and-pr-examples.md
│   ├── cpp-api-design-guidelines.md
│   ├── cpp-coding-guidelines.md
│   ├── cpp-doxygen-introduction.md
│   ├── cpp-doxygen-reference.md
│   ├── module-structure.md
│   ├── qt-api-design-guidelines.md
│   ├── qt-doxygen-guidelines.md
│   ├── qt-coding-guidelines.md
│   └── technology-stack.md
├── CONTRIBUTING.md
├── test/
│   ├── format_check.cpp
│   ├── qt_format_check.cpp
│   └── qt_special_format_check.cpp
├── tools/check_format.py
├── README.md
└── VERSIONING.md
```

## 根目录文件

| 文件 | 用途 |
| --- | --- |
| `.clang-format` | C++ 和 Qt 代码的统一排版配置，供本地工具、编辑器和 CI 共用。 |
| `README.md` | 项目简介、使用说明和快速入口。 |
| `CONTRIBUTING.md` | Issue、Commit、PR 和编码规范的入口说明。 |
| `VERSIONING.md` | 版本号和发布版本的管理规则。 |

## `.github/`

`.github/` 保存 GitHub 仓库协作配置。

| 路径 | 用途 |
| --- | --- |
| `.github/pull_request_template.md` | 创建 PR 时使用的默认正文模板。 |
| `.github/ISSUE_TEMPLATE/` | Issue 模板，按缺陷、功能建议、维护任务和文档更新分类。 |
| `.github/workflows/` | GitHub Actions 工作流。新增或修改持续集成流程时使用。 |

Issue 模板只负责收集问题信息，具体提交和 PR 规则见 [贡献指南](../CONTRIBUTING.md)。

## `docs/`

`docs/` 保存需要独立阅读或持续维护的规范和参考资料。

| 文件 | 用途 |
| --- | --- |
| `architecture.md` | Qt 项目的分层、依赖方向、模块边界和测试约束。 |
| `changelog-template.md` | 版本发布说明和附件列表的模板。 |
| `commit-and-pr-examples.md` | Commit 类型、Commit 标题和 PR 正文的完整案例。 |
| `cpp-api-design-guidelines.md` | 通用 C++ 公共头文件 API 的设计、ABI 和兼容性规范。 |
| `cpp-coding-guidelines.md` | 通用 C++ 编码规范。 |
| `cpp-doxygen-introduction.md` | 面向日常使用的精简 Doxygen 注释规范。 |
| `cpp-doxygen-reference.md` | Doxygen 关键字和高级用法的详细参考。 |
| `qt-doxygen-guidelines.md` | Qt 信号、槽、属性、模型和界面相关的注释规范。 |
| `qt-api-design-guidelines.md` | Qt 公共 API 对 QObject、元对象、线程和 Qt 类型的补充规范。 |
| `qt-coding-guidelines.md` | Qt 5.15.2 和 Qt 6 的专属编码规范。 |
| `technology-stack.md` | C++、Qt、CMake、第三方库和工具版本基线。 |
| `module-structure.md` | 仓库目录结构和文件职责说明。 |
| `clang-format.md` | clang-format 排版规则、本地检查方法和 CI 配置说明。 |

`tools/check_format.py` 位于 `docs/` 同级的 `tools/` 目录，用于使用固定版本 clang-format 检查或修复仓库中的 C++ 和 Qt 文件。

规范之间的关系如下：

```text
CONTRIBUTING.md
├── 提交和 PR 规则
├── VERSIONING.md
├── docs/cpp-api-design-guidelines.md
│   └── 通用公共 C++ API 规则
├── docs/cpp-coding-guidelines.md
│   ├── docs/cpp-doxygen-introduction.md
│   └── docs/cpp-doxygen-reference.md
└── docs/qt-api-design-guidelines.md
    ├── 复用 C++ API 设计规范
    ├── 复用 Qt 编码规范
    └── 复用 Qt 注释规范
```

## 新增文件的放置规则

- 新增协作配置放入 `.github/` 下对应目录。
- 新增规范、参考资料和较长的使用说明放入 `docs/`。
- 根目录只保留项目入口文档和仓库级管理文件。
- 新增目录或文件后，必须更新本文的目录树和文件说明。
