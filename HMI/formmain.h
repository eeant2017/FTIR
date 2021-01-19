#ifndef FORMMAIN_H
#define FORMMAIN_H

#include <QWidget>
#include "form_syssett.h"
#include "form_log.h"
#include"form_inform.h"
#include "form_nowdate.h"
#include <QTimer>
namespace Ui {
class Formmain;
}

class Formmain : public QWidget
{
    Q_OBJECT

public:
    explicit Formmain(QWidget *parent = 0);
    ~Formmain();

    Form_Log *FormLog;
    Form_SysSett *FormSysSett;

     Form_Inform *FormInfrom;
     Form_NowDate *FormNowDate;

protected:
    //void timerEvent(QTimerEvent *);

private://
    Ui::Formmain *ui;
    QTimer *timer3;
   // void SystemSetSlot();

private slots:

    void SystemSetSlot();
    //void on_btn_SysSet_clicked();
    //void on_btn_SysSet_clicked(bool checked);
    //void on_pushButton_clicked();

    void  LogSlot();
    void  NowdateSlot();
    void InformSlot();
     void Timer3Out();
     void NowdateescSlot();

};

#endif //
