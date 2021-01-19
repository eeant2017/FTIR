#include "form_log.h"
#include "ui_form_log.h"
//#include "form_paraset.h"
 #include <QString>
#include "sqlite.h"



char * pAlarmString[]=
{
    "0",

    "报警码超过界限，码值为:",
};

char * pCheckMenuString[]=
{
    "标样值(ug/L)",

};

const char *pElem1[15] =
{
    "无",


};

Form_Log::Form_Log(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Log)
{
    ui->setupUi(this);

   //横向标签显示
   // ui->tableWidget->setColumnCount(2);
   ui->tableWidget->setColumnWidth(1,200);
   HeadString<<"TIME";
   ui->tableWidget->setHorizontalHeaderLabels(HeadString);



//   ui->le_StartYear->SetRange(2000,2099,INT_KEY);
//   ui->le_StartMonth->SetRange(1,12,INT_KEY);
//   ui->le_StartDay->SetRange(1,31,INT_KEY);
//   ui->le_EndYear->SetRange(2000,2099,INT_KEY);
//   ui->le_EndMonth->SetRange(1,12,INT_KEY);
//   ui->le_EndDay->SetRange(1,31,INT_KEY);


   connect(ui->btn_Search,SIGNAL(clicked()),this,SLOT(HistoryDataSearchSlot()));
   //connect(ui->pushButton,SIGNAL(clicked()),this,SLOT(EscButtonSlot()));
   connect(ui->tableWidget,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(EscButtonSlot()));



}

Form_Log::~Form_Log()
{
    delete ui;
}

void Form_Log::TabWidgetChangedSlot(int index)
{

    switch (index)
    {
        case 0:
        HistoryData_Page();
        break;
        case 1:
        AlarmLog_Page();
        break;
        default:
        break;
    }
}

void Form_Log::HistoryData_Page()
{

    m_SQLite.OpenDB(1);
}

void Form_Log::GetAndShowHistoryData( INT16U uin_Page)
{
    SYSTEM_TIME st_StartTime,st_EndTime;
    static QString StartDate;
    static QString EndDate;

    if (uin_Page == 1)
    {
       // StartDate = QString("%1-%2-%3 00:00").arg(ui->le_StartYear->text()).arg(ui->le_StartMonth->text()).arg(ui->le_StartDay->text());
       // EndDate = QString("%1-%2-%3 00:00").arg(ui->le_EndYear->text()).arg(ui->le_EndMonth->text()).arg(ui->le_EndDay->text());
    }
    QString sql = QString("SELECT * FROM HistoryData WHERE Time BETWEEN '%1' AND '%2' LIMIT 10 OFFSET %3").arg(StartDate).arg(EndDate).arg((uin_Page-1)*10);
    QSqlQuery query(m_SQLite.db);
    if(!query.exec(sql))
    {

        qDebug()<<query.lastError();
        m_SQLite.db.close();
        return;
    }
    else
    {

        query.last();
        int row = query.at()+1;
        query.first();
        QStringList HeadString;
        ui->tableWidget->clear();
        for(int i=0;i<row;i++)
        {
            HeadString << QString::number((uin_Page-1)*10+i+1);
            for (int j=0;j<1;j++)
            {
                ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
            }
            query.next();
        }
    //    ui->tw_HistoryData->setVerticalHeaderLabels(HeadString);

        if(row < 10)
        {
    //        ui->btn_LogNext->setVisible(false);
        }
        else
        {
     //       ui->btn_LogNext->setVisible(true);
        }
        if(uin_Page > 1)
        {
      //      ui->btn_LogLast->setVisible(true);
        }
        else
        {
     //       ui->btn_LogLast->setVisible(false);
        }
        return;
    }
}

void Form_Log::HistoryDataSearchSlot()
{
    uin_HistoryDataPage=1;
    GetAndShowHistoryData(uin_HistoryDataPage);
}
void Form_Log::HistoryDataLastSlot()
{
    uin_HistoryDataPage--;
    GetAndShowHistoryData(uin_HistoryDataPage);
}
void Form_Log::HistoryDataNextSlot()
{
    uin_HistoryDataPage++;
    GetAndShowHistoryData(uin_HistoryDataPage);
}

void Form_Log::GetAndShowAlarmLog(INT8U uin_Page)
{

    QString sql = QString("SELECT * FROM AlarmLog LIMIT 10 OFFSET %3").arg((uin_Page-1)*10);
    QSqlQuery query(m_SQLite.db);
    if(!query.exec(sql))
    {
        qDebug()<<query.lastError();
        return;
    }
    else
    {

        query.last();
        int row = query.at()+1;
        query.first();
        QStringList HeadString;
    //    ui->tw_AlarmLog->clear();
        for(int i=0;i<row;i++)
        {
            HeadString << QString::number((uin_Page-1)*10+i+1);

        //    ui->tw_AlarmLog->setItem(i,0,new QTableWidgetItem(query.value(0).toString()));
        //    ui->tw_AlarmLog->setItem(i,1,new QTableWidgetItem(query.value(1).toString()));
        //    ui->tw_AlarmLog->setItem(i,2,new QTableWidgetItem(pAlarmString[query.value(1).toInt()]));
            query.next();
        }
      //  ui->tw_AlarmLog->setVerticalHeaderLabels(HeadString);

        if(row < 10)
        {
      //      ui->btn_LogNext->setVisible(false);
        }
        else
        {
       //     ui->btn_LogNext->setVisible(true);
        }
        if(uin_Page > 1)
        {
     //       ui->btn_LogLast->setVisible(true);
        }
        else
        {
     //       ui->btn_LogLast->setVisible(false);
        }
        return;
    }
}
void Form_Log::AlarmLog_Page()
{
    INT8U auch_CurAlarm[20];
    QString CurAlarm ;

    uch_AlarmLogPage = 1;
    GetAndShowAlarmLog(uch_AlarmLogPage);

    //当前报警
    //if (ComBottom.GetCurAlarm(auch_CurAlarm))  //cfh
    if (0)
    {

        for (char i=0;i<20;i++)
        {
            if (auch_CurAlarm[i] != 0)
            {
                CurAlarm += QString::number(auch_CurAlarm[i])+"\n";
            }
        }
  //      ui->lb_CurAlarm->setText(CurAlarm);
    }

}

void Form_Log::AlarmLogLastSlot()
{
    uch_AlarmLogPage--;
    GetAndShowAlarmLog(uch_AlarmLogPage);

}
void Form_Log::AlarmLogNextSlot()
{
    uch_AlarmLogPage++;
    GetAndShowAlarmLog(uch_AlarmLogPage);
}

void Form_Log::EscButtonSlot()
{
    //m_SQLite.db.close();
    //delete this;
    close();

}

void Form_Log::LogNextSlot()
{
    INT8U index;
   // index = ui->tabWidget->currentIndex();

    switch (index)
    {
        case 0:
        HistoryDataNextSlot();
        break;
        case 1:
        AlarmLogNextSlot();
        break;
        default:
        break;
    }
}

void Form_Log::LogLastSlot()
{
    INT8U index;

  //  index = ui->tabWidget->currentIndex();

    switch (index)
    {
        case 0:
        HistoryDataLastSlot();
        break;
        case 1:
        AlarmLogLastSlot();
        break;
        default:
        break;
    }
}

