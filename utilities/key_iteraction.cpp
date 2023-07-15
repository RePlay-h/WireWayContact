#include "key_iteraction.h"

#include <QDebug>

void utilities::PressKey(QByteArray &buf) {

    int key = buf[1] << 24;
    key |= buf[2] << 16;
    key |= buf[3] << 8;
    key |= buf[4];

    #ifdef Q_OS_WIN

    quint8 res_key = win::ResolveKeyCode(key);

    keybd_event(res_key, 0, 0, 0);
    keybd_event(res_key, 0, KEYEVENTF_KEYUP, 0);

    #elif Q_OS_LINUX

    #endif
}


quint8 utilities::win::ResolveKeyCode(quint32 key) {

#ifdef Q_OS_WIN

    // for ASCII


    // for F1 - F24
    if(key >= 0x01000030 && key <= 0x01000047) {
        return key - 0xFFFFC0;
    }


    switch (key) {
    case Qt::Key_Return:
        return VK_RETURN;

    case Qt::Key_Enter:
        return VK_RETURN;

    case Qt::Key_Backspace:
        return VK_BACK;

    case Qt::Key_Escape:
        return VK_ESCAPE;

    case Qt::Key_Tab:
        return VK_TAB;

    case Qt::Key_Delete:
        return VK_DELETE;

    case Qt::Key_Shift:
        return VK_SHIFT;

    case Qt::Key_Control:
        return VK_CONTROL;

    case Qt::Key_Minus:
        return VK_OEM_MINUS;

    case Qt::Key_Slash:
        return VK_OEM_2;

    case Qt::Key_Space:
        return VK_SPACE;

    case Qt::Key_CapsLock:
        return VK_CAPITAL;

    case Qt::Key_Alt:
        return VK_MENU;


    }



    if((key >= 0x30 && key <= 0x39) || (key >= 0x41 && key <= 0x5A)) {
        return key;
    }
#endif

    return 0;

}
