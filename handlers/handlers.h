#ifndef HANDLERS_H
#define HANDLERS_H

#include <QScreen>
#include <QImage>
#include <QGuiApplication>
#include <QPixmap>

#include <vector>
#include <list>
#include <cstdint>

#define CONNECT_BYTE 0x11
#define SCREEN_BYTE 0x50

namespace utilities {

   void CreateConnectPacket(std::vector<uint8_t> &buf, uint16_t id);

   void CreateScreenPackets(std::list<std::vector<uint8_t>> &screens, uint16_t &id);

} //utilities


#endif // HANDLERS_H
