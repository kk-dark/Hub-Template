#include "qt_additional_edge_cases_format_check.h"
#include <QMetaObject>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <QtGlobal>
#include <QtPlugin>
#include <optional>

class TrackEntity;

class TrackModel : public QObject
{
    Q_OBJECT
    Q_MOC_INCLUDE("domain/track_entity.h")

    // Q_MOC_INCLUDE、Q_PROPERTY 和 Q_CLASSINFO 属于同一组元对象声明。
    // clang-format off
    Q_CLASSINFO("Version", "1.2.0")
    Q_CLASSINFO("Author", "SDK Team")
    Q_PROPERTY(TrackEntity *entity READ entity NOTIFY entityChanged)
    // clang-format on

public:
    explicit TrackModel(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    TrackEntity *entity() const
    {
        return entity_;
    }

signals:
    void entityChanged(TrackEntity *entity);

private:
    TrackEntity *entity_ = nullptr;
};

class TrackEditorPrivate;

class TrackEditor : public QWidget
{
    Q_OBJECT
    // clang-format off
    Q_DECLARE_PRIVATE(TrackEditor)
    // clang-format on

public:
    explicit TrackEditor(QWidget *parent = nullptr);

private:
    QScopedPointer<TrackEditorPrivate> d_ptr;
};

class TrackEditorPrivate
{
    Q_DECLARE_PUBLIC(TrackEditor)

public:
    TrackEditor *q_ptr = nullptr;
};

TrackEditor::TrackEditor(QWidget *parent)
    : QWidget(parent)
    , d_ptr(new TrackEditorPrivate)
{
    Q_D(TrackEditor);
    d->q_ptr = this;
}

namespace display_modes
{
enum class DisplayMode
{
    Idle,        ///< 空闲
    Calculating, ///< 计算中
    Finished     ///< 导出完成
};

Q_NAMESPACE
Q_ENUM_NS(DisplayMode)
} // namespace display_modes

struct TrackResult
{
    QString value;
};

void invokeWithComplexArguments(QObject *object,
                                const std::optional<TrackResult> &result,
                                QString *text)
{
    // 模板参数中包含逗号时，保持 Q_ARG 和 Q_RETURN_ARG 的结构。
    // clang-format off
    QMetaObject::invokeMethod(
        object,
        "onDataReady",
        Qt::QueuedConnection,
        Q_ARG(std::optional<TrackResult>, result),
        Q_ARG(bool, result.has_value()),
        Q_RETURN_ARG(QString, *text)
    );
    // clang-format on
}

void connectWithLongLambda(QObject *loader, QObject *receiver)
{
    // 多行捕获和连续 emit 的 lambda 保持明确的业务步骤。
    // clang-format off
    QObject::connect(loader, SIGNAL(loaded(QString)), receiver, [receiver](const QString &value) {
        Q_UNUSED(receiver);
        Q_UNUSED(value);
        emit static_cast<TrackModel *>(receiver)->entityChanged(nullptr);
    });
    // clang-format on
}

const char *translatedText()
{
    // lupdate 需要看到完整的静态翻译源文本。
    // clang-format off
    static constexpr const char *kExportingText = QT_TR_NOOP("正在导出测线 %1");
    static constexpr const char *kTranslatedText = QT_TRANSLATE_NOOP3("Track", "导出完成", "status");
    static constexpr const char *kIdText = QT_TRID_NOOP("track.export.finished");
    // clang-format on
    return kExportingText;
}

class ITrackPlugin
{
public:
    virtual ~ITrackPlugin() = default;
};

Q_DECLARE_INTERFACE(ITrackPlugin, "com.example.ITrackPlugin")

class TrackPlugin : public QObject, public ITrackPlugin
{
    Q_OBJECT
    // clang-format off
    Q_PLUGIN_METADATA(IID "com.example.TrackPlugin")
    Q_INTERFACES(ITrackPlugin)
    // clang-format on
};

class ConditionalMetaObject : public QObject
{
    Q_OBJECT
#if QT_DEBUG
    // clang-format off
    Q_PROPERTY(QString debugInfo READ debugInfo)
    // clang-format on
#endif
};

void useLegacyQtIteration(const QStringList &values)
{
    QString value;
    Q_FOREACH (value, values)
    {
        Q_UNUSED(value);
    }

    Q_FOREVER
    {
        break;
    }
}

// moc 文件必须放在 cpp 末尾，不能放进命名空间。
#include "qt_additional_edge_cases_format_check.moc"
