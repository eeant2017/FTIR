#ifndef FORM_LOG_H
#define FORM_LOG_H

#include <QWidget>
//#include "../comm/com_bottom.h"
#include "../globalvar.h"
#include "Exter/softkeylineedit.h"
#include <QMessageBox>
#include <QDate>
#include "form_alarm.h"

namespace Ui {
    class Form_Log;
}

class Form_Log : public QWidget
{
    Q_OBJECT

public:
    explicit Form_Log(QWidget *parent = 0);
    ~Form_Log();

    //form_alarm *formalarm;

    void HistoryData_Page();
    void AlarmLog_Page();
    void PeakInfo_Page();
    void CheckLog_Page();
    void PMPCPeak_Page();
    void GetAndShowAlarmLog(INT8U uch_AlarmLogPage);
    void GetAndShowHistoryData( INT16U uin_PackNum);
    void GetAndShowPeakInfo(INT8U uch_PeakInfoPage);
    void GetAndShowCheckLog(INT8U uch_CheckLogPage);

    void AlarmLogLastSlot();
    void AlarmLogNextSlot();
    void HistoryDataLastSlot();
    void HistoryDataNextSlot();


   INT16U uin_HistoryDataPage;
    INT8U uch_AlarmLogPage;
   // ALARMLOG_PACK st_AlarmLogPack;
   // HistoryData_PACK st_HistoryDataPack;


    QStringList HeadString;

private:
    Ui::Form_Log *ui;

private slots:

    void TabWidgetChangedSlot(int);
    void EscButtonSlot();
    void LogLastSlot();
    void LogNextSlot();
    void HistoryDataSearchSlot();

};

#endif // FORM_LOG_H
