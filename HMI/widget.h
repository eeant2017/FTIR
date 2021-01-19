#ifndef WIDGET_H
#define WIDGET_H
#include <QTime>
#include <QWidget>
//#include "form_syssett.h"
//#include "form_log.h"
#include"formmain.h"
namespace Ui {
class Widget;
}


class QTimer;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    // Form_Log *FormLog;
    //Form_SysSett *FormSysSett;
    Formmain *Formmain_sss;



protected:
    //void timerEvent(QTimerEvent *);

private://
    Ui::Widget *ui;
    QTimer *timer2;
   // void SystemSetSlot();

private slots:
    void SystemSetSlot();
     void Timer2Out();
    //void on_btn_SysSet_clicked();
    //void on_btn_SysSet_clicked(bool checked);
    //void on_pushButton_clicked();
    //void  LogSlot();
};

#endif // WIDGET_H
