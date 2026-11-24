# Qt 注释规范

本文是 [C++ 注释规范](cpp-doxygen-introduction.md) 的 Qt 专属补充，适用于 Qt 5.15.2 和 Qt 6。通用 Doxygen 注释块、关键字和注释格式以 C++ 注释规范为准，本文只约定 Qt API 特有的语义。

## 适用范围

以下 Qt 公共 API 必须提供 Doxygen 注释：

- `QObject`、`QWidget`、`Q_GADGET` 和 `Q_NAMESPACE` 相关类型。
- 信号、槽、`Q_INVOKABLE` 函数和公共成员函数。
- `Q_PROPERTY`、`Q_ENUM`、`Q_FLAG` 及其关联的枚举。
- `QAbstractItemModel` 派生类的角色、索引和数据行为。
- 对用户可见的窗口、控件、动作和交互行为。

## Qt 类型和对象

类注释必须说明职责、对象所有权和线程要求。QObject 派生类还必须说明父对象关系或独立生命周期。

```cpp
/**
 * @brief 管理待处理文件的元数据。
 *
 * 对象由父对象负责销毁。所有公共成员函数必须在对象所属线程调用。
 */
class FileModel : public QObject
{
    Q_OBJECT
};
```

## 属性

`Q_PROPERTY` 的注释必须说明属性含义、读写行为、默认值和 `NOTIFY` 信号。属性的读函数、写函数和变化信号不需要重复相同描述，只需补充各自独有的约束。

```cpp
/**
 * @brief 当前文件路径。
 *
 * 设置新路径后发出 filePathChanged()。空路径表示尚未选择文件。
 */
Q_PROPERTY(QString filePath READ filePath WRITE setFilePath
           NOTIFY filePathChanged)
```

## 信号

信号注释必须说明触发时机和参数含义。信号表示已经发生的事件或状态变化，不描述调用者要求执行的动作。

```cpp
/**
 * @brief 文件路径发生变化时发出。
 *
 * @param path 变化后的文件路径。
 */
void filePathChanged(const QString& path);
```

## 槽和可调用函数

槽和 `Q_INVOKABLE` 函数的注释必须说明响应动作、调用线程和重复调用行为。异步操作还必须说明结果通过哪个信号返回。

```cpp
/**
 * @brief 重新加载当前文件。
 *
 * 可重复调用。函数在对象所属线程执行，完成后发出 reloadFinished()。
 */
public slots:
    void reload();
```

## 枚举和标志

使用 `Q_ENUM` 或 `Q_FLAG` 的枚举必须说明每个枚举值的含义。组合标志必须说明哪些值可以组合，以及零值代表的状态。

```cpp
/**
 * @brief 文件处理状态。
 */
enum class ProcessingState
{
    Idle,    ///< 尚未开始处理。
    Running, ///< 正在处理。
    Failed   ///< 处理失败。
};
Q_ENUM(ProcessingState)
```

## Model/View

模型注释必须说明：

- 每个自定义 role 的名称、类型和含义。
- `data()` 在无效索引或未知 role 下的返回行为。
- 插入、删除和移动数据时发出的模型信号。
- 数据是否允许编辑，以及 `setData()` 成功后的通知信号。

```cpp
/**
 * @brief 返回指定索引和角色对应的数据。
 *
 * @param index 有效的模型索引。
 * @param role 要读取的数据角色。DisplayRole 返回文件名，
 *             FilePathRole 返回完整路径。
 * @return 角色对应的数据。索引无效或角色未知时返回无效 QVariant。
 */
QVariant data(const QModelIndex& index,
              int role = Qt::DisplayRole) const override;
```

## QWidget 和用户界面

窗口、控件和动作的注释必须说明用户可见的结果。事件处理函数必须说明接受、忽略或转发事件的条件。自动连接槽必须说明对应的控件和信号。

```cpp
/**
 * @brief 保存按钮被点击时保存当前编辑内容。
 *
 * 保存成功后更新状态栏并发出 documentSaved()。
 * 保存失败时保留编辑内容并显示错误信息。
 */
void on_saveButton_clicked();
```

## QML 和元对象接口

供 QML 或其他元对象调用的 `Q_INVOKABLE` 函数、属性和信号必须说明：

- QML 可见名称或属性名称。
- 参数和返回值在元对象中的类型。
- 调用线程和对象生命周期要求。
- 异步结果对应的信号。

## 注释位置和维护要求

- Qt 公共声明优先在头文件中注释，源文件只补充实现原因和算法约束。
- 属性、信号、槽和枚举的注释应紧邻对应声明。
- 不重复抄写 `Q_PROPERTY`、函数名或信号名已经表达的信息。
- `@param`、`@return`、`@note` 等通用关键字的用法见 [C++ 注释规范](cpp-doxygen-introduction.md)。
- 完整 Doxygen 关键字和高级用法见 [Doxygen 详细参考](cpp-doxygen-reference.md)。

