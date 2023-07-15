#include "creators.h"

void utilities::CreateConnectPacket(QByteArray &buf, quint16 &id)
{
    buf.resize(7);

    //type
    buf[0] = CONNECT_BYTE;
    buf[0] |= 0x01;

    //size
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x02;

    //id
    buf[5] = (id >> 8);
    buf[6] = id;
}

void utilities::CreateScreenPackets(QList<QByteArray> &screens, quint16 &id) {


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

        screens.push_back(QByteArray());
        QByteArray& scr = screens.back();

        scr.resize(7);

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

        for(size_t i = 0; i < step; ++i) {
            scr.push_back(*(buf + i));
        }

        size_of_screen -= step;
        buf += step;
    }
}

void utilities::CreateScreen(QList<QByteArray> &screens, quint8 id) {

    QScreen* display = QGuiApplication::primaryScreen();
    QPixmap ScreenShot = display->grabWindow(0);

    ScreenShot = ScreenShot.scaled(1600, 900, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QByteArray bytes;

    QBuffer buffer(&bytes);

    ScreenShot.save(&buffer, "JPEG");

    const char* b = bytes.constData();

    qsizetype bytes_size = bytes.size();

    quint8 i = 0;

    quint8 n_pkt = std::ceil(double(bytes_size) / 57600);


    while(bytes_size >= 0) {

        QByteArray scr;

        scr.append(id);
        scr.append(i);
        scr.append(n_pkt);

        scr.append(b, bytes_size > 57600 ? 57600 : bytes_size);

        b += bytes_size > 57600 ? 57600 : bytes_size;

        bytes_size -= 57600;

        screens.push_back(std::move(scr));

        ++i;
    }

}



void utilities::CreateDisconnectPacket(QByteArray &buf) {

    buf.resize(5);

    buf[0] = DISCONNECT_BYTE;
    buf[1] = 0x00;
    buf[2] = 0x00;
    buf[3] = 0x00;
    buf[4] = 0x00;
}


