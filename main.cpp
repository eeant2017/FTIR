//#include "Bsp/Bsp_Time.h"
#include"Include.h"
#include <QGraphicsView>
#include <QGraphicsProxyWidget>
#include <QTextCodec>
#include <App/app_app.h>
#include <Bsp/Bsp_Time.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CTimer cl_Timer; //init timer 给Pub_Tmr提供计数
    SystemInit();
    qDebug("SystemInit.\n");
    cl_AppCom.start();
    cl_App.start();
    //cl_AppCom.wait();
    //cl_App.wait();

    return a.exec();
}
