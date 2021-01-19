#ifndef FORMNOWDATE_H
#define FORMNOWDATE_H

#include <QWidget>

#include <QTimer>
namespace Ui {
class Form_NowDate;
}
class Form_NowDate : public QWidget
{
    Q_OBJECT

public:
    explicit Form_NowDate(QWidget *parent = 0);
    ~Form_NowDate();

    /*Form_Log *FormLog;
    Form_SysSett *FormSysSett;

     Form_Inform *FormInfrom;*/

protected:
    //void timerEvent(QTimerEvent *);

private://
    Ui::Form_NowDate *ui;

   // void SystemSetSlot();


private slots:
   void EscButtonSlot();


};

#endif //
