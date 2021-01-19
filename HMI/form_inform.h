#ifndef FORM_INFORM_H
#define FORM_INFORM_H

#include <QWidget>
///#include "../comm/com_bottom.h"
#include "../globalvar.h"
#include "Exter/softkeylineedit.h"
#include <QMessageBox>
#include "form_alarm.h"
//#include "form_softupdate.h"

namespace Ui {
    class Form_Inform;
}

class Form_Inform : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Inform(QWidget *parent = 0);
    ~Form_Inform();
    form_alarm *formalarm;
    //Form_SoftUpdate *FormSoftUpdate;

    void SysInfo_Page();
    void SysVol_Page();

private:
    Ui::Form_Inform *ui;

private slots:
    void TabWidgetChangedSlot(int);
    void EscButtonSlot();
    void CalibrateButtonSlot();
    void SoftUpdateButtonSlot();
    void SoftUpdateButton2Slot();

};

#endif // FORM_INFORM_H
