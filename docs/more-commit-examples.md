# 更多 Commit 案例

## 包含描述并且脚注中有破坏性变更

```yaml
feat: allow provided config object to extend other configs

BREAKING CHANGE: `extends` key in config file is now used for extending other config files
```

## 使用 `!` 提醒注意破坏性变更

```vbnet
feat!: send an email to the customer when a product is shipped
```

## 包含范围和破坏性变更 `!`

```vbnet
feat(api)!: send an email to the customer when a product is shipped
```

## 同时包含 `!` 和 `BREAKING CHANGE` 脚注

```rust
feat!: drop support for Node 6

BREAKING CHANGE: use JavaScript features not available in Node 6.
```

## 不包含正文

```yaml
docs: correct spelling of CHANGELOG
```

## 包含范围

```scss
feat(lang): add polish language
```

## 包含多行正文和多行脚注

```yaml
fix: prevent racing of requests

Introduce a request id and a reference to latest request. Dismiss
incoming responses other than from latest request.

Remove timeouts which were used to mitigate the racing issue but are
obsolete now.

Reviewed-by: Z
Refs: #123
```

