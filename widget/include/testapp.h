#ifndef TESTAPP_H
#define TESTAPP_H

#include <QObject>
#include <QString>

class TestApp : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString textValue READ textValue WRITE setTextValue NOTIFY textValueChanged)

public:
    explicit TestApp(QObject *parent = nullptr);
    ~TestApp();

    QString textValue() const { return m_textValue; }
    void setTextValue(const QString &value);

signals:
    void textValueChanged();

public slots:
    void onButton1Clicked();
    void onButton2Clicked();

private:
    QString m_textValue;
};

#endif // TESTAPP_H
