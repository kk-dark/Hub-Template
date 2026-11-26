# C++ / Qt 格式检查

项目固定使用 clang-format 23.1.5。根目录 `.clang-format` 定义排版规则，包括四空格缩进、左花括号换行、指针星号靠类型，以及 `Q_OBJECT`、`Q_PROPERTY` 等 Qt 宏。格式检查覆盖 C++ 源文件和头文件；命名、所有权等设计约定由代码审核检查。

## 本地安装与使用

已安装 Git 和 Python 3.8+ 的开发者可以通过 pip 下载对应平台的 clang-format。已有同版本可执行文件时，将其目录加入 `PATH`，或将环境变量 `CLANG_FORMAT` 设为可执行文件的完整路径。[下载文件](https://pypi.org/project/clang-format/23.1.5/#files) · [LLVM 工具手册](https://clang.llvm.org/docs/ClangFormat.html)

在项目根目录执行：

```shell
python -m pip install clang-format==23.1.5
clang-format --version
python tools/check_format.py
```

检查报告文件和行号。运行 `python tools/check_format.py --fix` 可批量修复，再执行 `git diff` 查看改动。只修复一个文件可用 `clang-format -i src/main.cpp`。脚本扫描 Git 已跟踪及尚未被忽略的新增 C++ 源文件和头文件，包括测试代码。构建目录可通过项目 `.gitignore` 排除。

## 在 GitHub / Gitea 启用

把以下文件提交到目标项目，保留相对路径。

| 文件 | 用途 |
| --- | --- |
| `.clang-format` | 本地、编辑器和 CI 共用的排版配置 |
| `tools/check_format.py` | 查找文件、校验工具版本；默认以 `--dry-run --Werror` 检查 |
| `.github/workflows/clang-format.yml` | GitHub Actions 入口 |
| `.gitea/workflows/clang-format.yml` | Gitea Actions 入口 |

- **GitHub**：在仓库设置中启用 Actions。工作流使用 GitHub 提供的 Ubuntu runner。
- **Gitea**：在实例和仓库中启用 Actions，并注册具有 `ubuntu-latest` 标签的 runner。该标签对应的执行环境需要 Git、Node.js 20（运行 `checkout@v4`）、Python 3、`venv` 和 pip，并能下载 checkout action 和 PyPI 安装包。[Gitea Actions 快速入门](https://docs.gitea.com/usage/actions/quickstart)

推送分支或创建、更新 PR 时，CI 安装固定版本并运行同一检查脚本。任一文件排版不符，`clang-format` 作业即失败。若要求失败时禁止合并，首次运行后，在目标分支的保护规则中启用必需状态检查，选择此次运行产生的 `clang-format` 状态。[GitHub 分支保护](https://docs.github.com/en/repositories/configuring-branches-and-merges-in-your-repository/managing-protected-branches/about-protected-branches) · [Gitea 分支保护](https://docs.gitea.com/usage/access-control/protected-branches)
