#include "newwindow.h"

#include <QApplication>

/*--------------------------------------------------------
 *
 * 2020年8月29日
 * 月
 * 参考博客：
 * https://blog.csdn.net/weiwen12138/article/details/74126886
 * http://www.youhaidong.cn/linux/2016/08/23/QCustomPlot%E5%AE%9E%E7%8E%B0%E5%AE%9E%E6%97%B6%E5%8A%A8%E6%80%81%E6%9B%B2%E7%BA%BF
 *
--------------------------------------------------------*/

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NewWindow w;
    w.show();
    return a.exec();
}
