#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),
      sock_(new QTcpSocket(this)),
      pkt_size_(0),
      pkt_type_(0),
      scr_id_(0),
      port_(8888)
{
    ui->setupUi(this);

    setWindowTitle("WireWayContact");

    setWindowIcon(QIcon(":/icons/icon/icon.png"));
}

void MainWindow::Start(QString address) {

    QObject::connect(sock_, &QTcpSocket::readyRead, this, &MainWindow::ReadyReadSlot);

    QObject::connect(sock_, &QTcpSocket::errorOccurred, this, &MainWindow::ErrorHandler);

    QObject::connect(&timer_, &QTimer::timeout, this, &MainWindow::SendScreens);

    QObject::connect(sock_, &QTcpSocket::connected, this, &MainWindow::SendConnectPacket);

   sock_->connectToHost(address, port_);


}

void MainWindow::ErrorHandler(QAbstractSocket::SocketError socketError) {

   ui->Connect->setEnabled(true);

   DestroyTcpConnection();

   QMessageBox::critical(this, "Oooops...", "Error occurred while communicating with the server!");

}

void MainWindow::SendConnectPacket()
{
    QByteArray buf;

    // generate random id

    id_ = QRandomGenerator::global()->bounded(65535);

    utilities::CreateConnectPacket(buf, id_);

    // send CONNECT packet
    sock_->write(buf);


    // Start the timer to start sending Screen packets
    timer_.start(7000);
}

void MainWindow::ReadyReadSlot() {

    QDataStream in(sock_);
    in.setVersion(QDataStream::Qt_6_3);

    for(;;) {

        // If we haven't received the package yet
        if(pkt_size_ == 0) {

            if(sock_->bytesAvailable() < 5) {
                break;
            }


            // Read packet type and packet size
            in >> pkt_type_;
            in >> pkt_size_;

        }

        if(sock_->bytesAvailable() < pkt_size_) {
            break;
        }

        // Read the rest of the packet
        for(quint32 i = 0; i < pkt_size_; ++i) {
            quint8 byte;
            in >> byte;
            remain_part_.push_back(byte);
        }

        PacketHandler();


    }
}

void MainWindow::SendScreens() {

    if(packets_.empty())
        utilities::CreateScreenPackets(packets_, id_);

    if(!sock_->isOpen()) {

        DestroyTcpConnection();
        return;
    }

    // send packet from list of packets

    if(sock_->write(packets_.first()) == -1) {
        DestroyTcpConnection();
        return;
    }
    packets_.pop_front();

    timer_.start(500);
}

void MainWindow::ChangeSocket(QString address) {

    address_ = address;
    port_ = 1225;

    socket_ = new QUdpSocket(this);

    if(!socket_->bind(QHostAddress::AnyIPv4, port_)) {
        QMessageBox::critical(this, "Oooops...", "Error occurred while configuring the program!");
        return;
    }

    packets_.clear();

    QObject::connect(&timer_, &QTimer::timeout, this, &MainWindow::SendScreen);
    QObject::connect(socket_, &QUdpSocket::readyRead, this, &MainWindow::ReadyReadCommand);

    timer_.start(1000);
}

void MainWindow::PacketHandler() {

    switch(pkt_type_ & 0xF0) {

    case CONNACK_BYTE:
        if(pkt_type_ & 0x0F) {

            QMessageBox::critical(this, "Oooops...", "Failed to connect to the server");

            DestroyTcpConnection();

        }
        break;

    case INTERCAPT_BYTE:

        DestroyTcpConnection();

        ChangeSocket(QString::fromUtf8(remain_part_));

        QMessageBox::information(this, "Information", "You get INTERCAPT");


        break;

    case COMMAND_BYTE:

        switch(pkt_type_ & 0x0F) {

        case MOUSE_MOVE:
            utilities::SetMousePos(remain_part_);
            break;

        case KEY_CLICK:
            utilities::PressKey(remain_part_);
            break;

        case MOUSE_PRESS:
            utilities::MousePress(remain_part_[1] & 0x0F);
            break;

        case MOUSE_RELEASE:
            utilities::MouseRelease(remain_part_[1] & 0x0F);


        }

    }
}

void MainWindow::SendScreen() {

    utilities::CreateScreen(packets_, scr_id_);

    if(scr_id_ == 127)
        scr_id_ = 0;
    else
        ++scr_id_;


    for(auto &part : packets_) {

        socket_->writeDatagram(part, QHostAddress(address_), port_);

        socket_->flush();

        QThread::currentThread()->msleep(2);
    }



    packets_.clear();
    timer_.start(5);
}

void MainWindow::ReadyReadCommand() {

    while(socket_->hasPendingDatagrams()) {

        QNetworkDatagram datagram = socket_->receiveDatagram();

        remain_part_ = std::move(datagram.data());
        pkt_type_ = remain_part_[0];

        PacketHandler();

    }

}

MainWindow::~MainWindow()
{
    DestroyTcpConnection();
    delete ui;
}

void MainWindow::DestroyTcpConnection() {

    QObject::disconnect(sock_, &QTcpSocket::readyRead, this, &MainWindow::ReadyReadSlot);
    QObject::disconnect(sock_, &QTcpSocket::errorOccurred, this, &MainWindow::ErrorHandler);
    QObject::disconnect(&timer_, &QTimer::timeout, this, &MainWindow::SendScreens);

    if(sock_->isOpen()) {

        QByteArray pkt;
        utilities::CreateDisconnectPacket(pkt);

        sock_->write(pkt);

        sock_->flush();
    }

}

void MainWindow::on_Connect_clicked() {

    ui->Connect->setEnabled(false);

    Start(S_ADDRESS);


}

