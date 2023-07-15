#ifndef HANDLERS_H
#define HANDLERS_H

#include <QScreen>
#include <QImage>
#include <QGuiApplication>
#include <QPixmap>
#include <QList>
#include <QBuffer>


#define CONNECT_BYTE 0x10
#define CONNACK_BYTE 0x20
#define SCREEN_BYTE 0x50
#define INTERCAPT_BYTE 0x40
#define DISCONNECT_BYTE 0x70
#define COMMAND_BYTE 0x80

#define MOUSE_MOVE 0x00
#define KEY_CLICK 0x01
#define MOUSE_PRESS 0x03
#define MOUSE_RELEASE 0x04


namespace utilities {

   void CreateConnectPacket(QByteArray &buf, quint16 &id);

   void CreateScreenPackets(QList<QByteArray> &screens, quint16 &id);

   void CreateScreen(QList<QByteArray> &screens, quint8 id);

   void CreateDisconnectPacket(QByteArray &buf);



} //utilities


#endif // HANDLERS_H
