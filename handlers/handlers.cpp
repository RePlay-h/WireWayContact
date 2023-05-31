#include "handlers.h"

void utilities::CreateConnectPacket(std::vector<uint8_t> &buf, uint16_t id)
{
    buf.resize(7);

    //type
    buf[0] = CONNECT_BYTE;

    //size
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x02;

    //id
    buf[5] = (id >> 8);
    buf[6] = id;
}

void utilities::CreateScreenPackets(std::list<std::vector<uint8_t>> &screens, uint16_t &id) {


    QScreen* display = QGuiApplication::primaryScreen();
    QPixmap ScreenShot = display->grabWindow(0);

    QImage img;
    img = ScreenShot.toImage();

    img = img.scaled(300, 200);

    unsigned char *buf = img.bits();

    size_t size_of_screen = img.sizeInBytes();

    while(size_of_screen != 0) {

        size_t step;

        if(size_of_screen > 65528) {
            step = 65535;
        } else {
            step = size_of_screen + 7;
        }

        screens.push_back(std::vector<uint8_t>(7));
        std::vector<uint8_t>& scr = screens.back();



        scr[0] = (step == 65535) ? (SCREEN_BYTE | 0x01) : SCREEN_BYTE;
        scr[0] |= (size_of_screen == img.sizeInBytes()) ? 0x02 : 0x00;

        step -= 5;

        scr[1] = (step >> 24);
        scr[2] = (step >> 16);
        scr[3] = (step >> 8);
        scr[4] = step;
        scr[5] = (id >> 8);
        scr[6] = id;

        step -= 2;

        std::copy_n(buf, step, back_inserter(scr));
        scr.resize(7 + step);
        size_of_screen -= step;
        buf += step;
    }
}
