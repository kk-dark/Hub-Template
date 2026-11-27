#include "qt_comprehensive_format_check.h"
#include <QMetaObject>
#include <QObject>
#include <QString>
#include <QtGlobal>
#include <QtTest>

// 以下文件由 Qt 工具生成，不属于手写代码的格式化范围：
// moc_qt_comprehensive_format_check.cpp、ui_qt_comprehensive_format_check.h、qrc_qt_comprehensive_format_check.cpp。

QT_BEGIN_NAMESPACE
class GeneratedQtExtension;
QT_END_NAMESPACE

namespace comprehensive {
Q_NAMESPACE

enum class State { Idle = 0x01, Running = 0x02, Finished = 0x04 };

Q_ENUM_NS(State)

enum Feature { NoFeature = 0x00, Import = 0x01, Export = 0x02, Preview = 0x04 };

Q_DECLARE_FLAGS(Features, Feature)
Q_FLAG_NS(Features)
Q_DECLARE_OPERATORS_FOR_FLAGS(Features)

// 元类型宏连续出现时保留成组布局。
// clang-format off
Q_DECLARE_METATYPE(State)
Q_DECLARE_METATYPE(Features)
Q_DECLARE_METATYPE(QStringList)
// clang-format on
} // namespace comprehensive

struct GadgetData {
    Q_GADGET
    // Q_GADGET 与 Q_OBJECT 一样，元属性区块必要时局部关闭格式化。
    // clang-format off
    Q_CLASSINFO("Description", "Data exchanged with the Qt meta-object system")
    Q_PROPERTY(QString name MEMBER name)
    Q_PROPERTY(int revision MEMBER revision)
    // clang-format on

    QString name;
    int revision = 0;
};

class ComprehensiveObject : public QObject {
    Q_OBJECT
#if defined(QT_QML_LIB)
    QML_ELEMENT
    QML_NAMED_ELEMENT(ComprehensiveObject)
    QML_SINGLETON
#endif
    // clang-format off
    Q_CLASSINFO("Author", "format-check")
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(int revision READ revision WRITE setRevision NOTIFY revisionChanged)
    // clang-format on

public:
    explicit ComprehensiveObject(QObject *parent = nullptr) : QObject(parent) {}

    QString title() const { return title_; }

    int revision() const { return revision_; }

    Q_REVISION(1)
    Q_INVOKABLE void refresh() { emit refreshed(); }

public slots:
    void setTitle(const QString &title) {
        if (title_ == title) {
            return;
        }
        title_ = title;
        emit titleChanged(title_);
    }

    void setRevision(int revision) {
        if (revision_ == revision) {
            return;
        }
        revision_ = revision;
        emit revisionChanged(revision_);
    }

signals:
    void titleChanged(const QString &title);
    void revisionChanged(int revision);
    void refreshed();

private:
    QString title_;
    int revision_ = 0;
};

void invokeWithMetaObject(ComprehensiveObject *object, QString *result) {
    // 复杂 Q_ARG / Q_RETURN_ARG 调用按项目约定保留参数顺序。
    // clang-format off
    QMetaObject::invokeMethod(object, "title", Qt::DirectConnection,
                              Q_RETURN_ARG(QString, *result));
    QMetaObject::invokeMethod(object, "setRevision", Qt::QueuedConnection,
                              Q_ARG(int, 2));
    // clang-format on
}

void connectWithBothSyntaxes(ComprehensiveObject *object, QObject *receiver) {
    QObject::connect(object, &ComprehensiveObject::titleChanged, receiver,
                     [](const QString &title) { Q_UNUSED(title); });

    // 老式 SIGNAL/SLOT 语法保留为完整表达式。
    // clang-format off
    QObject::connect(object, SIGNAL(revisionChanged(int)), receiver, SLOT(deleteLater()));
    // clang-format on
}

QString translatedMessage(const QString &name) {
    // 翻译宏中的字符串不能被拆分为多个源文本。
    // clang-format off
    return QString::fromUtf8(QT_TR_NOOP("导出对象 %1")).arg(name);
    // clang-format on
}

class ComprehensiveQtTest : public QObject {
    Q_OBJECT

private slots:
    void compareValues() {
        const QString value = QStringLiteral("ready");
        QCOMPARE(value, QStringLiteral("ready"));
        QVERIFY(!value.isEmpty());
        QVERIFY2(value == QStringLiteral("ready"), "unexpected translated value");
    }

    void fetchValues() {
        QFETCH(QString, value);
        QVERIFY(!value.isNull());
    }

private:
    void fetch_data() {
        QTest::addColumn<QString>("value");
        QTest::newRow("default") << QStringLiteral("ready");
    }
};

QTEST_MAIN(ComprehensiveQtTest)

// AUTOMOC 生成文件必须位于实现文件末尾，且不能放进命名空间。
#include "qt_comprehensive_format_check.moc"
