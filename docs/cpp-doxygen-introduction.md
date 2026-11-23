# C++ 注释规范

本项目使用 Doxygen 生成 C++ API 文档。公共头文件中的类、结构体、枚举、类型别名、命名空间、函数和重要成员应提供注释。


## 基本写法

公共声明使用 `/** ... */`，简单的成员说明使用 `///<`：

```cpp
/**
 * @brief 计算两个数的和。
 *
 * @param[in] left 左操作数。
 * @param[in] right 右操作数。
 *
 * @return 两个数的和。
 */
int add(int left, int right);

int value; ///< 当前值。
```

也可以使用 `///` 或 `//!`，同一项目应保持一致：

```cpp
/// 简短说明。
int count;

//! 简短说明。
int capacity;
```

## 常用关键字

| 关键字 | 用途 |
| --- | --- |
| `@brief` | 一句话说明，公共实体必须提供。 |
| `@details` | 补充详细行为、生命周期或限制。 |
| `@param[in]` | 输入参数。 |
| `@param[out]` | 输出参数。 |
| `@param[in,out]` | 既作为输入又会被修改的参数。 |
| `@tparam` | 模板参数。 |
| `@return` | 返回值的含义。 |
| `@retval` | 分别说明不同返回值，例如 `true` 和 `false`。 |
| `@throws` | 可能抛出的异常及条件。 |
| `@pre` | 调用前必须满足的条件。 |
| `@post` | 调用完成后保证的结果。 |
| `@note` | 普通注意事项。 |
| `@warning` | 需要避免的风险。 |
| `@deprecated` | 已废弃接口及替代方案。 |
| `@see` | 相关类型、函数或文档。 |
| `@since` | API 首次引入的版本。 |
| `@todo` | 具体的待办事项。 |
| `@bug` | 已知缺陷。 |

`@param` 后的参数名必须与声明一致。无返回值的函数不写 `@return`。`@throws` 和 `@exception` 只选一种写法。

完整关键字说明见[详细 Doxygen 注释规范](cpp-doxygen-reference.md)。

## 实体示例

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
 * @details 管理连接状态和收发缓冲区。
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
 */
bool parse_config(const std::filesystem::path& path, Config& config);
```

### 结构体

```cpp
/**
 * @struct Point
 * @brief 二维坐标。
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

### 类型别名和模板

```cpp
/**
 * @brief 用户 ID。
 */
using UserId = std::uint64_t;

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

### 文件

```cpp
/**
 * @file calculator.hpp
 * @brief 计算器公共接口。
 */
```

## 书写要求

- 描述接口行为、参数约束、错误处理和线程安全要求，不重复代码本身。
- 公共接口说明参数、返回值和异常。
- 成员变量优先使用行尾注释，复杂成员再使用完整注释块。
- `@todo` 和 `@bug` 写清具体问题，必要时附上 Issue 编号。
- 废弃接口使用 `@deprecated`，同时说明替代接口。
- 示例代码应可以编译，或明确标记为伪代码。
- 实现细节使用普通注释，公共 API 说明使用 Doxygen 注释。

## 更多详细用法

高级关键字、完整命令说明和特殊场景示例见[详细 Doxygen 注释规范](cpp-doxygen-reference.md)。
