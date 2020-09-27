#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h"
#include <QTimer>
/*--------------User---------------------*/
//port
#include <QSerialPort>
//debug
#include <QDebug>
/*--------------User---------------------*/

QT_BEGIN_NAMESPACE
namespace Ui { class NewWindow; }
QT_END_NAMESPACE

class NewWindow : public QMainWindow
{
    Q_OBJECT

public:
    NewWindow(QWidget *parent = nullptr);
    ~NewWindow();
    //设置qcustomplot画图属性，实时
    void setupRealtimeDataDemo(QCustomPlot *customPlot);
    int n;


//定义槽函数
private slots:
    //button
    void btn_open_port(bool);
    void btn_close_port(bool);
    void btn_send_data(bool);

    //received data
    void receive_data();

//    //添加实时数据槽
//    void realtimeDataSlot();

private:
    Ui::NewWindow *ui;
    /*---------------function-----------------*/
    void system_init();
    /*---------------variable-----------------*/
    QSerialPort global_port;  //直接定义

    //定时器，周期调用realtimeDataSlot()槽，实现动态数据添加到曲线
    QTimer dataTimer;
};
#endif // NEWWINDOW_H
