# API 设计规范

本文适用于所有对外暴露在公共头文件中的 C++ `public` 接口，包括类、结构体、函数、枚举、类型别名、常量和宏。实现文件中的内部接口不受本文约束，但仍必须遵守 [C++ 编码规范](cpp-coding-guidelines.md)。

本文的目标是保持接口清晰、所有权明确，并降低发布后的 ABI 兼容成本。公共 API 的注释遵循 [C++ 注释规范](cpp-doxygen-introduction.md)。

本文参考 [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)、[Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)、[cppreference 的 PImpl 说明](https://en.cppreference.com/w/cpp/language/pimpl) 和 [Qt 的 D-Pointer 说明](https://wiki.qt.io/D-Pointer)。外部资料作为设计依据，本仓库列出的“必须”和“禁止”属于本项目的强制规则。

## 公共头文件边界

本节结合 [Google C++ Style Guide 的 Header Files](https://google.github.io/styleguide/cppguide.html#header_files) 制定。

- 公共头文件只能暴露调用方必须使用的类型和函数。
- 禁止暴露私有成员、内部结构体、内部容器、锁、线程对象和实现细节。
- 公共头文件必须自包含，直接包含后能够通过编译。
- 公共头文件不得包含不必要的实现依赖。能使用前置声明时，必须优先使用前置声明。
- 公共 API 必须使用项目命名空间，禁止把项目类型放入全局命名空间。
- 公共头文件禁止使用全局 `using namespace`。
- 公共头文件禁止定义可变的命名空间级对象。

## 类和 ABI

PImpl 通过不透明指针把实现细节移出对象布局，可用于减少头文件依赖并保持 ABI 稳定，具体机制见 [cppreference 的 PImpl 说明](https://en.cppreference.com/w/cpp/language/pimpl)。对外暴露的实体类必须统一使用 PIMPL。公共类的头文件只保留一个指向实现类的 `std::unique_ptr<Impl>`。

Qt 项目中的库类可以参考 Qt 的 [D-Pointer 模式](https://wiki.qt.io/D-Pointer)，但本仓库统一使用标准 C++ 的 `std::unique_ptr<Impl>` 表达实现所有权。

```cpp
class Connection
{
public:
    Connection();
    ~Connection();

    Connection(Connection&&) noexcept;
    Connection& operator=(Connection&&) noexcept;

    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};
```

- `Impl` 必须在源文件中定义，禁止在公共头文件中暴露其成员。
- 使用 `std::unique_ptr<Impl>` 的类必须在源文件中定义析构函数，避免不完整类型导致编译错误。
- PIMPL 类的移动构造和移动赋值必须明确声明。是否允许复制必须由接口语义明确决定。
- 公共类禁止把 STL 容器、第三方类型或项目内部类型作为公开数据成员。
- 公共类的布局、成员顺序和私有实现不得成为调用方依赖的接口。
- 公共类新增状态必须优先放入 `Impl`，避免改变公共类对象布局。
- 对外暴露的 `struct` 只能表示稳定的值类型或参数类型，字段必须是调用方需要的公共数据，不能借此暴露内部状态。

## 枚举和类型

- 对外枚举必须优先使用强类型 `enum class`。
- 公共枚举必须固定底层类型，例如 `enum class ErrorCode : std::uint32_t`。
- 枚举值必须明确命名空间归属，禁止依赖隐式整数转换。
- 公共 API 禁止使用未命名结构体、匿名枚举和实现专用类型。
- 类型别名必须表达稳定的业务语义，禁止用别名隐藏指针所有权或内部容器。
- 公共 API 禁止使用模板作为主要接口。模板会在使用者侧实例化，容易暴露实现细节并破坏 ABI。
- 模板只有在接口本身必须依赖编译期类型，且项目已明确接受源码兼容和 ABI 约束时才允许公开。

```cpp
enum class ErrorCode : std::uint32_t
{
    Ok = 0,
    InvalidArgument,
    NotFound,
    IoError
};
```

## 错误处理和返回值

- 对外 API 的错误状态必须使用项目定义的 `ErrorCode`，禁止使用裸 `int`、`bool` 或未约定的整数表示错误。
- 只返回成功或失败状态的操作返回 `ErrorCode`。
- 返回值可能不存在，且“不存在”是正常业务结果时，优先使用 `std::optional<T>`。
- 需要同时区分错误原因和返回值时，必须使用项目统一的结果类型，或返回 `ErrorCode` 并通过明确标注的输出参数承载 `std::optional<T>`。
- 禁止用空指针、特殊整数、空字符串或未定义的枚举值伪造错误状态。
- 返回容器时必须明确空容器表示“没有数据”还是“操作失败”。两者有区别时必须使用错误码和结果值分别表达。
- 公共 API 禁止以异常作为跨模块的默认错误协议，除非接口文档明确声明异常类型和抛出条件。

```cpp
class Repository
{
public:
    ErrorCode open(const std::string& path);
    ErrorCode find(std::string_view key,
                   std::optional<std::string>& value) const;
};
```

`find()` 返回 `ErrorCode::Ok` 时，`value` 可能为空，表示没有匹配项。发生 I/O 或参数错误时，返回对应的错误码。

## 所有权和生命周期

所有权规则参考 [C++ Core Guidelines 的 Resource Management](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#S-resource) 和 [Google C++ Style Guide 的 Ownership and Smart Pointers](https://google.github.io/styleguide/cppguide.html#ownership_and_smart_pointers)。

- 裸指针只能表示借用关系，公共 API 必须在注释中说明是否允许为空以及生命周期由谁负责。
- 公共 API 不得返回指向内部对象、内部容器元素或临时对象的悬空指针和引用。
- 转移唯一所有权使用 `std::unique_ptr<T>`。
- 确实需要共享生命周期时使用 `std::shared_ptr<T>`，并说明共享原因。
- 观察共享对象但不延长生命周期时使用 `std::weak_ptr<T>`。
- 公共 API 禁止要求调用方使用 `new` 或 `delete` 管理返回对象。
- 资源的创建和释放必须由同一套明确的 API 规则管理。

## 函数参数和接口形状

- 参数必须按语义区分输入、输出和输入输出。
- 只读对象优先使用 `const T&` 或值传递。可为空的借用对象使用指针，并在注释中说明。
- 输出参数只有在接口需要返回多个结果或必须返回 `ErrorCode` 时才允许使用。
- 公共函数参数超过四个时，必须改用参数结构体或拆分接口。
- 参数结构体必须只包含稳定的公共字段，不得暴露内部缓存和实现状态。
- 公共函数不得返回局部对象的引用或指针。
- 不修改对象状态的成员函数必须声明为 `const`。
- 不抛出异常的析构函数和移动操作必须声明为 `noexcept`。

## 兼容性和版本演进

- 新增公共 API 必须说明引入版本，废弃 API 必须使用 `[[deprecated]]` 并提供替代接口。
- 删除、重命名、改变参数类型、改变所有权或改变错误码含义都属于破坏性变更，必须按 [版本控制规范](../VERSIONING.md) 处理。
- 已发布的枚举值不得重新编号或改变含义。
- 已发布的错误码不得复用旧含义。
- 公共类的虚函数、信号、槽和属性变更必须评估 ABI 与源码兼容性。
- 公共 API 的默认参数会固化在调用方代码中，默认值变更必须评估兼容性。
- 公共头文件的修改必须检查下游重新编译后的 ABI 和行为。

## 文档和验证

- 每个公共实体必须说明用途、参数约束、返回值、错误码、所有权和线程安全要求。
- PIMPL 类必须说明复制、移动和生命周期语义。
- `ErrorCode` 必须为每个可观察错误提供明确含义。
- 公共 API 新增或修改时必须补充正常路径、错误路径和兼容性测试。
- 发布前必须使用项目配置的 ABI 检查工具或等效方法检查公共接口变化。

