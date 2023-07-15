#ifndef MOUSE_ITERACTION_H
#define MOUSE_ITERACTION_H

#include <QByteArray>
#include <QApplication>
#include <QScreen>

#ifdef Q_OS_WIN
    #include <Windows.h>
#elif Q_OS_LINUX

#endif


namespace utilities {


    void SetMousePos(QByteArray &buf);

    void MousePress(bool is_left);

    void MouseRelease(bool is_left);
}

#endif // MOUSE_ITERACTION_H
