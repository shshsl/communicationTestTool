#include <QObject>
#include <QString>

#include "include/testapp.h"

TestApp::TestApp(QObject *parent) :
    m_textValue("")
{

}

TestApp::~TestApp()
{

}

void TestApp::setTextValue(const QString &value)
{
        if (m_textValue != value) {
            m_textValue = value;

            emit textValueChanged();
        }
}

void TestApp::onButton1Clicked()
{
    setTextValue("Button 1 Clicked");
}

void TestApp::onButton2Clicked()
{
    setTextValue("Button 2 Clicked");
}
