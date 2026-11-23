# 贡献指南

## 提交 议题/工单 (Issue)

提交前先查看是否已有相关讨论。

- 报告缺陷时说明实际现象，并附上版本和复现步骤。
- 提出功能建议时说明遇到的问题和期望的变化。
- 安排工程任务时写明目标和完成标准。
- 疑似安全漏洞请私下联系维护者。

## 提交 评论 (Commit) 和 合并请求 (Pull Requests,PR) 标题

### 格式

本仓库采用 [Conventional Commits 1.0.0](https://www.conventionalcommits.org/zh-hans/v1.0.0/)。Commit 和 PR 使用同一套标题格式：

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

### 类型

类型使用小写英文，范围表示受影响的模块，可以省略。描述应说明具体变化，正文用于补充背景和实现细节，脚注用于记录关联议题或破坏性变更。

| 改动类型 | 示例 |
| --- | --- |
| 新增功能 | `feat(export): 支持按日期导出订单` |
| 修复问题 | `fix(export): 修复导出时遗漏订单的问题` |

除 `fix` 和 `feat` 外，也可以使用以下提交类型：

| 改动类型 | 示例 |
| --- | --- |
| 构建系统 | `build: 升级 Node.js 版本` |
| 工程维护 | `chore: 更新工具配置` |
| 持续集成 | `ci: 调整 GitHub Actions 工作流` |
| 更新文档 | `docs: 更正安装命令` |
| 代码样式 | `style: 统一缩进格式` |
| 重构实现 | `refactor(export): 提取日期解析函数` |
| 性能改进 | `perf(export): 减少数据库查询` |
| 更新测试 | `test(export): 覆盖跨月导出场景` |

类型参考
[commitlint 的 Conventional 配置](https://github.com/conventional-changelog/commitlint/tree/master/%40commitlint/config-conventional)
和 [Angular 提交约定](https://github.com/angular/angular/blob/22b96b9/CONTRIBUTING.md#-commit-message-guidelines)。


### 示例

破坏兼容性时在类型或 scope 后加 `!`，并在提交信息末尾说明迁移办法：

```text
feat(api)!: 将导出接口改为异步任务

BREAKING CHANGE: 导出请求现在返回任务 ID。
```

更多示例见 [更多 Commit 案例](docs/more-commit-examples.md)。

### 回滚

如果提交用于撤销之前的提交，应以 `revert: ` 开头，后面接被撤销提交的标题。正文应写明：

```text
This reverts commit <hash>.
```

其中 `<hash>` 是被撤销提交的 SHA。例如：

```text
revert: feat(export): 支持按日期导出订单

This reverts commit 4f8c2a1.
```

## 提交 PR

PR 标题概括整组改动的结果，类型按主要目的确定。

小修正可以直接提 PR。较大功能、公共接口或数据结构变更、新增重要依赖，请先通过 Issue 确认范围。

一个 PR 解决一个问题。正文简要说明动机、最终行为和验证结果。未执行的检查、数据迁移风险及恢复办法也应一并说明。关联议题时使用真实编号，例如 `Closes #123`。

### 格式

PR 正文可以参考以下模板：

```markdown
## 变更简述

说明改动动机和最终行为。

## 关联议题

关联讨论或需求使用 `#123`，完整解决时使用 `Closes #123`。

## 验证结果

说明已执行的测试、检查或手工验证。未执行的项目请说明原因。

## 评审人

@reviewer
```

### 示例

```markdown
标题：feat(export): 支持按日期导出订单

之前只能导出全部订单，现在可以指定起止日期，首尾两天都包含在内。已验证同一天作为起止日期时只导出当天订单。

Closes #123
```

如果 PR 只引用 Issue、暂不表示已解决，可以直接写 Issue 编号：

```markdown
标题：docs(export): 补充按日期导出的用法

补充日期导出的参数说明，供相关讨论参考。

#456
```

PR 默认由至少一名具备权限的维护者独立完成人工审核后合并。

## 多条 commit 汇总为一个 PR

假设一个功能由以下 commit 组成：

```text
feat(export): 支持按起止日期筛选订单
test(export): 覆盖起止日期相同的情况
docs(export): 补充按日期导出的用法
```

PR 标题应概括整组改动：

```text
feat(export): 支持按日期导出订单
```

PR 标题按整组改动的主要目的确定。实现、测试和文档分别提交时，PR 标题仍应概括最终合并后的结果。

## 版本控制规范

本仓库采用 [Semantic Versioning 2.0.0](https://semver.org/spec/v2.0.0.html)，版本号格式为：

```text
MAJOR.MINOR.PATCH
```

### 版本号递增

- **MAJOR**：公共接口或使用方式发生不兼容变更。
- **MINOR**：以向后兼容的方式新增功能。
- **PATCH**：以向后兼容的方式修复问题。

弃用功能时，先在文档中说明，并通过次版本发布弃用提示。删除或改变已弃用功能时，再递增主版本。

### 预发布版本和构建信息

预发布版本用于发布候选版本或实验版本，例如 `1.2.0-rc.1`。预发布版本的优先级低于对应的正式版本。

构建信息用于记录构建环境或流水线信息，例如 `1.2.0+build.42`。构建信息不影响版本优先级。

### 发布约定

- 正式版本使用 `vMAJOR.MINOR.PATCH` 格式创建 Git 标签，例如 `v1.2.0`。
- 预发布版本的标签须保留预发布标识，例如 `v1.2.0-rc.1`。
- 发布前确认变更已记录在发布说明中，并核对版本号与实际改动一致。

版本号遵循 SemVer 规则，具体比较方式和格式要求以 [Semantic Versioning 2.0.0](https://semver.org/lang/zh-CN/) 为准。
