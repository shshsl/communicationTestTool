#ifndef COMMUNICATIONENUM_H
#define COMMUNICATIONENUM_H

#include <QObject>
#include <QMetaType>

namespace Communication
{
    class Uart
    {
        Q_GADGET // Q_GADGET을 사용하여 메타 데이터 지원
        Q_ENUMS(SerialPortState) // Qt 5.6에서 Q_ENUMS로 enum 등록

    public:
        enum SerialPortState
        {
            None = 0,
            Open = 1,
            Close = 2,
        };
    };
    
    class Socket
    {
        Q_GADGET
        Q_ENUMS(ConnectOption)
        
    public:
        enum ConnectOption
        {
            None = 0,
            Server,
            Client,
            Udp,
            IpAddress,
            Port,
        };
    };
}
// 열거형을 QVariant로 사용하려면 Q_DECLARE_METATYPE 필요
Q_DECLARE_METATYPE(Communication::Uart::SerialPortState)
Q_DECLARE_METATYPE(Communication::Socket::ConnectOption)

#endif // COMMUNICATIONENUM_H
