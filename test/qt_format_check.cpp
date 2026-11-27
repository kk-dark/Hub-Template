#include <QObject>
#include <QString>

class FormatProbe : public QObject{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
public:
    explicit FormatProbe(QObject *parent=nullptr):QObject(parent){}
    QString name()const{return name_;}
public slots:
    void setName(const QString &name){if(name_==name)return;name_=name;emit nameChanged(name_);}
signals:
    void nameChanged(const QString &name);
private:
    QString name_;
};

void connectProbe(FormatProbe *probe,QObject *receiver){
    QObject::connect(probe,&FormatProbe::nameChanged,receiver,[receiver](const QString &name){Q_UNUSED(receiver);Q_UNUSED(name);});
}
