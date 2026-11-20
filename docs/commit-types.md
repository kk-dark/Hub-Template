# 提交类型案例

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

## 示例

### 包含描述并且脚注中有破坏性变更

```yaml
feat: allow provided config object to extend other configs

BREAKING CHANGE: `extends` key in config file is now used for extending other config files
```

### 使用 `!` 提醒注意破坏性变更

```vbnet
feat!: send an email to the customer when a product is shipped
```

### 包含范围和破坏性变更 `!`

```vbnet
feat(api)!: send an email to the customer when a product is shipped
```

### 同时包含 `!` 和 `BREAKING CHANGE` 脚注

```rust
feat!: drop support for Node 6

BREAKING CHANGE: use JavaScript features not available in Node 6.
```

### 不包含正文

```yaml
docs: correct spelling of CHANGELOG
```

### 包含范围

```scss
feat(lang): add polish language
```

### 包含多行正文和多行脚注

```yaml
fix: prevent racing of requests

Introduce a request id and a reference to latest request. Dismiss
incoming responses other than from latest request.

Remove timeouts which were used to mitigate the racing issue but are
obsolete now.

Reviewed-by: Z
Refs: #123
```

## PR 案例

### 新增功能

**标题：** `feat(export): 支持按日期导出订单`

> 之前只能导出全部订单，现在可以指定起止日期，首尾两天都包含在内。已验证日期边界和空结果场景。
>
> Closes #123

### 修复问题

**标题：** `fix(export): 导出时包含截止日订单`

> 修复日期范围查询未包含截止日的问题。已补充截止日与跨月场景测试。
>
> Closes #456

### 仅供参考

**标题：** `docs(export): 补充按日期导出的用法`

> 补充日期导出的参数说明和示例，供相关讨论参考。
>
> #789

### 多条 commit 汇总为一个 PR

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

PR 正文可以这样写：

```markdown
## 变更简述

之前只能导出全部订单，现在可以指定起止日期，首尾两天都包含在内。

## 关联议题

Closes #123

## 验证结果

已测试起止日期相同、跨月和无结果三种场景。

## 评审人

@reviewer
```

PR 标题按整组改动的主要目的确定。实现、测试和文档分别提交时，PR 标题仍应概括最终合并后的结果。
