// 这些头文件的顺序由构建系统和生成工具约定，不能依赖自动排序。
#include "qt_special_format_check.h"
#include <QObject>
#include <QString>
#include "generated/ui_qt_special_format_check.h"

#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

// 生成代码或外部工具要求保留原始布局。
// clang-format off
#define QT_SPECIAL_PROPERTY(name, value) \
    Q_PROPERTY(QString name READ name WRITE set##value NOTIFY name##Changed)

static const char *const kGeneratedMetadata[] = {
    "qt.special",
    "generated",
    nullptr
};
// clang-format on

class SpecialFormatProbe : public QObject {
    Q_OBJECT
    QT_SPECIAL_PROPERTY(displayName, DisplayName)
    Q_PROPERTY(QString name READ name WRITE set##value NOTIFY name##Changed)

public:
    explicit SpecialFormatProbe(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void displayNameChanged(const QString &name);
};

Q_DECLARE_METATYPE(SpecialFormatProbe *)

// AUTOMOC 生成的文件必须保留在实现文件末尾。
#include "qt_special_format_check.moc"
