#include "newwindow.h"
#include "ui_newwindow.h"
#include <QVector>
#include <QTimer>
#include <QTime>

NewWindow::NewWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::NewWindow)
{
    ui->setupUi(this);
    system_init();

}

NewWindow::~NewWindow()
{
    delete ui;
}

void NewWindow::system_init()
{

    //画图初始化函数
    setupRealtimeDataDemo(ui->customPlot);
    ui->customPlot->replot();


    //port config
    global_port.setParity(QSerialPort::NoParity);
    global_port.setDataBits(QSerialPort::Data8);
    global_port.setStopBits(QSerialPort::OneStop);

    //connect
    //connect(信号发出者地址，发什么信号，在哪个类触发（地址），槽函数)  全是地址
    connect(ui->btn_open,&QPushButton::clicked,this,&NewWindow::btn_open_port);
    connect(ui->btn_close,&QPushButton::clicked,this,&NewWindow::btn_close_port);
    connect(ui->btn_send,&QPushButton::clicked,this,&NewWindow::btn_send_data);
    connect(&global_port,&QSerialPort::readyRead,this,&NewWindow::receive_data);
}
/*---------------------------------------------------
 *      slots
 * -------------------------------------------------*/
//实现槽函数
void NewWindow::btn_open_port(bool)
{
    /*-------------------port name-----------------------*/
    //qDebug()-->printf   /   cout
    qDebug()<<"OKl";
    //qDebug("OK");
    qDebug()<<ui->cmb_port_name->currentIndex();   //取到当前端口的值
    switch (ui->cmb_port_name->currentIndex()) {
    case 0:
        global_port.setPortName("COM1");
        break;
    default:
        global_port.setPortName("COM2");
        break;

    }

    /*------------baud rate-------------------------------------*/
    switch (ui->cmb_baud_rate->currentIndex()) {
    case 0:
        global_port.setBaudRate(QSerialPort::Baud115200);
        break;
    case 1:
        global_port.setBaudRate(QSerialPort::Baud57600);
        break;
    case 2:
        global_port.setBaudRate(QSerialPort::Baud38400);
        break;
    case 3:
        global_port.setBaudRate(QSerialPort::Baud19200);
        break;
    case 4:
        global_port.setBaudRate(QSerialPort::Baud9600);
        break;
    case 5:
        global_port.setBaudRate(QSerialPort::Baud4800);
        break;
    case 6:
        global_port.setBaudRate(QSerialPort::Baud2400);
        break;
    default:
        global_port.setBaudRate(QSerialPort::Baud1200);
        break;
    }

    // open
    global_port.open(QIODevice::ReadWrite);
    //ui->lab_status->setText("Connected");
    //test
    //global_port.write("1");

}

/*--------------close port--------------------------------*/
void NewWindow::btn_close_port(bool)
{
    //ui->lab_status->setText("Disconnected");
   global_port.close();

}
/*------------sending data----------------------*/


void NewWindow::btn_send_data(bool)
{
    QString data = ui->lint_send_data->text();
    QByteArray array = data.toLatin1(); // QString----->QByteArray
    global_port.write(array);
}

/*------------paint init---------------------------*/
//画图初始化
void NewWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
    customPlot->addGraph();  //addGraph() 添加一条曲线
    customPlot->graph(0)->setPen(QPen(Qt::red));
    //graph(x)->setPen(QPen(Qt::red));  x是曲线序号，添加的第一条是0，设置曲线颜色

    customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
    customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(2);
    customPlot->axisRect()->setupFullAxesBox();

    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(NewWindow::receive_data()));
    dataTimer.start(0); // Interval 0 means to refresh as fast as possible
    customPlot->legend->setVisible(true);
}


/*------------receive data---------------------------*/
void NewWindow::receive_data()
{
    QByteArray array = global_port.readAll();
    qDebug()<<array;
    bool ok;
//    n = array.toInt(&ok);

    //key的单位是秒
    for(n=0;n<5;n++)
    {
        double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
        ui->customPlot->graph(0)->addData(key, n);

        //删除8秒之前的数据。这里的8要和下面设置横坐标宽度的8配合起来
        //才能起到想要的效果，可以调整这两个值，观察显示的效果。
        ui->customPlot->graph(0)->removeDataBefore(key-8);

          //自动设定graph(1)曲线y轴的范围，如果不设定，有可能看不到图像
    //也可以用ui->customPlot->yAxis->setRange(up,low)手动设定y轴范围
        ui->customPlot->graph(0)->rescaleValueAxis();

        //这里的8，是指横坐标时间宽度为8秒，如果想要横坐标显示更多的时间
        //就把8调整为比较大到值，比如要显示60秒，那就改成60。
        //这时removeDataBefore(key-8)中的8也要改成60，否则曲线显示不完整。
        ui->customPlot->xAxis->setRange(key+0.25, 8, Qt::AlignRight);//设定x轴的范围
        ui->customPlot->replot();
    }

}
