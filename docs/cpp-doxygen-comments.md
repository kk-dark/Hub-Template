# C++ 注释规范

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

## 关键字清单

以下清单按用途整理 Doxygen 常用命令。命令可以使用 `@` 或 `\` 开头，例如 `@brief` 与 `\brief` 等价。完整命令及参数规则以 [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html) 为准。

### 结构和归属

`@file`、`@class`、`@struct`、`@union`、`@enum`、`@fn`、`@var`、`@def`、`@typedef`、`@namespace`、`@interface`、`@package`、`@dir`、`@headerfile`、`@group`、`@defgroup`、`@addtogroup`、`@ingroup`、`@weakgroup`、`@mainpage`、`@page`、`@subpage`、`@section`、`@subsection`、`@subsubsection`、`@paragraph`、`@anchor`、`@relates`、`@related`、`@relatedalso`、`@implements`、`@extends`、`@showinitializer`、`@showdate`。

### 描述和元数据

`@brief`、`@details`、`@short`、`@author`、`@date`、`@version`、`@copyright`、`@since`、`@deprecated`、`@note`、`@attention`、`@warning`、`@remark`、`@remarks`、`@bug`、`@todo`、`@test`、`@invariant`、`@pre`、`@post`、`@requires`、`@ensures`。

### 参数、返回值和异常

`@param`、`@tparam`、`@return`、`@returns`、`@result`、`@retval`、`@exception`、`@throw`、`@throws`。

参数方向写在 `@param` 后面：

```cpp
@param[in] input 只读输入参数。
@param[out] output 输出参数。
@param[in,out] buffer 输入并被更新的参数。
```

### 引用、复制和重载

`@ref`、`@see`、`@sa`、`@cite`、`@link`、`@endlink`、`@copydoc`、`@copybrief`、`@copydetails`、`@overload`、`@nosubgrouping`。

### 代码、文件和示例

`@code`、`@endcode`、`@verbatim`、`@endverbatim`、`@include`、`@dontinclude`、`@verbinclude`、`@line`、`@skip`、`@skipline`、`@until`、`@example`、`@snippet`、`@image`、`@dot`、`@enddot`、`@plantuml`、`@startuml`、`@enduml`、`@mermaid`、`@endmermaid`。

### 列表、格式和字符

`@arg`、`@li`、`@n`、`@a`、`@b`、`@c`、`@p`、`@e`、`@em`、`@emoji`、`@f$`、`@f(`、`@f)`、`@f[`、`@f]`、`@f{`、`@f}`、`@unicode`、`@verbatim`。

### 输出格式控制

`@htmlonly`、`@endhtmlonly`、`@latexonly`、`@endlatexonly`、`@manonly`、`@endmanonly`、`@rtfonly`、`@endrtfonly`、`@docbookonly`、`@enddocbookonly`、`@xmlonly`、`@endxmlonly`、`@htmlinclude`、`@latexinclude`、`@maninclude`、`@rtfinclude`、`@docbookinclude`、`@xmlinclude`。

### 常用转义命令

`@$`、`@&`、`@<`、`@>`、`@#`、`@%`、`@@`、`@\\`、`@~`。

## C++ 实体示例

### 文件

```cpp
/**
 * @file calculator.hpp
 * @brief 计算器公共接口。
 *
 * @details 提供无状态的整数计算函数。
 */
```

### 命名空间

```cpp
/**
 * @namespace math
 * @brief 数学工具。
 */
namespace math
{
}
```

### 类

```cpp
/**
 * @class Connection
 * @brief 表示一个网络连接。
 *
 * @details Connection 管理连接状态和收发缓冲区。
 * 对象不可复制，但可以移动。
 *
 * @note 成员函数不是线程安全的。
 * @since 2.0.0
 */
class Connection
{
public:
    /**
     * @brief 建立连接。
     *
     * @param[in] endpoint 远端地址。
     * @param[in] timeout 超时时间。
     *
     * @return 连接是否建立成功。
     * @throws std::system_error 网络操作失败时抛出。
     */
    bool connect(std::string_view endpoint,
                 std::chrono::milliseconds timeout);
};
```

### 成员函数

```cpp
/**
 * @brief 关闭连接并释放发送缓冲区。
 *
 * 重复调用是安全的。
 *
 * @post is_open() 返回 false。
 */
void close() noexcept;
```

### 公共函数

```cpp
/**
 * @brief 解析配置文件。
 *
 * @param[in] path 配置文件路径。
 * @param[out] config 解析后的配置。
 *
 * @return 解析是否成功。
 * @retval true 配置完整且有效。
 * @retval false 文件不存在或格式错误。
 *
 * @throws std::filesystem::filesystem_error 文件访问失败时抛出。
 */
bool parse_config(const std::filesystem::path& path, Config& config);
```

### 结构体

```cpp
/**
 * @struct Point
 * @brief 二维坐标。
 *
 * @var Point::x
 * 横坐标。
 *
 * @var Point::y
 * 纵坐标。
 */
struct Point
{
    double x; ///< 横坐标。
    double y; ///< 纵坐标。
};
```

### 枚举

```cpp
/**
 * @enum ConnectionState
 * @brief 连接状态。
 */
enum class ConnectionState
{
    Disconnected, ///< 尚未连接。
    Connecting,   ///< 正在连接。
    Connected     ///< 已连接。
};
```

### 类型别名

```cpp
/**
 * @brief 用户 ID。
 */
using UserId = std::uint64_t;
```

### 模板和模板参数

```cpp
/**
 * @brief 固定容量的环形缓冲区。
 *
 * @tparam T 元素类型。
 * @tparam Capacity 缓冲区容量，必须大于零。
 */
template <typename T, std::size_t Capacity>
class RingBuffer
{
};
```

### 宏

```cpp
/**
 * @def CHECK
 * @brief 检查条件，不满足时记录错误并返回。
 * @param condition 待检查的条件。
 */
#define CHECK(condition)
```

### 变量和常量

```cpp
/**
 * @var kMaxConnections
 * @brief 单个进程允许的最大连接数。
 */
inline constexpr std::size_t kMaxConnections = 128;
```

### 回调函数类型

```cpp
/**
 * @brief 连接完成回调。
 *
 * @param error 错误码。成功时为默认构造值。
 */
using ConnectCallback = std::function<void(std::error_code error)>;
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
