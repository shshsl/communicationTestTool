#ifndef COMMUNICATIONENUM_H
#define COMMUNICATIONENUM_H

#include <QObject>
#include <QMetaType>

namespace Communication
{
    class UartEnums
    {
        Q_GADGET // Q_GADGET을 사용하여 메타 데이터 지원
        Q_ENUMS(SerialPortState) // Qt 5.6에서 Q_ENUMS로 enum 등록

    public:
        enum SerialPortState
        {
            SERIAL_PORT_NONE = 0,
            SERIAL_PORT_OPEN = 1,
            SERIAL_PORT_CLOSE = 2,
        };
    };
}
// 열거형을 QVariant로 사용하려면 Q_DECLARE_METATYPE 필요
Q_DECLARE_METATYPE(Communication::UartEnums::SerialPortState)

namespace SocketCommunication
{

}

#endif // COMMUNICATIONENUM_H
