#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define S_ADDRESS "127.0.0.1"

#include <QMainWindow>
#include <QPixmap>
#include <QMessageBox>
#include <QScreen>
#include <QTcpSocket>
#include <QUdpSocket>
#include <QTimer>
#include <QThread>
#include <QRandomGenerator>
#include <QNetworkDatagram>
#include <QIcon>

#include "utilities/creators.h"
#include "utilities/key_iteraction.h"
#include "utilities/mouse_iteraction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    void Start(QString address);

    void ErrorHandler(QAbstractSocket::SocketError socketError);

    void DestroyTcpConnection();

    // functions for interacting with the server

    void ReadyReadSlot();

    void PacketHandler();

    // functions for p2p connection

    void ChangeSocket(QString address);

    ~MainWindow();

private slots:


    void SendConnectPacket();

    void on_Connect_clicked();

    void SendScreens(); // send screen to server

    void SendScreen(); // send screen to user

    void ReadyReadCommand(); // read command

private:
    Ui::MainWindow *ui;

    QTcpSocket *sock_; // to communicate with the server

    QUdpSocket *socket_; // to communicate with the user

    QTimer timer_; // timer for SendScreens and SendScreen slots

    // packet

    quint8 pkt_type_; // packet type

    quint32 pkt_size_; // packet size

    QByteArray remain_part_; // remaining part

    // information for QUdpSocket

    QString address_; // user address

    quint16 port_; // user port


    quint16 id_; // contact id

    quint8 scr_id_; // screen packet id

    QList<QByteArray> packets_;

};


#endif // MAINWINDOW_H
