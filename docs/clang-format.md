# C++ / Qt 格式检查

项目使用 LLVM 23 系列的 clang-format。根目录 `.clang-format` 以 LLVM 风格为基线，统一 C++17 和 Qt 代码的排版。格式检查覆盖 C++ 源文件和头文件；命名、所有权等设计约定由代码审核检查。

## 排版规则

`.clang-format` 当前明确设置以下规则，其余选项继承 LLVM 默认值：

| 配置 | 约定 |
| --- | --- |
| `BasedOnStyle: LLVM` | 以 LLVM 风格为基线 |
| `Standard: c++17` | 按 C++17 语法解析 |
| `IndentWidth: 4` | 使用四个空格缩进 |
| `AccessModifierOffset: -4` | 访问控制标签相对类体左移四格 |
| `ColumnLimit: 120` | 单行目标宽度为 120 列 |
| `BreakBeforeBraces: Attach` | 左花括号与控制语句或声明同行 |
| `PointerAlignment: Right` | 指针星号靠近变量名，例如 `int *value` |
| `DerivePointerAlignment: false` | 不根据已有代码推断指针风格 |
| `SortIncludes: Never` | 不自动调整 `#include` 顺序 |
| `AllowShortFunctionsOnASingleLine: Inline` | 仅允许类内联短函数保持单行 |
| `AllowShortIfStatementsOnASingleLine: Never` | `if` 语句体始终换行 |

`BinPackArguments` 和 `BinPackParameters` 当前未显式设置，继续使用 LLVM 默认行为。Qt 宏的语义由编译器和 Qt 元对象工具处理，排版仍遵循上述通用规则。

## 本地安装与使用

已安装 Git 和 Python 3.8+ 的开发者可以通过 pip 下载 clang-format 23.1.6。已有同版本可执行文件时，将其目录加入 `PATH`，或将环境变量 `CLANG_FORMAT` 设为可执行文件的完整路径。[下载文件](https://pypi.org/project/clang-format/23.1.6/#files) · [LLVM 工具手册](https://clang.llvm.org/docs/ClangFormat.html)

在项目根目录执行：

```shell
python -m pip install clang-format==23.1.6
clang-format --version
python tools/check_format.py
```

检查报告文件和行号。运行 `python tools/check_format.py --fix` 可批量修复，再执行 `git diff` 查看改动。只修复一个文件可用 `clang-format -i src/main.cpp`。脚本扫描 Git 已跟踪及尚未被忽略的新增 C++ 源文件和头文件，包括测试代码。构建目录可通过项目 `.gitignore` 排除。

## 在 GitHub 启用

把以下文件提交到目标项目，保留相对路径。

| 文件 | 用途 |
| --- | --- |
| `.clang-format` | 本地、编辑器和 CI 共用的排版配置 |
| `tools/check_format.py` | 查找文件、校验工具版本；默认以 `--dry-run --Werror` 检查 |
| `.github/workflows/clang-format.yml` | GitHub Actions 入口，使用 LLVM 23 的 `cpp-linter/cpp-linter-action@v2` |

- **GitHub**：在仓库设置中启用 Actions。工作流使用 GitHub 提供的 Ubuntu runner。

GitHub 工作流只在 PR 事件上运行，使用 Ubuntu runner、`actions/checkout@v7` 和 LLVM 23。它开启 `format-review: true`，将格式问题作为文件审查注释发布。任一文件排版不符，`checks-failed` 大于 0 时作业失败。工作流需要 `contents: read` 和 `pull-requests: write` 权限。若要求失败时禁止合并，在目标分支保护规则中启用该工作流的必需状态检查。[GitHub 分支保护](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches)

GitHub 工作流使用 [C/C++ Linter Action](https://github.com/marketplace/actions/c-c-linter) 的 `cpp-linter/cpp-linter-action@v2`。`style: file` 读取仓库根目录的 `.clang-format`。当前配置保留 `tidy-checks: ''`，并通过 `checks-failed` 判断格式检查是否失败。[Action 输入和输出](https://cpp-linter.github.io/cpp-linter-action/)

本地脚本可用于提交前检查全部已跟踪及新增文件，GitHub Action 默认检查变更涉及的文件。两者共用根目录 `.clang-format`。
