#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QMessageBox>
#include <QScreen>

#include <thread>
#include <chrono>
#include <random>

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/use_future.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/redirect_error.hpp>

#include "handlers/handlers.h"

using namespace boost;
using namespace std::chrono_literals;
using asio::ip::tcp;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(tcp::endpoint endpoint, QWidget *parent = nullptr);

    void RunIo();

    void Start();

    bool SendConnectPacket();

    bool GetConnackPacket();

    asio::awaitable<void> SendScreens();

    ~MainWindow();

private slots:
    void on_Connect_clicked();

private:
    Ui::MainWindow *ui;

    uint16_t id_;

    asio::io_context io_;
    asio::io_context::work work_{io_};

    tcp::endpoint endpoint_;

    tcp::socket sock_;

    std::thread thr_;

};
#endif // MAINWINDOW_H
