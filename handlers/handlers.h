#ifndef HANDLERS_H
#define HANDLERS_H

#include <QScreen>
#include <QImage>
#include <QGuiApplication>
#include <QPixmap>
#include <QList>

#define CONNECT_BYTE 0x10
#define CONNACK_BYTE 0x20
#define SCREEN_BYTE 0x50
#define INTERCAPT_BYTE 0x40
#define DISCONNECT_BYTE 0x70

#define MOUSE_MOVE 0x00
#define KEY_CLICK 0x01
#define MOUSE_CLICK 0x02
#define MOUSE_DBCLICK 0x03


namespace utilities {

   void CreateConnectPacket(QByteArray &buf, uint16_t id);

   void CreateScreenPackets(QList<QByteArray> &screens, uint16_t &id);

   void CreateScreen(QList<QByteArray> &screens);

   void CreateDisconnectPacket(QByteArray &buf);

} //utilities


#endif // HANDLERS_H
