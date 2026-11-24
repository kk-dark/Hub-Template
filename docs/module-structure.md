# 目录结构

本文说明仓库当前的目录和文件职责。新增文件时，应放入与其用途对应的目录，并同步更新本文的目录树和说明。

当前仓库以项目规范和协作配置为主，尚未设置 `src/`、`include/`、`tests/` 等源码目录。项目开始包含实现代码后，应按模块职责补充目录，并同步更新本文。

## 目录树

```text
.
├── .github/
│   ├── ISSUE_TEMPLATE/
│   │   ├── Bug提交.md
│   │   ├── 功能建议.md
│   │   ├── 维护任务.md
│   │   └── 文档更新.md
│   ├── workflows/
│   └── pull_request_template.md
├── docs/
│   ├── commit-and-pr-examples.md
│   ├── cpp-coding-guidelines.md
│   ├── cpp-doxygen-introduction.md
│   ├── cpp-doxygen-reference.md
│   ├── module-structure.md
│   ├── qt-doxygen-guidelines.md
│   └── qt-coding-guidelines.md
├── CONTRIBUTING.md
├── README.md
└── VERSIONING.md
```

## 根目录文件

| 文件 | 用途 |
| --- | --- |
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
| `commit-and-pr-examples.md` | Commit 类型、Commit 标题和 PR 正文的完整案例。 |
| `cpp-coding-guidelines.md` | 通用 C++ 编码规范。 |
| `cpp-doxygen-introduction.md` | 面向日常使用的精简 Doxygen 注释规范。 |
| `cpp-doxygen-reference.md` | Doxygen 关键字和高级用法的详细参考。 |
| `qt-doxygen-guidelines.md` | Qt 信号、槽、属性、模型和界面相关的注释规范。 |
| `qt-coding-guidelines.md` | Qt 5.15.2 和 Qt 6 的专属编码规范。 |
| `module-structure.md` | 仓库目录结构和文件职责说明。 |

规范之间的关系如下：

```text
CONTRIBUTING.md
├── 提交和 PR 规则
├── VERSIONING.md
├── docs/cpp-coding-guidelines.md
│   ├── docs/cpp-doxygen-introduction.md
│   └── docs/cpp-doxygen-reference.md
└── docs/qt-coding-guidelines.md
    ├── 复用 C++ 编码规范
    ├── 复用 C++ Doxygen 注释规范
    └── docs/qt-doxygen-guidelines.md
        └── 补充 Qt 特有的注释语义
```

## 新增文件的放置规则

- 新增协作配置放入 `.github/` 下对应目录。
- 新增规范、参考资料和较长的使用说明放入 `docs/`。
- 根目录只保留项目入口文档和仓库级管理文件。
- 新增目录或文件后，必须更新本文的目录树和文件说明。
