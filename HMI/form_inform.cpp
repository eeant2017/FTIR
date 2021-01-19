#include "form_inform.h"
#include "ui_form_inform.h"

Form_Inform::Form_Inform(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_Inform)
{
    ui->setupUi(this);

    connect(ui->tabWidget,SIGNAL( currentChanged(int) ),this,SLOT(TabWidgetChangedSlot(int)));
    connect(ui->btn_Calibrate,SIGNAL(clicked()),this,SLOT(CalibrateButtonSlot()));
    connect(ui->btn_Esc1,SIGNAL(clicked()),this,SLOT(EscButtonSlot()));
   // connect(ui->btn_SoftUpdate,SIGNAL(clicked()),this,SLOT(SoftUpdateButtonSlot()));
    //connect(ui->btn_SoftUpdate_2,SIGNAL(clicked()),this,SLOT(SoftUpdateButton2Slot()));

    if (m_GlobalVar.uch_CurUser == USER)
    {
        //ui->tabWidget->removeTab(2);
    }

    ui->tabWidget->setCurrentIndex(0);
    SysInfo_Page();
}

Form_Inform::~Form_Inform()
{
    delete ui;
}

void Form_Inform::EscButtonSlot()
{
    delete this;

}
void Form_Inform::TabWidgetChangedSlot(int index)
{
    switch (index)
    {
        case 0:
        SysInfo_Page();
        break;
        case 1:
        SysVol_Page();
        break;
        default:
        break;
    }
}

void Form_Inform::SysInfo_Page()
{
    INT8U auch_MN[14],auch_SoftVersion[82];
    auch_SoftVersion[80] = '\0';
    auch_MN[12] = '\0';
    QString qstr;

   /* if (ComBottom.GetMachineMN(auch_MN)&&
        ComBottom.GetSoftsVersion(auch_SoftVersion))
    {
        qstr = QString::fromStdString((char*)auch_MN);
        ui->lb_MachineSN->setText(qstr);

        qstr = QString::fromStdString((char*)auch_SoftVersion);
        ui->lb_MBSoftVersion->setText(qstr);

        qstr = QString::fromStdString((char*)&auch_SoftVersion[32]);
        ui->lb_CHEVersion->setText(qstr);

        qstr = QString::fromStdString((char*)&auch_SoftVersion[64]);
        ui->lb_FlowVersion->setText(qstr);
    }
    else
    {
        formalarm = new form_alarm(this,"提示","通信异常",ICON_ERR);
        formalarm->exec();
        delete formalarm;
    }*/

}

void Form_Inform::SysVol_Page()
{
    SYSVOL_PARA st_SysVol;

    /*if (ComBottom.GetSysVolPara(&st_SysVol))
    {
        ui->lb_5VD->setText(QString::number(st_SysVol.f_5VD));
        ui->lb_5VA->setText(QString::number(st_SysVol.f_5VA));
        ui->lb_5VDIS->setText(QString::number(st_SysVol.f_5VDis));
        ui->lb_5VE->setText(QString::number(st_SysVol.f_5VE));
        ui->lb_24V->setText(QString::number(st_SysVol.f_24V));
        ui->lb_RefV->setText(QString::number(st_SysVol.f_Ref));
    }
    else
    {
        formalarm = new form_alarm(this,"提示","通信异常",ICON_ERR);
        formalarm->exec();
        delete formalarm;
    }*/
}
void Form_Inform::SoftUpdateButtonSlot()
{
   // FormSoftUpdate = new Form_SoftUpdate(this,1);
    //FormSoftUpdate->exec();
    //delete FormSoftUpdate;

}
void Form_Inform::SoftUpdateButton2Slot()
{
   // FormSoftUpdate = new Form_SoftUpdate(this,2);
   // FormSoftUpdate->exec();
   // delete FormSoftUpdate;

}


void Form_Inform::CalibrateButtonSlot()
{
    /*QProcess Process;

    char cmd[64];
    sprintf(cmd,"/usr/local/tslib/bin/ts_calibrate");
    system(cmd);

    Process.start(QString("/data/rc.sh"));
    Process.waitForStarted();                              //超时30s
    exit(0);*/

}


