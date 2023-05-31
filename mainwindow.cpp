#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <iterator>

MainWindow::MainWindow(tcp::endpoint endpoint, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
     endpoint_(std::move(endpoint)),
     sock_{io_}
{
    ui->setupUi(this);
}

void MainWindow::RunIo() {
    thr_ = std::thread([this]() {io_.run();});
}

void MainWindow::Start() {
    asio::co_spawn(io_, SendScreens(), asio::detached);
}

bool MainWindow::SendConnectPacket()
{
    std::vector<uint8_t> buf;
    system::error_code ec;


    // generate random id
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint16_t> dist(0, 65535);

    id_ = dist(gen);

    utilities::CreateConnectPacket(buf, id_);

    // send CONNECT packet
    sock_.send(asio::buffer(buf, 7), 0, ec);

    if(ec != system::errc::success) {
        QMessageBox::critical(nullptr, "Oooops...", "No connection to server");
        return false;
    }

    return true;

}

bool MainWindow::GetConnackPacket()
{
    std::vector<uint8_t> buf;
    buf.resize(5);


    // get CONNACK packet
    std::future<size_t> fut = sock_.async_receive(asio::buffer(buf, 5), asio::use_future);


    switch (fut.wait_for(5s)) {
    case std::future_status::ready:

        // check the eighth bit of the first byte
        // If equal to 1, then the user with this id is already connected to the server
        // If equal to 0, the everything okay
        if(buf[0] & 0x01) {
            return false;
        }
        break;

    case std::future_status::timeout:
        return false;
    }

    qDebug() << "MY ID" << id_;

    return true;
}

asio::awaitable<void> MainWindow::SendScreens() {
    for(;;) {

        boost::system::error_code ec;

        std::list<std::vector<uint8_t>> packets;
        utilities::CreateScreenPackets(packets, id_);

        for(auto &pkt : packets) {

            // send packet from list of packets
            co_await sock_.async_send(asio::buffer(pkt, pkt.size()), 0, asio::redirect_error(asio::use_awaitable, ec));

            if(ec != boost::system::errc::success) {
                QMessageBox::critical(this, "Oooops...", "Server not responding");
                co_return;
            }

            std::this_thread::sleep_for(1s);


        }
   }
}

MainWindow::~MainWindow()
{
    io_.stop();
    thr_.join();
    delete ui;
}


void MainWindow::on_Connect_clicked() {

    sock_.connect(endpoint_);

    RunIo();
    SendConnectPacket();
    GetConnackPacket();
    Start();
}

