#ifndef KEY_ITERACTION_H
#define KEY_ITERACTION_H

#include <QByteArray>

#ifdef Q_OS_WIN
    #include <Windows.h>
#elif Q_OS_LINUX

#endif


namespace utilities {

    void PressKey(QByteArray &buf);

    namespace win {
        quint8 ResolveKeyCode(quint32 key);
    }
}

#endif // KEY_ITERACTION_H
