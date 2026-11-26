# 贡献指南

## 提交 Issue

提交前先查看是否已有相关讨论。

- 报告缺陷时说明实际现象，并附上版本和复现步骤。
- 提出功能建议时说明遇到的问题和期望的变化。
- 安排工程任务时写明目标和完成标准。
- 疑似安全漏洞请私下联系维护者。

## Commit 和 PR 规范

本仓库采用 [Conventional Commits 1.0.0](https://www.conventionalcommits.org/zh-hans/v1.0.0/)。Commit 和 PR 标题均使用以下格式：

原文：

```swift
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

译文：

```text
<类型>[可选 范围]: <描述>

[可选 正文]

[可选 脚注]
```

类型使用小写英文，范围表示受影响的模块，可以省略。描述应说明具体变化，正文用于补充背景和实现细节，脚注用于记录关联议题或破坏性变更。

| 改动类型 | 示例 |
| --- | --- |
| 新增功能 | `feat(export): 支持按日期导出订单` |
| 修复问题 | `fix(export): 修复导出时遗漏订单的问题` |

更多提交类型、Commit 和 PR 示例见 [Commit 与 PR 案例](docs/commit-and-pr-examples.md)。

破坏兼容性时，在类型后或 scope 后加 `!`，并在提交信息中使用 `BREAKING CHANGE:` 说明迁移办法：

```text
feat(api)!: 将导出接口改为异步任务

BREAKING CHANGE: 导出请求现在返回任务 ID。
```

如果提交用于撤销之前的提交，应以 `revert: ` 开头，后面接被撤销提交的标题。正文应写明 `This reverts commit <hash>.`，其中 `<hash>` 是被撤销提交的 SHA。

## 提交 PR

PR 标题概括整组改动的最终结果，类型按主要目的确定。一个 PR 解决一个问题。较大功能、公共接口或数据结构变更、新增重要依赖，请先通过 Issue 确认范围。

PR 正文应说明变更动机、最终行为和验证结果。仅引用 Issue 时填写编号，完整解决 Issue 时使用 `Fixes #123`。PR 合并到默认分支后，GitHub 会自动关闭对应 Issue。

PR 正文模板：

```markdown
## 变更简述

## 关联议题
Closes #
```

PR 默认由至少一名具备权限的维护者独立完成人工审核后合并。

## 版本号规范

版本号规则见 [版本控制规范](VERSIONING.md)。

## 编码规范

提交 C++ 或 Qt 代码前，运行 `python tools/check_format.py`。本地安装、自动修复和 CI 配置见 [clang-format 使用说明](docs/clang-format.md)。

C++ 编码规范见 [C++ 编码规范](docs/cpp-coding-guidelines.md)。

公共 C++ 头文件 API 设计见 [C++ API 设计规范](docs/cpp-api-design-guidelines.md)。

Qt 公共 API 设计见 [Qt API 设计规范](docs/qt-api-design-guidelines.md)。

C++ 注释规范见 [C++ 注释规范](docs/cpp-doxygen-introduction.md)。

项目分层、依赖方向和模块边界见 [Qt 项目架构规范](docs/architecture.md)。

语言、Qt、CMake 和第三方库基线见 [技术栈规范](docs/technology-stack.md)。

Qt 编码规范见 [Qt 编码规范](docs/qt-coding-guidelines.md)。

Qt 注释规范见 [Qt 注释规范](docs/qt-doxygen-guidelines.md)。
