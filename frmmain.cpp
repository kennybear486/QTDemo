#include "frmmain.h"
#include "ui_frmmain.h"
#include "myhelper.h"

frmMain::frmMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    myHelper::FormInCenter(this);
    this->Init();
}

frmMain::~frmMain()
{
    delete ui;
}

void frmMain::Init()
{
    ui->txtIP->setText("192.168.1.186");
    ui->txtPortClient->setText("60000");
    ui->txtPortServer->setText("60000");

    ui->btnSendClient->setEnabled(false);
    ui->txtDataClient->setReadOnly(true);

    tcpClient=new QTcpSocket(this);
    tcpClient->abort();//取消原有连接
    connect(tcpClient,SIGNAL(readyRead()),this,SLOT(ReadData()));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(ReadError(QAbstractSocket::SocketError)));

    ui->btnSendServer->setEnabled(false);
    ui->txtDataServer->setReadOnly(true);

    tcpServer=new myTcpServer(this);

    connect(tcpServer,SIGNAL(ClientConnect(int,QString,int)),
            this,SLOT(ClientConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientDisConnect(int,QString,int)),
            this,SLOT(ClientDisConnect(int,QString,int)));
    connect(tcpServer,SIGNAL(ClientReadData(int,QString,int,QByteArray)),
            this,SLOT(ClientReadData(int,QString,int,QByteArray)));

    for (int i=10;i<=1000;i=i+10)
    {
        ui->cboxMax->addItem(QString::number(i));
    }
}

void frmMain::ReadData()
{
    QByteArray buffer=tcpClient->readAll();
    if (!buffer.isEmpty())
    {
        if (ui->ckHexReceiveClient->isChecked())
        {
            ui->txtDataClient->append(tr("接收数据:%1 时间:%2")
                                      .arg(myHelper::ByteArrayToHexStr(buffer)).arg(QTime::currentTime().toString("hh:mm:ss")));
        }
        else
        {
            ui->txtDataClient->append(tr("接收数据:%1 时间:%2")
                                      .arg(QString(buffer)).arg(QTime::currentTime().toString("hh:mm:ss")));
        }
    }
}

void frmMain::ReadError(QAbstractSocket::SocketError)
{
    tcpClient->disconnectFromHost();
    ui->btnConnect->setText("连接");
    ui->txtDataClient->append(tr("连接服务器失败,原因:%1").arg(tcpClient->errorString()));
}

void frmMain::ClientReadData(int clientID,QString IP,int Port,QByteArray data)
{
    if (!data.isEmpty())
    {
        if (ui->ckHexReceiveServer->isChecked())
        {
            ui->txtDataServer->append(tr("接收数据:%1 来自:[clientID:%2 IP:%3 Port:%4] 时间:%5")
                                      .arg(QString(myHelper::ByteArrayToHexStr(data)))
                                      .arg(clientID).arg(IP).arg(Port).arg(QTime::currentTime().toString("hh:mm:ss")));
        }
        else
        {
            ui->txtDataServer->append(tr("接收数据:%1 来自:[clientID:%2 IP:%3 Port:%4] 时间:%5")
                                      .arg(QString(data)).arg(clientID).arg(IP).arg(Port).arg(QTime::currentTime().toString("hh:mm:ss")));
        }
    }
}

void frmMain::ClientConnect(int clientID,QString IP,int Port)
{
    ui->txtDataServer->append(tr("客户端:[clientID:%1 IP:%2 Port:%3]上线 时间:%4")
                              .arg(clientID).arg(IP).arg(Port).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->cboxClient->addItem(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port));
}

void frmMain::ClientDisConnect(int clientID,QString IP,int Port)
{
    ui->txtDataServer->append(tr("客户端:[clientID:%1 IP:%2 Port:%3]下线 时间:%4")
                              .arg(clientID).arg(IP).arg(Port).arg(QTime::currentTime().toString("hh:mm:ss")));
    ui->cboxClient->removeItem(ui->cboxClient->findText(tr("%1:%2:%3").arg(clientID).arg(IP).arg(Port)));
}

void frmMain::on_btnSendServer_clicked()
{
    QString txt=ui->txtSendServer->text();
    if (txt==""){return;}

    QString str=ui->cboxClient->currentText();
    int clientID=str.split(":")[0].toInt();
    QByteArray tempData;

    if (ui->ckHexSendServer->isChecked())
    {
        tempData=myHelper::HexStrToByteArray(txt);
        tcpServer->SendData(clientID,tempData);
    }
    else
    {
        tempData=txt.toLatin1();
        tcpServer->SendData(clientID,tempData);
    }
}

void frmMain::on_btnSendClient_clicked()
{
    QString data=ui->txtSendClient->text();

    if (data!="")
    {
        if (ui->ckHexSendClient->isChecked())
        {
            tcpClient->write(myHelper::HexStrToByteArray(data));
        }
        else
        {
            tcpClient->write(data.toLatin1());
        }
    }
}

void frmMain::on_btnListen_clicked()
{    
    if (ui->btnListen->text()=="监听")
    {
        bool ok=tcpServer->listen(QHostAddress::Any,ui->txtPortServer->text().toInt());
        if (ok)
        {
            ui->btnListen->setText("停止");
            ui->txtDataServer->append("监听成功");
            ui->btnSendServer->setEnabled(true);
        }
    }
    else
    {
        tcpServer->CloseAllClient();//断开所有已连接的客户端
        tcpServer->close();//停止监听
        ui->btnListen->setText("监听");
        ui->txtDataServer->append("停止监听成功");
        ui->btnSendServer->setEnabled(false);
    }
}

void frmMain::on_btnClearServer_clicked()
{
    ui->txtDataServer->clear();
}

void frmMain::on_btnConnect_clicked()
{
    if (ui->btnConnect->text()=="连接")
    {
        tcpClient->connectToHost(ui->txtIP->text(),ui->txtPortClient->text().toInt());
        if (tcpClient->waitForConnected(1000))
        {
            ui->btnConnect->setText("断开");
            ui->txtDataClient->append("连接成功");
            ui->btnSendClient->setEnabled(true);
        }
    }
    else
    {
        tcpClient->disconnectFromHost();
        if (tcpClient->state() == QAbstractSocket::UnconnectedState || tcpClient->waitForDisconnected(1000))
        {
            ui->btnConnect->setText("连接");
            ui->txtDataClient->append("断开连接成功");
            ui->btnSendClient->setEnabled(true);
        }
    }
}

void frmMain::on_btnClearClient_clicked()
{   
    ui->txtDataClient->clear();
}

void frmMain::on_cboxMax_currentIndexChanged(int index)
{
    tcpServer->setMaxPendingConnections(index+1);
}
