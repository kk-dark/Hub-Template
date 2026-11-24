# C++ 编码规范

本文适用于本仓库中的 C++ 源码、头文件、测试和示例代码。本文使用以下明确措辞：

- **必须**：违反后不得合并。
- **禁止**：不得使用。
- **允许**：满足所列条件时可以使用。

本文将外部规范作为参考依据，将仓库自身的工程约束单独列出。外部规范中的建议只有在本文明确写成“必须”或“禁止”时，才构成本仓库的强制要求。

主要参考来源：

- [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html)

## 1. 语言和文件

- 必须使用项目指定的 C++ 标准，并开启项目规定的编译器警告。
- 禁止使用未定义行为、依赖未声明平台特性的代码。
- 使用标准库设施前必须直接包含提供该设施声明的头文件。
- 头文件必须使用 `#pragma once` 或完整 include guard，项目内统一使用一种方式。
- 文件名使用小写字母和下划线，例如 `connection_manager.hpp`。
- 一个文件只实现一个主要模块或类型。

## 2. 头文件和源文件

本节结合 [Google C++ Style Guide 的 Header Files](https://google.github.io/styleguide/cppguide.html#header_files) 和本仓库的模块组织要求制定。

- 头文件必须只放公共声明、模板定义、必须在编译期求值的定义，以及明确要求内联的短函数。
- 普通非模板函数、复杂成员函数和业务逻辑必须放在 `.cpp` 文件中。
- 禁止在头文件中放置普通函数的非内联实现。
- 禁止在头文件中定义可变的命名空间级对象。
- 头文件必须自包含，单独包含该头文件时必须能够通过编译。
- 源文件必须先包含对应的头文件，再包含其他头文件。

## 3. 命名

命名规则参考 [Google C++ Style Guide 的 Naming](https://google.github.io/styleguide/cppguide.html#naming)，具体名称形式以本节表格为准。

命名必须表达实体职责。禁止使用 `data`、`info`、`thing`、`temp`、`obj` 等无法说明含义的名称。

| 实体 | 规则 | 示例 |
| --- | --- | --- |
| 命名空间 | 小写蛇形命名 | `stargazers::repository` |
| 类、结构体、枚举、类型别名 | 大驼峰命名 | `ConnectionManager`、`ConnectionState`、`UserId` |
| 函数和成员函数 | 小写蛇形命名 | `load_config()`、`is_connected()` |
| 局部变量和参数 | 小写蛇形命名 | `retry_count`、`file_path` |
| 非静态数据成员 | 小写蛇形命名并以下划线结尾 | `file_path_` |
| 常量和 `constexpr` 变量 | `k` 加大驼峰命名 | `kMaxConnections` |
| 枚举值 | 大驼峰命名 | `ConnectionState::Connected` |
| 宏 | 全大写蛇形命名并使用项目专属前缀 | `STARGAZERS_CHECK` |
| 模板参数 | 大驼峰命名 | `ElementType`、`Capacity` |

- 布尔变量和函数必须使用 `is_`、`has_`、`can_` 或 `should_` 等真假语义前缀。
- 禁止使用匈牙利命名法、拼音和无明确含义的缩写。
- 禁止使用单字母命名表达长期存在的变量。

## 4. 常量和宏

常量优先使用类型安全的语言设施，符合 [C++ Core Guidelines 的 Constants](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-constants) 和 [Google C++ Style Guide 的 Preprocessor Macros](https://google.github.io/styleguide/cppguide.html#preprocessor_macros)。本仓库进一步禁止使用 `#define` 定义普通常量。

- 常量必须优先使用 `constexpr`、`const` 或 `enum class`。
- 禁止使用 `#define` 定义数值、字符串、布尔值或类型常量。
- 禁止使用函数式宏实现可以由函数、模板或 `constexpr` 函数实现的逻辑。
- 宏只允许用于条件编译、编译器属性和无法由语言设施替代的预处理操作。
- 每个宏名称必须使用项目专属前缀。

```cpp
inline constexpr std::size_t kMaxConnections = 128;

// 禁止
#define MAX_CONNECTIONS 128
```

## 5. 所有权和资源管理

本节遵循 [C++ Core Guidelines 的 Resource Management](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource) 和 [Google C++ Style Guide 的 Ownership and Smart Pointers](https://google.github.io/styleguide/cppguide.html#ownership_and_smart_pointers)。

- 裸指针禁止表达所有权。
- `T*` 只能表示可为空的借用对象，生命周期必须由接口或注释说明。
- 必须使用引用表示不能为空且不转移所有权的对象。
- 必须使用 `std::unique_ptr` 表示唯一所有权。
- 必须使用 `std::shared_ptr` 表示确实需要共享生命周期的对象。
- 必须使用 `std::weak_ptr` 打破共享所有权环。
- 禁止直接使用 `new` 和 `delete` 管理对象生命周期。
- 必须使用 `std::make_unique` 和 `std::make_shared` 创建智能指针。
- 文件、锁、套接字等非内存资源必须使用 RAII 类型管理。
- 只有与 C API 或特殊分配器交互时才允许在资源封装类内部使用裸 `new` 或 `delete`。

```cpp
void process(const Connection& connection);
void inspect(const Connection* connection);
std::unique_ptr<Connection> create_connection();
```

## 6. 类、结构体和接口

类的职责、特殊成员函数和资源管理参考 [C++ Core Guidelines 的 Classes](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-class)。

- 类必须保持单一职责。
- 资源必须由构造函数获取，由析构函数释放。
- 通过基类指针删除对象时，基类析构函数必须为 `virtual`。
- 禁止复制或移动时，必须显式使用 `= delete`。
- 只保存数据且不维护不变量的类型使用 `struct`。
- 维护不变量、封装资源或需要访问控制的类型使用 `class`。
- 类的数据成员必须设为 `private`。
- 不修改对象状态的成员函数必须标记 `const`。
- 不抛出异常的析构函数和移动操作必须标记 `noexcept`。
- 单参数构造函数必须写 `explicit`，除非该隐式转换是接口设计的一部分。

## 7. 函数和类型

函数职责、参数设计和返回值设计参考 [C++ Core Guidelines 的 Functions](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-functions)。

- 一个函数必须只承担一个可说明的职责。
- 函数参数超过四个时，必须评估是否改用参数结构体。
- 输出结果优先使用返回值，禁止使用非必要的输出指针参数。
- 禁止返回局部对象的引用或指针。
- 函数必须明确选择异常、错误码、`std::optional` 或 `std::expected` 表达失败。
- 禁止同时用返回值、输出参数和异常表达同一种失败状态。
- 枚举必须优先使用 `enum class`。
- 禁止使用 C 风格转换，必须使用语义正确的 C++ 显式转换。
- 使用 `reinterpret_cast` 时，代码附近必须说明底层布局或 ABI 依据。

## 8. 命名空间和 `using`

全局 `using namespace` 的限制属于本仓库的工程规则。名称和别名的基本原则参考 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)。

- 禁止在头文件中使用全局 `using namespace`。
- 禁止在源文件的全局作用域使用全局 `using namespace`。
- 函数内部允许使用局部 `using` 声明，但只能引入实际使用的名称。
- 新代码中的类型别名必须使用 `using`，禁止使用 `typedef`。
- 源文件中仅供本文件使用的全局函数和变量必须放入未命名命名空间，禁止使用文件作用域 `static` 定义它们。

```cpp
namespace {

constexpr int kDefaultPort = 8080;

bool is_valid_port(int port)
{
    return port > 0 && port <= 65535;
}

}  // namespace
```

上例中的 `kDefaultPort` 和 `is_valid_port()` 只在当前源文件内可见。不得写成以下形式：

```cpp
static constexpr int kDefaultPort = 8080;
static bool is_valid_port(int port);
```

## 9. 错误处理和并发

异常处理参考 [C++ Core Guidelines 的 Error Handling](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-errors)。线程和锁的规则参考 [C++ Core Guidelines 的 Concurrency](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-concurrency)。

- 错误处理方式必须与接口约定一致。
- 异常只能表达异常情况，禁止用异常表达正常分支。
- 捕获异常时必须按 `const` 引用或具体类型捕获。
- 禁止捕获后不处理、不记录或不重新抛出异常。
- 析构函数、移动构造函数和移动赋值运算符禁止抛出异常。
- 每个可共享对象必须明确声明线程安全等级。
- 共享可变状态必须使用互斥量、原子类型或其他同步机制保护。
- 禁止通过 `volatile` 实现线程同步。
- 禁止在持有锁时调用未知实现、用户回调或可能阻塞的外部接口。

## 10. 注释和测试

注释格式和命令遵循 [Doxygen Special Commands](https://www.doxygen.nl/manual/commands.html)，注释内容的组织方式参考 [Google C++ Style Guide 的 Commenting](https://google.github.io/styleguide/cppguide.html#commenting)。测试要求属于本仓库的工程规则。

- 公共 API 必须使用 Doxygen 注释。
- 注释必须说明行为、参数约束、错误处理和线程安全要求。
- 禁止使用“适当时”“必要时”“尽量”“可能”等无法验证的模糊词描述强制要求。
- 实现注释必须解释原因、约束或取舍，禁止复述代码。
- C++ 注释格式见[C++ 注释规范](cpp-doxygen-introduction.md)，完整命令见 [Doxygen 详细参考](cpp-doxygen-reference.md)。
- 新增公共 API 必须增加正常路径和失败路径测试。
- 修复缺陷必须增加能够复现原问题的测试。
- 提交前必须运行项目已有的构建和测试命令。
- 文档或配置修改至少运行 `git diff --check`。

## 11. 工具和自动检查

- 代码格式化统一使用项目配置的 `clang-format`。格式规则应提交为 `.clang-format`，并在 CI 中检查格式差异。
- 静态检查统一使用项目配置的 `clang-tidy`。检查规则应提交为 `.clang-tidy`，并在 CI 中执行。
- `clang-format` 的规则说明见 [ClangFormatStyleOptions](https://clang.llvm.org/docs/ClangFormatStyleOptions.html)。
- `clang-tidy` 的检查说明见 [Clang-Tidy Checks](https://clang.llvm.org/extra/clang-tidy/checks/list.html)。
