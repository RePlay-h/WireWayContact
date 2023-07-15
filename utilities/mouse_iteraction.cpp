#include "mouse_iteraction.h"


void utilities::SetMousePos(QByteArray &buf) {

    int x = uint8_t(buf[1]) << 24;

    x |= uint8_t(buf[2]) << 16;

    x |= uint8_t(buf[3]) << 8;

    x |= uint8_t(buf[4]);

    int y = uint8_t(buf[5]) << 24;
    y |= uint8_t(buf[6]) << 16;
    y |= uint8_t(buf[7]) << 8;
    y |= uint8_t(buf[8]);

    QScreen* scr = QApplication::screens().at(0);

    QSize screen_size = scr->availableSize();

    double k_x = double(screen_size.width()) / 1600;
    double k_y = double(screen_size.height()) / 900;

    int res_x = double(x) * k_x;

    int res_y = double(y) * k_y;

    #ifdef Q_OS_WIN
        SetCursorPos(res_x, res_y);
    #elif Q_OS_LINUX

    #endif
}

void utilities::MousePress(bool is_left) {
#ifdef Q_OS_WIN

        mouse_event(is_left ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);

#elif Q_OS_LINUX

#endif
}

void utilities::MouseRelease(bool is_left) {
#ifdef Q_OS_WIN


        mouse_event(is_left ? MOUSEEVENTF_LEFTUP : MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);

#elif Q_OS_LINUX

#endif
}


