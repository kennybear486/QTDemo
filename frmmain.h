#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include "mytcpserver.h"

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:

    //客户端槽函数
    void ReadData();
    void ReadError(QAbstractSocket::SocketError);

    //服务端槽函数
    void ClientReadData(int clientID,QString IP,int Port,QByteArray data);
    void ClientConnect(int clientID,QString IP,int Port);
    void ClientDisConnect(int clientID,QString IP,int Port);

    void on_btnSendServer_clicked();

    void on_btnSendClient_clicked();

    void on_btnListen_clicked();

    void on_btnClearServer_clicked();

    void on_btnConnect_clicked();

    void on_btnClearClient_clicked();

    void on_cboxMax_currentIndexChanged(int index);

private:
    Ui::frmMain *ui;

    QTcpSocket *tcpClient;
    myTcpServer *tcpServer;

    void Init();
};

#endif // FRMMAIN_H
