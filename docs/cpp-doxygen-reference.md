# C++ Doxygen 注释参考

日常开发优先阅读[精简版 C++ 注释规范](cpp-doxygen-introduction.md)。本文用于查询完整关键字、特殊输出格式和进阶用法。

本项目使用 Doxygen 注释生成 C++ API 文档。公共头文件中的类、结构体、枚举、类型别名、命名空间、函数、成员函数、成员变量和宏应提供 Doxygen 注释。

规范优先使用 `/** ... */` 块注释。行较短的成员说明可以使用 `///<` 或 `//!<`。同一项目应统一使用反斜杠命令或 `@` 命令，本项目统一使用 `@` 命令。

## 完整注释模板

下面的模板覆盖项目约定中可能使用的 Doxygen 关键字。每个实体只保留适用的字段，不要为了填满模板而添加空字段。

```cpp
/**
 * @file example.hpp
 * @brief 文件的一句话说明。
 *
 * @details 文件的详细说明，包括职责、线程模型和使用限制。
 *
 * @author Author Name
 * @date 2026-11-23
 * @version 1.0.0
 * @copyright Copyright (c) 2026
 *
 * @defgroup example_group Example 模块
 * @ingroup parent_group
 * @{
 */

/**
 * @brief 类型的一句话说明。
 *
 * @details 类型的详细说明。
 *
 * @tparam T 模板类型参数。
 * @tparam Allocator 分配器类型。
 *
 * @note 使用注意事项。
 * @attention 需要特别注意的事项。
 * @warning 可能导致错误或风险的条件。
 * @remark 补充说明。
 * @deprecated 请使用 NewType。
 * @since 1.2.0
 *
 * @see RelatedType
 * @sa related_function()
 * @implements Interface
 * @extends BaseType
 *
 * @class Example
 * @struct ExampleData
 * @union ExampleUnion
 * @enum ExampleState
 * @typedef ExampleAlias
 * @namespace example
 */
class Example
{
public:
    /**
     * @brief 构造对象。
     *
     * @param[in] value 初始值。
     * @param[in] name 对象名称。
     *
     * @pre value 必须大于等于零。
     * @post 对象处于可用状态。
     * @throws std::invalid_argument 参数无效时抛出。
     */
    Example(int value, std::string name);

    /**
     * @brief 执行操作。
     *
     * @param[in,out] input 输入并被更新的数据。
     * @param[in] count 要处理的元素数量。
     *
     * @return 操作是否成功。
     * @retval true 操作成功。
     * @retval false 操作失败。
     *
     * @exception std::runtime_error 执行失败时抛出。
     * @throws std::runtime_error 同上，推荐统一使用其中一种写法。
     *
     * @invariant 对象状态在调用前后保持有效。
     * @requires 调用方已经完成初始化。
     * @ensures 返回 true 时所有元素均已处理。
     *
     * @see reset()
     * @todo 增加异步执行接口。
     * @bug 当前版本在空输入下存在性能问题。
     */
    bool process(std::vector<int>& input, std::size_t count);

private:
    int value_; ///< 当前值。
    std::string name_; //!< 对象名称。
};

/**
 * @brief 公共函数的一句话说明。
 *
 * @param[in] path 输入文件路径。
 * @param[out] output 输出数据。
 *
 * @return 读取结果。
 * @retval true 读取成功。
 * @retval false 文件不存在或格式无效。
 *
 * @pre path 不为空。
 * @post output 仅在返回 true 时有效。
 */
bool load_data(std::string_view path, Data& output);

/**
 * @brief 示例测试。
 *
 * @test 验证正常输入。
 * @test 验证空输入。
 * @example examples/basic.cpp
 */

/**
 * @brief 代码示例。
 *
 * @code{.cpp}
 * Example example(1, "demo");
 * @endcode
 *
 * @verbatim
 * 原样输出的文本，不解析 Doxygen 命令。
 * @endverbatim
 */

/**
 * @brief 页面或分组的补充说明。
 *
 * @par 线程安全
 * 多个线程可以并发读取，写入操作需要外部同步。
 *
 * @anchor example_anchor
 * 可通过 @ref example_anchor 跳转到这里。
 *
 * @image html example.png "示意图"
 * @dot
 * digraph ExampleGraph { A -> B; }
 * @enddot
 */

/** @} */
```

模板中的 `@class`、`@struct`、`@fn`、`@namespace` 等结构命令通常由 Doxygen 根据代码自动识别。只有在注释与实体分离，或需要明确指定文档归属时才单独使用。

## 关键字说明

命令可以使用 `@` 或 `\` 开头，例如 `@brief` 与 `\brief` 等价。结构命令用于把注释绑定到文件、类、函数或分组，通常只有在注释不紧邻实体时才需要显式写出。完整命令及参数规则以 [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html) 为准。

### 结构和归属

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@file` | 将注释绑定到源文件或头文件。 | 文件包含全局函数、变量、类型或宏的公共说明时使用。 |
| `@class` | 指定注释描述一个类。 | 注释不紧邻类定义，或需要指定头文件路径时使用。 |
| `@struct` | 指定注释描述一个结构体。 | 注释不紧邻结构体定义时使用。 |
| `@union` | 指定注释描述一个联合体。 | 注释不紧邻联合体定义时使用。 |
| `@enum` | 指定注释描述一个枚举。 | 需要单独说明枚举整体含义时使用。 |
| `@fn` | 指定注释描述一个函数或成员函数。 | 注释与函数声明分离时使用，紧邻声明时通常省略。 |
| `@var` | 指定注释描述变量或枚举值。 | 注释与变量分离，或需要单独描述枚举值时使用。 |
| `@def` | 指定注释描述 `#define` 宏。 | 宏的行为、参数和副作用需要生成文档时使用。 |
| `@typedef` | 指定注释描述类型别名。 | 类型别名注释与声明分离时使用。 |
| `@namespace` | 指定注释描述命名空间。 | 命名空间需要模块级说明时使用。 |
| `@interface` | 指定注释描述接口。 | 使用 Doxygen 的接口模型，或需要单独标识接口时使用。 |
| `@package` | 指定注释描述一个包。 | 主要用于 Java 等带包概念的语言，C++ 项目通常不使用。 |
| `@dir` | 指定注释描述目录。 | 需要为目录生成独立说明页时使用。 |
| `@headerfile` | 指定使用者应包含的头文件。 | 定义和声明分离，或公共头文件路径不明显时使用。 |
| `@defgroup` | 定义一个文档分组。 | 为模块、主题或一组 API 创建唯一分组时使用。 |
| `@addtogroup` | 向已有分组追加内容。 | 多个文件共同补充分组文档时使用。 |
| `@ingroup` | 将实体加入一个或多个分组。 | 类、文件、命名空间或成员需要出现在某个模块分组中时使用。 |
| `@weakgroup` | 定义可合并的弱分组。 | 允许多个位置使用同一分组名且不触发重复警告时使用。 |
| `@group` | 标记分组范围。 | 与 `@{`、`@}` 配合，将连续声明归入一个分组时使用。 |
| `@mainpage` | 定义文档首页。 | 项目需要自定义 API 文档首页时使用。 |
| `@page` | 定义一个独立文档页。 | 编写使用指南、设计说明或专题页面时使用。 |
| `@subpage` | 引用子页面。 | 将多个 `@page` 页面组织成层级结构时使用。 |
| `@section` | 定义一级章节。 | 在页面或详细说明中建立章节时使用。 |
| `@subsection` | 定义二级章节。 | 在 `@section` 下继续分层时使用。 |
| `@subsubsection` | 定义三级章节。 | 只有在二级章节仍需细分时使用。 |
| `@paragraph` | 定义段落标题。 | 需要比 `@subsubsection` 更轻量的局部标题时使用。 |
| `@anchor` | 创建可跳转的锚点。 | 需要让 `@ref` 或外部链接跳到指定位置时使用。 |
| `@relates` | 将非成员函数放入类的相关函数区域。 | 函数与某个类强相关，但不是类成员时使用。 |
| `@related` | `@relates` 的等价写法。 | 需要兼容已有写法时使用。 |
| `@relatesalso` | 将非成员函数同时保留在原位置和类的相关函数区域。 | 函数既要出现在全局文档，又要出现在相关类中时使用。 |
| `@relatedalso` | `@relatesalso` 的等价写法。 | 需要兼容已有写法时使用。 |
| `@implements` | 手动声明实现关系。 | 语言本身无法表达接口实现关系时使用，普通 C++ 继承通常不需要。 |
| `@extends` | 手动声明继承关系。 | 语言本身无法表达继承关系时使用，普通 C++ 继承通常不需要。 |
| `@showinitializer` | 显示变量或枚举值的初始化表达式。 | 初始化值对使用者有意义时使用。 |
| `@showdate` | 显示文档生成日期。 | 项目确实需要显示生成日期时使用，默认不建议添加。 |

### 描述、元数据和状态

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@brief` | 一句话摘要。 | 所有公共实体都应优先提供。 |
| `@details` | 详细说明。 | `@brief` 无法完整说明行为、约束或生命周期时使用。 |
| `@short` | `@brief` 的别名。 | 项目已有该写法时使用，新代码统一使用 `@brief`。 |
| `@author` | 作者信息。 | 项目明确要求记录作者时使用，否则通常省略。 |
| `@date` | 创建或更新日期。 | 需要记录文档时间时使用，避免因为频繁改动造成维护负担。 |
| `@version` | API 或文档版本。 | 该实体有独立版本要求时使用。 |
| `@copyright` | 版权信息。 | 文件或模块需要单独声明版权时使用。 |
| `@since` | 引入版本。 | 公共 API 首次可用版本明确时使用。 |
| `@deprecated` | 标记已废弃 API，并说明替代方案。 | API 仍可用但不应继续使用时使用。 |
| `@note` | 普通注意事项。 | 补充使用者容易遗漏的前提或行为时使用。 |
| `@attention` | 需要特别关注的事项。 | 事项可能导致误用或严重后果时使用。 |
| `@warning` | 风险或危险行为提示。 | 输入、线程、资源或安全风险需要明确提醒时使用。 |
| `@remark` | 补充备注。 | 需要补充背景但不属于约束或警告时使用。 |
| `@remarks` | `@remark` 的复数形式。 | 与现有文档风格保持一致时使用。 |
| `@bug` | 已知缺陷。 | 缺陷需要公开记录，并可关联 Issue 时使用。 |
| `@todo` | 待完成事项。 | 事项具体、可执行，且确实属于文档范围时使用。 |
| `@test` | 测试说明或测试用例引用。 | 需要把 API 文档与测试场景关联时使用。 |
| `@invariant` | 调用前后必须持续成立的条件。 | 描述对象状态不变量时使用。 |
| `@pre` | 调用前置条件。 | 调用方必须满足某条件时使用。 |
| `@post` | 调用后置条件。 | 函数完成后保证某结果时使用。 |
| `@requires` | 要求或前置条件。 | 需要使用契约风格表达约束时使用，项目内与 `@pre` 选一种。 |
| `@ensures` | 保证或后置条件。 | 需要使用契约风格表达结果时使用，项目内与 `@post` 选一种。 |

### 参数、返回值和异常

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@param` | 函数参数说明。 | 公共函数的每个重要参数都应说明。 |
| `@tparam` | 模板参数说明。 | 模板类型参数或非类型参数需要约束说明时使用。 |
| `@return` | 返回值说明。 | 函数有返回值且需要解释时使用。 |
| `@returns` | `@return` 的别名。 | 项目已有该写法时使用，新代码统一使用 `@return`。 |
| `@result` | 返回结果说明。 | 与既有文档风格一致时使用，新代码优先 `@return`。 |
| `@retval` | 针对某个具体返回值的说明。 | `bool`、枚举或错误码的不同结果需要分别解释时使用。 |
| `@exception` | 异常类型和抛出条件。 | 公共接口可能抛出异常时使用。 |
| `@throw` | `@exception` 的别名。 | 项目已有该写法时使用。 |
| `@throws` | `@exception` 的别名。 | 项目已有该写法时使用。 |

参数方向写在 `@param` 后面：

```cpp
@param[in] input 只读输入参数。
@param[out] output 输出参数。
@param[in,out] buffer 输入并被更新的参数。
```

### 引用、复制和重载

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@ref` | 创建到实体或锚点的引用。 | 引用类、函数、页面或自定义锚点时使用。 |
| `@see` | 添加相关实体或资料。 | 需要引导读者查看相关 API 时使用。 |
| `@sa` | `@see` 的别名。 | 项目已有该写法时使用。 |
| `@cite` | 引用文献或参考资料。 | 需要引用 bibliography 条目时使用。 |
| `@link` / `@endlink` | 创建带自定义文字的链接。 | 链接文字不能直接使用实体名称时使用。 |
| `@copydoc` | 复制另一实体的完整文档。 | 重载或包装接口需要复用同一份说明时使用。 |
| `@copybrief` | 只复制摘要。 | 多个接口共享同一句简短说明时使用。 |
| `@copydetails` | 只复制详细说明。 | 共享详细行为但摘要不同的接口时使用。 |
| `@overload` | 标记重载函数并复用相关说明。 | 重载只改变参数形式，行为说明相同时使用。 |
| `@nosubgrouping` | 禁止自动子分组。 | 类成员不希望按访问级别或类型自动分组时使用。 |

### 代码、文件和示例

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@code` / `@endcode` | 保留格式显示代码块。 | 展示短小、可复制的代码时使用。 |
| `@verbatim` / `@endverbatim` | 原样显示文本，不解析命令。 | 展示日志、配置或包含 Doxygen 符号的文本时使用。 |
| `@include` | 将文件内容插入文档并解析其中的 Doxygen 命令。 | 示例文件需要参与文档解析时使用。 |
| `@dontinclude` | 读取示例文件但暂不输出。 | 与 `@line`、`@skip`、`@until` 配合截取代码时使用。 |
| `@verbinclude` | 原样插入文件内容。 | 插入完整日志或配置文件时使用。 |
| `@line` | 输出示例文件的指定行。 | 只展示示例中的某一行时使用。 |
| `@skip` | 跳过文本直到指定内容。 | 从示例文件中定位代码片段起点时使用。 |
| `@skipline` | 跳过当前行直到指定内容。 | 截取示例片段时使用。 |
| `@until` | 输出直到指定内容。 | 截取示例片段终点时使用。 |
| `@example` | 引用一个示例文件。 | 给 API 附加完整可运行示例时使用。 |
| `@snippet` | 引用带标签的代码片段。 | 一个示例文件包含多个独立片段时使用。 |
| `@image` | 插入图片。 | 需要展示架构图、界面或流程图时使用。 |
| `@dot` / `@enddot` | 插入 Graphviz 图。 | 生成依赖、调用或状态关系图时使用。 |
| `@startuml` / `@enduml` | 插入 PlantUML 图。 | 项目启用 PlantUML 集成时使用。 |
| `@mermaid` / `@endmermaid` | 插入 Mermaid 图。 | 项目启用 Mermaid 输出时使用。 |

### 分组、列表和格式

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@arg` | `@param` 的别名或列表项参数。 | 参数说明或参数列表中使用。 |
| `@li` | 列表项。 | 需要列出状态、步骤或选项时使用。 |
| `@n` | 换行。 | 需要在同一段中强制换行时使用。 |
| `@p` | 参数名或段落格式标记。 | 强调代码中的参数名时使用。 |
| `@a` | 斜体或参数样式文本。 | 需要沿用 Doxygen 文本格式时使用。 |
| `@b` | 粗体文本。 | 需要强调短文本时使用。 |
| `@c` | 等宽代码文本。 | 标记短代码、宏或命令时使用。 |
| `@e` / `@em` | 斜体文本。 | 强调术语时使用。 |
| `@f$`、`@f(`、`@f)`、`@f[`、`@f]`、`@f{`、`@f}` | 数学公式边界。 | 需要在文档中嵌入 LaTeX 公式时使用。 |
| `@emoji` | 插入 Emoji。 | 文档确实需要图标表达时使用。 |
| `@unicode` | 插入 Unicode 字符。 | 需要显示特殊字符时使用。 |

### 输出格式控制

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@htmlonly` / `@endhtmlonly` | 仅在 HTML 输出中显示内容。 | 内容只适用于 HTML 时使用。 |
| `@latexonly` / `@endlatexonly` | 仅在 LaTeX 输出中显示内容。 | 内容只适用于 LaTeX 时使用。 |
| `@manonly` / `@endmanonly` | 仅在 man 输出中显示内容。 | 内容只适用于 man 页面时使用。 |
| `@rtfonly` / `@endrtfonly` | 仅在 RTF 输出中显示内容。 | 内容只适用于 RTF 时使用。 |
| `@docbookonly` / `@enddocbookonly` | 仅在 DocBook 输出中显示内容。 | 内容只适用于 DocBook 时使用。 |
| `@xmlonly` / `@endxmlonly` | 仅在 XML 输出中显示内容。 | 内容只适用于 XML 时使用。 |
| `@htmlinclude` | 插入 HTML 文件。 | 需要直接嵌入 HTML 内容时使用。 |
| `@latexinclude` | 插入 LaTeX 文件。 | 需要直接嵌入 LaTeX 内容时使用。 |
| `@maninclude` | 插入 man 文件。 | 需要直接嵌入 man 内容时使用。 |
| `@rtfinclude` | 插入 RTF 文件。 | 需要直接嵌入 RTF 内容时使用。 |
| `@docbookinclude` | 插入 DocBook 文件。 | 需要直接嵌入 DocBook 内容时使用。 |
| `@xmlinclude` | 插入 XML 文件。 | 需要直接嵌入 XML 内容时使用。 |

### 内部文档和条件内容

| 关键字 | 含义 | 什么时候使用 |
| --- | --- | --- |
| `@internal` / `@endinternal` | 标记只供内部文档使用的内容。 | 不希望出现在公开 API 文档中，但仍需生成内部文档时使用。 |
| `@cond` / `@endcond` | 条件隐藏或显示文档片段。 | 需要根据配置项控制文档内容时使用。 |
| `@publicsection` | 开始公共成员章节。 | C 等无法直接表达访问级别的语言中使用，普通 C++ 通常省略。 |
| `@private` / `@protected` | 标记访问级别。 | 需要手动补充语言无法识别的访问级别时使用。 |

## 进阶用法示例


### 文件与分组

使用 `@defgroup` 定义模块，使用 `@ingroup` 将文件或实体加入模块。`@{` 和 `@}` 可以包围一组连续声明。

```cpp
/**
 * @defgroup storage 存储模块
 * @brief 文件和缓存相关接口。
 */

/**
 * @file cache.hpp
 * @ingroup storage
 * @brief 内存缓存接口。
 * @{
 */

class Cache;

/** @} */
```

### 页面、章节和子页面

`@page` 适合编写独立的使用说明或设计文档。页面之间使用 `@subpage` 建立层级关系。

```cpp
/**
 * @page storage_guide 存储模块指南
 * @brief 介绍存储模块的初始化和生命周期。
 *
 * @section storage_init 初始化
 * 先创建配置，再创建存储对象。
 *
 * @subsection storage_errors 错误处理
 * 初始化失败时应释放已创建的资源。
 *
 * 相关页面见 @subpage storage_api。
 */
```

### 相关函数和重载

非成员函数可以使用 `@relates` 放入相关类的文档中。重载函数可以使用 `@copydoc` 复用已有说明。

```cpp
/**
 * @brief 比较两个缓存对象。
 * @relates Cache
 */
bool operator==(const Cache& left, const Cache& right);

/**
 * @copydoc Cache::get(std::string_view) const
 * @overload
 */
std::string Cache::get(const char* key) const;
```

### 代码片段和外部示例

`@include` 适合插入完整示例文件，`@snippet` 适合从一个文件中引用带标签的片段，`@dontinclude`、`@skip` 和 `@until` 适合按内容截取示例。

```cpp
/**
 * @brief 基本用法。
 *
 * @snippet examples/cache.cpp cache-basic
 */
```

示例文件可以这样标记片段：

```cpp
// [cache-basic]
Cache cache;
cache.put("answer", 42);
// [cache-basic]
```

### 内部文档和条件内容示例

内部 API 可以使用 `@internal`，条件内容可以使用 `@cond`。这些命令应配对使用，并与 Doxygen 配置中的输出策略保持一致。

```cpp
/**
 * @brief 公共入口。
 */
void start();

/**
 * @internal
 * @brief 仅供实现使用的辅助函数。
 * @endinternal
 */
void start_impl();
```

### 自定义链接和锚点

当自然的实体名称不足以表达链接文字时使用 `@link`。需要在长页面中建立固定跳转位置时使用 `@anchor`。

```cpp
/**
 * 详细说明见 @link Cache::put(std::string_view, int) 写入接口@endlink。
 *
 * @anchor cache_threading
 * 本模块的线程安全约束见此处。
 */
```

### 输出格式专用内容

只有在项目同时生成多种格式，并且某段内容确实需要区分输出格式时，才使用 `@htmlonly`、`@latexonly`、`@xmlonly` 等命令。单一 HTML 输出项目通常不需要这些命令。

```cpp
/**
 * HTML 版本使用下面的提示。
 * @htmlonly
 * <strong>仅显示在 HTML 文档中。</strong>
 * @endhtmlonly
 */
```
## 书写要求

- 注释描述接口的行为、约束、错误处理和线程安全要求，不重复代码本身。
- 公共接口必须说明参数、返回值和异常。无返回值函数不写 `@return`。
- `@param` 的名称必须与声明中的参数名称完全一致。
- `@retval` 只用于需要解释不同返回结果的函数。
- `@pre`、`@post` 和 `@invariant` 只记录可验证的条件。
- `@todo` 和 `@bug` 应描述具体问题，必要时附上 Issue 编号。
- 废弃接口使用 `@deprecated`，同时说明替代接口和移除计划。
- 成员变量优先使用行尾注释。复杂成员使用完整块注释。
- 不在实现细节、临时变量或显而易见的代码旁堆积注释。
- 示例代码应可以编译，或明确标注为伪代码。
- 注释中的代码名称使用反引号或 `@p`、`@c` 标记。

## 注释块选择

```cpp
/** 文档块，推荐用于公共声明。 */
/// 文档行，适合连续的短说明。
//! 文档行，适合 Qt 风格项目。
int value; ///< 成员的行尾说明。
int other; //!< 成员的行尾说明。
```

不要混用普通 `/* ... */` 和 Doxygen 文档块来描述同一个实体。普通注释用于实现说明，Doxygen 注释用于生成公共文档。
