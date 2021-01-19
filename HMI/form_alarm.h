#ifndef FORM_ALARM_H
#define FORM_ALARM_H

#include <QDialog>
//#include "softkeylineedit.h"
#include "../globalvar.h"

namespace Ui {
    class form_alarm;
}

class form_alarm : public QDialog
{
    Q_OBJECT

public:
    explicit form_alarm(QWidget *parent, const QString &title,const QString& text,char button);
    ~form_alarm();

    char result;

private:
    Ui::form_alarm *ui;

private slots:
    void OKButtonClickedSlot();
    void EscButtonClickedSlot();

};

#endif // FORM_ALARM_H
