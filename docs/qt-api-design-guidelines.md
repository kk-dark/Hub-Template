# Qt API 设计规范

本文是 [C++ API 设计规范](cpp-api-design-guidelines.md) 的 Qt 专属补充，适用于 Qt 5.15.2 和 Qt 6 的公共头文件。通用的 ABI、错误处理、所有权和兼容性规则仍以 C++ API 设计规范为准。

本文参考 [Qt API Design Principles](https://doc.qt.io/qt-6/design-principles.html)、[Qt Source and Binary Compatibility](https://doc.qt.io/qt-6/source-binary-compatibility.html)、[Qt D-Pointer](https://wiki.qt.io/D-Pointer) 和 [QObject Class Reference](https://doc.qt.io/qt-6/qobject.html)。外部资料提供设计依据，本仓库列出的“必须”和“禁止”属于项目强制规则。

## QObject 类和 d-pointer

- 对外暴露且需要 ABI 稳定的 QObject 实体类必须使用 d-pointer。d-pointer 是 Qt 对 PIMPL 的约定，用于把私有数据和实现移出公共类布局。
- 本仓库的 d-pointer 存储统一使用 `std::unique_ptr<ClassPrivate>`，并命名为 `d_ptr`。它同时满足 Qt 的 d-pointer 约定和 C++ API 设计规范的唯一所有权要求。
- 只有明确具有稳定布局、不会继续增加私有状态的简单值类型，才可以不使用 d-pointer。
- 公共类必须提供 `QObject* parent = nullptr` 构造参数，除非该类型明确不属于 QObject 父子对象树。
- QObject 派生类禁止复制。头文件必须显式删除复制构造函数和复制赋值运算符，或使用 Qt 提供的禁用复制宏。
- QObject 的私有实现不得通过公共信号、槽、属性或数据成员暴露。
- 由父对象管理生命周期的 QObject 禁止要求调用方手动 `delete`。
- `Q_DECLARE_PRIVATE`、`Q_D` 和 `Q_Q` 只允许在私有实现需要访问对应公共类，或需要 Qt 私有类访问机制时使用。普通成员函数直接访问 `d_ptr` 时不添加这些宏。

```cpp
class FileModelPrivate;

class FileModel : public QObject
{
    Q_OBJECT

public:
    explicit FileModel(QObject* parent = nullptr);
    ~FileModel() override;

    FileModel(const FileModel&) = delete;
    FileModel& operator=(const FileModel&) = delete;

private:
    Q_DECLARE_PRIVATE(FileModel)
    std::unique_ptr<FileModelPrivate> d_ptr;
};
```

`FileModelPrivate` 必须在源文件或私有头文件中定义。公共头文件只保留前置声明和一个 `d_ptr`，不得暴露私有类成员。

## 智能指针选择

Qt 项目默认使用 C++ 标准智能指针。`std::unique_ptr`、`std::shared_ptr` 和 `std::weak_ptr` 的一般规则见 [C++ API 设计规范](cpp-api-design-guidelines.md)。

只有需要 Qt 特有语义或与 Qt API 直接互操作时，才必须使用 Qt 智能指针：

| 场景 | 必须使用 | 原因 |
| --- | --- | --- |
| 保存可能被其他代码销毁的借用 `QObject`，并要求对象销毁后指针自动变为空 | `QPointer<T>` | `QPointer` 提供 QObject 的受保护指针语义。 |
| 公共或内部接口已经以 `QSharedPointer<T>` 表达共享所有权 | `QSharedPointer<T>` | 保持 Qt 共享控制块和接口类型一致。 |
| 观察 `QSharedPointer<T>` 管理的对象且不延长生命周期 | `QWeakPointer<T>` | `QWeakPointer` 只能从 `QSharedPointer` 建立弱引用。 |
| 使用 Qt 规定的清理器或必须与已有 `QScopedPointer` 接口互操作 | `QScopedPointer<T, Cleanup>` | 保持 Qt 清理策略和现有接口兼容。 |

- `QPointer<T>` 只用于 `QObject` 派生类，不能表示所有权，也不能替代 `std::unique_ptr<T>`。
- QObject 已由父对象管理时，禁止再用 `QSharedPointer` 或 `std::shared_ptr` 取得第二套所有权。
- 新增的独立所有权默认使用 `std::unique_ptr<T>`，确实需要共享所有权时使用 `std::shared_ptr<T>`。
- 公共 API 一旦选择 `QSharedPointer`、`QWeakPointer` 或 `QPointer`，相关参数、成员和返回值必须保持同一套 Qt 指针语义。

参考 [QPointer](https://doc.qt.io/qt-6/qpointer.html)、[QSharedPointer](https://doc.qt.io/qt-6/qsharedpointer.html)、[QWeakPointer](https://doc.qt.io/qt-6/qweakpointer.html) 和 [QScopedPointer](https://doc.qt.io/qt-6/qscopedpointer.html)。

## Qt 元对象接口

- 公共信号、槽、属性和可反射枚举必须使用 Qt 元对象系统声明。
- `Q_PROPERTY` 必须明确 `READ`、`WRITE`、`RESET`、`NOTIFY` 等实际支持的操作，不能声明不存在的能力。
- 属性变化信号必须只在属性值实际变化时发出，除非接口文档明确规定重复通知。
- `Q_ENUM` 和 `Q_FLAG` 必须紧邻对应的枚举声明。
- 公共 `Q_INVOKABLE` 函数必须遵守 C++ API 的参数、错误和线程规则。
- 公共接口不得把私有 QObject、内部模型或内部容器作为属性类型、信号参数或槽参数暴露。

## 信号、槽和线程

- 信号必须表示已经发生的事件或状态变化，命名和参数必须保持稳定。
- 槽和 `Q_INVOKABLE` 函数必须说明调用线程、重复调用行为和异步结果信号。
- 跨线程 API 必须通过信号槽、事件或明确的线程安全接口通信，禁止要求调用方直接操作其他线程中的 QObject。
- 使用 `Qt::BlockingQueuedConnection` 的公共接口必须在文档中说明等待关系和避免循环等待的条件。
- 公共 API 不得把连接顺序作为调用方可依赖的行为，除非文档明确规定。

## Qt 类型和所有权

- Qt 库的公共 API 必须统一使用项目规定的 Qt 类型，例如 `QString`、`QByteArray`、`QUrl` 和 Qt 容器。
- Qt API 不得在相同语义中混用 Qt 字符串、标准库字符串和 C 字符串接口，除非边界转换是接口职责的一部分。
- QObject 的父子关系用于表达唯一所有权时，接口必须说明父对象要求和销毁时机。
- 不属于父子对象树的资源使用 C++ API 设计规范中的智能指针和所有权规则。
- 公共 API 不得返回指向 QObject 内部对象、Qt 容器元素或临时值的悬空引用和指针。

## 模型、视图和界面

- `QAbstractItemModel` 派生类必须稳定定义 role、索引有效性和编辑行为。
- 模型增删改操作必须使用对应的 `begin...`/`end...` 通知，调用方不得依赖内部容器布局。
- QWidget 公共接口必须说明 GUI 线程要求和用户可见结果。
- QML 可见属性、信号和 `Q_INVOKABLE` 函数必须说明 QML 名称、类型、生命周期和线程要求。

## Qt 版本和兼容性

- 公共 API 必须明确适用 Qt 主版本和最低次版本。
- Qt 5 和 Qt 6 的 API、构建目标和特有类型不得在同一个目标中混用。
- 已发布的 `Q_PROPERTY` 名称、信号签名、枚举值和 role 值不得随意修改。
- 修改 QObject 布局、虚函数、信号、槽、属性或元对象声明前，必须评估源码兼容性和 ABI 兼容性。
- Qt 版本差异导致的替代接口必须在文档中标明，并提供迁移说明。

## 参考关系

本文件只补充 Qt 特有的 API 设计要求：

- 通用公共头文件、PIMPL、错误码、`std::optional`、所有权和 ABI 规则见 [C++ API 设计规范](cpp-api-design-guidelines.md)。
- Qt 命名、线程、构建和 Qt 类型使用规则见 [Qt 编码规范](qt-coding-guidelines.md)。
- Qt 信号、槽、属性、模型和界面的注释规则见 [Qt 注释规范](qt-doxygen-guidelines.md)。
