# Qt 编码规范

本文适用于 Qt 5.15.2 和 Qt 6 的 C++ 源码、头文件、测试和示例代码。本文只规定 Qt 专属写法。通用 C++ 规则见 [C++ 编码规范](cpp-coding-guidelines.md)，Doxygen 注释见 [C++ 注释规范](cpp-doxygen-introduction.md)，Qt 特有的注释规则见 [Qt 注释规范](qt-doxygen-guidelines.md)。

本文将 Qt 5.15.2 简称为 Qt 5.15，不将所有 Qt 5 版本统称为 Qt 5。未标注版本的规则同时适用于 Qt 5.15.2 和 Qt 6。标注版本的规则只适用于对应版本。项目必须在构建配置中明确 Qt 主版本，不能把 Qt 5 和 Qt 6 的 API 混用在同一个目标中。

本文使用以下明确措辞：

- **必须**：违反后不得合并。
- **禁止**：不得使用。
- **允许**：满足所列条件时可以使用。

主要参考：

- [Qt Coding Style](https://wiki.qt.io/Qt_Coding_Style)
- [Qt 5.15 Documentation](https://doc.qt.io/qt-5.15/)
- [Qt 6 Documentation](https://doc.qt.io/qt-6/)
- [Qt 5.15 QObject Class Reference](https://doc.qt.io/qt-5.15/qobject.html)
- [Qt 5.15 Signals and Slots](https://doc.qt.io/qt-5.15/signalsandslots.html)
- [Qt 5.15 Threads and QObjects](https://doc.qt.io/qt-5.15/threads-qobject.html)
- [QObject Class Reference](https://doc.qt.io/qt-6/qobject.html)
- [Signals and Slots](https://doc.qt.io/qt-6/signalsandslots.html)
- [Threads and QObjects](https://doc.qt.io/qt-6/threads-qobject.html)
- [Qt Meta-Object System](https://doc.qt.io/qt-6/metaobjects.html)

## Qt 5.15.2 和 Qt 6 的差异

| 项目 | Qt 5.15.2 | Qt 6 |
| --- | --- | --- |
| 常用拥有型字符串 | `QString` | `QString` |
| UTF-16 非拥有视图 | `QStringView` | `QStringView` |
| Latin-1 字符串 | `QLatin1String` | `QLatin1StringView` |
| UTF-8 非拥有视图 | 不使用 `QUtf8StringView` | 仅在目标 Qt 6 版本提供该类型时使用 |
| 构建目标 | `Qt5::Core`、`Qt5::Gui`、`Qt5::Widgets` | `Qt6::Core`、`Qt6::Gui`、`Qt6::Widgets` |
| qmake 模块声明 | `QT += core gui widgets` | `QT += core gui widgets` |

Qt 5.15 支持版本无关的 `Qt::Core` 等 CMake 目标，但项目必须先按 Qt 5.15 的方式查找 Qt 包。需要兼容 Qt 5.15 和 Qt 6 时，构建脚本必须明确选择实际的 Qt 主版本。详见 [Qt 5 和 Qt 6 的 CMake 兼容性](https://doc.qt.io/qt-6/cmake-qt5-and-qt6-compatibility.html)。

## 1. 命名

Qt 项目沿用 Qt 官方风格。下表是本仓库对 Qt 代码的具体约定：

| 实体 | 规则 | 示例 |
| --- | --- | --- |
| 类、结构体、枚举类型 | 大驼峰命名 | `FileModel`、`ConnectionState` |
| 公共函数和成员函数 | 小驼峰命名 | `loadFile()`、`isConnected()` |
| 信号和槽 | 小驼峰命名，使用动作或状态变化表达语义 | `fileLoaded()`、`onReadyRead()` |
| 非静态成员变量 | `m_` 加小驼峰命名 | `m_filePath`、`m_retryCount` |
| 局部变量和参数 | 小驼峰命名 | `filePath`、`retryCount` |
| 常量和枚举值 | 大写或项目现有风格，新增代码统一使用 `k` 加大驼峰 | `kMaxRetries` |
| Qt 属性 | 与属性名一致 | `enabled`、`filePath` |
| UI 控件成员 | `m_` 加控件用途和类型 | `m_saveButton`、`m_statusLabel` |

```cpp
class FileModel : public QObject
{
    Q_OBJECT

public:
    explicit FileModel(QObject* parent = nullptr);

    QString filePath() const;
    void setFilePath(const QString& filePath);

signals:
    void filePathChanged();

private:
    QString m_filePath;
};
```

- 成员变量必须使用 `m_` 前缀。Qt 代码中的成员变量不使用通用 C++ 规范的尾部下划线形式。
- 信号名称必须描述已经发生的事件或状态变化，例如 `fileLoaded()`、`errorOccurred()`。
- 槽名称必须描述响应动作。自动连接槽使用 `on_<objectName>_<signalName>()` 格式，例如 `on_saveButton_clicked()`。
- 禁止使用 `slot`、`signal` 等容易与 Qt 关键字混淆的名称作为普通变量名。

## 2. QObject 和对象所有权

- QObject 派生类必须在构造函数中接收 `QObject* parent = nullptr`，除非该类型明确不需要 QObject 父子关系。
- 创建 QObject 时，必须为具有明确生命周期关系的对象设置父对象。
- 父对象销毁时，Qt 会销毁其子对象。禁止手动 `delete` 已经交给父对象管理的 QObject。
- QObject 派生类禁止复制。应保留 Qt 默认的不可复制语义，禁止为 QObject 派生类实现复制构造函数和复制赋值运算符。
- QObject 的所有权转移必须通过 `setParent()`、构造函数 parent 参数或明确的智能指针策略完成。
- 跨线程移动 QObject 必须使用 `moveToThread()`，对象不能有父对象，且调用线程必须符合 Qt 的线程亲和性要求。

```cpp
auto* worker = new Worker(this);
```

上例中的 `new` 由 QObject 父子关系托管。除 QObject 父子所有权或 Qt API 明确要求裸指针的场景外，普通 C++ 代码仍遵循禁止直接使用 `new` 和 `delete` 的规则。

## 3. 元对象和 Qt 宏

- QObject 派生类使用信号、槽、`Q_PROPERTY` 或其他 Qt 元对象功能时必须包含 `Q_OBJECT`。
- 不继承 QObject 但需要枚举反射或其他受限元对象功能的值类型必须使用 `Q_GADGET`。
- `Q_OBJECT` 必须放在类定义的最前部，位于访问说明符之前。
- `Q_NAMESPACE` 只用于需要通过元对象系统注册命名空间枚举的命名空间。
- `Q_PROPERTY` 必须提供与属性语义一致的读写函数和变化信号。
- `Q_ENUM`、`Q_FLAG` 必须紧邻对应的枚举声明。
- 禁止手动修改 `moc`、`uic`、`rcc` 生成文件。
- 使用 `Q_OBJECT`、`Q_GADGET` 或 `Q_NAMESPACE` 的头文件必须纳入构建系统的 Qt 自动处理流程。

```cpp
class FileModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath
               NOTIFY filePathChanged)

public:
    explicit FileModel(QObject* parent = nullptr);

    QString filePath() const;
    void setFilePath(const QString& filePath);

signals:
    void filePathChanged();

private:
    QString m_filePath;
};
```

## 4. 信号、槽和连接

- 新代码必须优先使用函数指针形式的 `connect()`，禁止使用字符串形式连接新代码。
- `connect()` 必须保存连接上下文，跨对象连接时优先传入接收者或上下文对象。
- 连接返回的 `QMetaObject::Connection` 在需要断开时必须保存并使用。
- 一个信号连接多个槽时，必须保证槽的执行顺序不会影响结果。
- 槽函数必须能够安全处理重复调用。
- 使用 `Qt::UniqueConnection` 前必须确认连接对象和信号支持该连接类型。
- 只在确实需要手动断开时使用 `disconnect()`。对象销毁时 Qt 会自动清理相关连接。

```cpp
connect(&m_watcher, &QFileSystemWatcher::fileChanged,
        this, &FileModel::reloadFile);
```

禁止新代码使用：

```cpp
connect(sender, SIGNAL(fileChanged(QString)),
        receiver, SLOT(reloadFile(QString)));
```

## 5. 线程和事件循环

- QObject 的方法必须在其所属线程中调用，跨线程操作必须通过信号槽、事件或线程安全接口完成。
- 需要在工作线程运行的 QObject 必须先创建，再通过 `moveToThread()` 移动，并由线程启动后执行工作。
- 线程必须有明确的启动、停止和退出路径。
- 禁止在线程结束前销毁仍属于该线程的 QObject。
- 禁止在 GUI 线程执行阻塞 I/O、长时间计算或无限等待。
- QWidget 及其子类只能在 GUI 线程创建和操作。
- `Qt::BlockingQueuedConnection` 只能用于明确不会形成循环等待的调用。
- `QTimer` 必须在所属线程的事件循环中启动和停止。

## 6. Qt 类型和容器

- 文本接口必须根据语义选择 `QString`、`QStringView` 或项目目标版本支持的非拥有字符串类型。
- Qt 5.15 代码使用 `QLatin1String` 表示 Latin-1 非拥有字符串。Qt 6 代码可以使用 `QLatin1StringView`。
- `QUtf8StringView` 只允许在提供该类型的 Qt 6 目标中使用。Qt 5.15 代码使用 `QString`、`QByteArray` 或项目已有的 UTF-8 接口表达 UTF-8 数据。
- 二进制数据使用 `QByteArray`，禁止将二进制数据当作以 `'\0'` 结尾的 C 字符串处理。
- Qt API 边界优先使用 Qt 类型，标准库类型只在接口明确要求或与非 Qt 模块交互时使用。
- 需要隐式共享和 Qt 元对象集成时优先使用 Qt 容器。
- 遍历 Qt 容器时优先使用范围 `for`。
- 禁止在循环中反复触发不必要的隐式共享拷贝。
- 需要表达可选值时使用 `std::optional` 或项目已经统一采用的 Qt 类型。

```cpp
QString displayName(const QString& filePath);
QByteArray readRawData(QIODevice& device);
```

## 7. GUI 和模型视图

- QWidget 只能在 GUI 线程使用。
- UI 文件由 Qt Designer 管理时，禁止手动修改生成的 `ui_*.h` 文件。
- UI 控件的业务逻辑必须放在窗口类或模型类中，禁止把业务逻辑堆在自动连接槽中。
- 模型类必须正确实现 `rowCount()`、`columnCount()`、`data()` 和必要的索引验证。
- 修改模型数据前后必须发送对应的 `begin...` 和 `end...` 信号。
- 动态插入、删除或移动模型项时，禁止直接修改容器后只发送 `dataChanged()`。

## 8. 属性、资源和翻译

- 用户可见文本必须使用 `tr()`、`QObject::tr()` 或项目统一的翻译接口。
- 禁止把需要翻译的界面文本写成未标记的字符串字面量。
- Qt Resource System 中的路径必须使用项目统一的前缀和命名规则。
- 资源文件、`.ui` 文件和翻译文件必须通过构建系统管理。
- `QSettings` 的键名必须集中定义，禁止在多个文件中散落拼写相同的字符串。

```cpp
ui->saveButton->setText(tr("Save"));
```

## 9. Doxygen 注释

Qt 代码使用与普通 C++ 相同的 Doxygen 规范：

- 公共 QObject、QWidget、模型、信号、槽和属性必须提供 Doxygen 注释。
- 信号必须说明触发时机和参数含义。
- 槽必须说明响应动作、线程要求和重复调用行为。
- `Q_PROPERTY` 必须在注释中说明属性语义和 `NOTIFY` 信号。
- UI 交互行为必须说明用户可见结果。

通用写法见 [精简版 C++ 注释规范](cpp-doxygen-introduction.md) 和 [Doxygen 详细参考](cpp-doxygen-reference.md)，Qt 特有写法见 [Qt 注释规范](qt-doxygen-guidelines.md)。

## 10. 构建和检查

- Qt 5.15 模块必须通过 CMake `find_package(Qt5 5.15 REQUIRED COMPONENTS ...)` 或项目统一的 qmake 配置声明。Qt 6 模块必须通过 `find_package(Qt6 REQUIRED COMPONENTS ...)` 声明。
- Qt 类型使用的模块必须在构建目标中显式链接。Qt 5.15 使用 `Qt5::Core`、`Qt5::Gui` 或 `Qt5::Widgets`，Qt 6 使用对应的 `Qt6::` 目标。
- 使用 `Q_OBJECT` 的 CMake 目标必须启用 Qt 的自动 MOC、UIC 和 RCC 处理。使用 qmake 的项目必须将相关源文件纳入对应的 `SOURCES`、`HEADERS`、`FORMS` 或 `RESOURCES` 配置。
- 禁止提交构建目录、`moc_*.cpp`、`ui_*.h`、`qrc_*.cpp` 等生成文件，除非项目明确要求提交。
- 提交前必须运行项目已有的构建和测试命令。
- 文档或配置修改至少运行 `git diff --check`。

Qt API 细节以项目实际使用版本的 [Qt 5.15 Documentation](https://doc.qt.io/qt-5.15/) 或 [Qt 6 Documentation](https://doc.qt.io/qt-6/) 为准。项目自己的规则以本文为准。
