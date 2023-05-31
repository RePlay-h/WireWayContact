#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w(tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 8888));

    w.show();
    return a.exec();
}
