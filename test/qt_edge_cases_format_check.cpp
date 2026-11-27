#include "qt_edge_cases_format_check.h"
#include <QObject>
#include <QMetaType>
#include <QString>
#include <QtGlobal>

// 自动生成文件不应进入手写代码的格式化范围：
// moc_qt_edge_cases_format_check.cpp、ui_qt_edge_cases_format_check.h、qrc_qt_edge_cases.cpp。

namespace edge_cases {
Q_NAMESPACE

enum class State { Idle = 0x01, Running = 0x02, Finished = 0x04 };

Q_ENUM_NS(State)

enum Permission { Read = 0x01, Write = 0x02, Execute = 0x04 };

Q_DECLARE_FLAGS(Permissions, Permission)
Q_DECLARE_OPERATORS_FOR_FLAGS(Permissions)
} // namespace edge_cases

class EdgeCaseObject : public QObject {
    Q_OBJECT
#if defined(QT_QML_LIB)
    QML_ELEMENT
    QML_NAMED_ELEMENT(EdgeCaseObject)
    QML_SINGLETON
#endif

    // 连续的 Q_PROPERTY 需要保持可读的单行布局。
    // clang-format off
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int state READ state WRITE setState NOTIFY stateChanged)
    // clang-format on

public:
    explicit EdgeCaseObject(QObject *parent = nullptr) : QObject(parent) {}

    QString title() const { return title_; }

    int state() const { return state_; }

public slots:
    void setTitle(const QString &title) {
        if (title_ == title) {
            return;
        }
        title_ = title;
        emit titleChanged(title_);
    }

    void setState(int state) {
        if (state_ == state) {
            return;
        }
        state_ = state;
        emit stateChanged(state_);
    }

signals:
    void titleChanged(const QString &title);
    void stateChanged(int state);

private:
    QString title_;
    int state_ = 0;
};

void connectUsingFunctionPointers(EdgeCaseObject *object, QObject *receiver) {
    QObject::connect(object, &EdgeCaseObject::titleChanged, receiver, [](const QString &title) { Q_UNUSED(title); });
}

void connectUsingLegacySyntax(EdgeCaseObject *object, QObject *receiver) {
    // 旧式 SIGNAL/SLOT 字符串语法需要保留完整表达式。
    // clang-format off
    QObject::connect(object, SIGNAL(stateChanged(int)), receiver, SLOT(deleteLater()));
    // clang-format on
}

QString translatedTitle(const QString &name, int count) {
    // 翻译源文本应保持为一个完整字符串，避免 lupdate 产生不同消息。
    // clang-format off
    return QObject::tr("当前选中对象 %1，共 %2 个项目").arg(name).arg(count);
    // clang-format on
}

Q_DECLARE_METATYPE(EdgeCaseObject *)

// AUTOMOC 生成文件必须位于实现文件末尾。
#include "qt_edge_cases_format_check.moc"
