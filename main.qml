import QtQuick 2.0
import QtQuick.Controls 1.4

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Test App Example"

    Column {
        spacing: 20
        anchors.centerIn: parent

        // TextField와 TestApp의 textValue 속성 바인딩
        TextField {
            id: editField
            width: parent.width * 0.8
            placeholderText: "Enter some text..."
            text: testApp.textValue // TestApp의 textValue와 바인딩

            onTextChanged: testApp.setTextValue(text)
        }

        Row {
            spacing: 10

            Button {
                text: "Button 1"
                onClicked: testApp.onButton1Clicked() // Button 1 클릭 시 슬롯 호출
            }

            Button {
                text: "Button 2"
                onClicked: testApp.onButton2Clicked() // Button 2 클릭 시 슬롯 호출
            }
        }

        Label {
            text: "Current Value: " + testApp.textValue // 현재 값 표시
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
