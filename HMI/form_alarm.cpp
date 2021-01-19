#include "form_alarm.h"
#include "ui_form_alarm.h"

form_alarm::form_alarm(QWidget *parent, const QString &title,const QString& text,char type) :
    QDialog(parent),
    ui(new Ui::form_alarm)
{
    ui->setupUi(this);
    ui->lb_Title->setAttribute(Qt::WA_TranslucentBackground);
    ui->lb_Text->setAttribute(Qt::WA_TranslucentBackground);

    connect(ui->btn_esc,SIGNAL(clicked()),this,SLOT(EscButtonClickedSlot()) );
    connect(ui->btn_set,SIGNAL(clicked()),this,SLOT(OKButtonClickedSlot()) );

    QString IconStyle;

    if ( ICON_ANSER == type)
    {
        IconStyle = "background-image: url(:/Images/alarmconform.png)";
    }
    else if ( ICON_OK == type)
    {
        IconStyle = "background-image: url(:/Images/alarmok.png)";
    }
    else
    {
        IconStyle = "background-image: url(:/Images/alarmerr.png)";
    }
    ui->lb_Icon->setStyleSheet(IconStyle);

    ui->lb_Title->setText(title);
    ui->lb_Text->setText(text);
}

form_alarm::~form_alarm()
{
    delete ui;
}

void form_alarm::OKButtonClickedSlot()
{
    this->close();
    result = true;
}

void form_alarm::EscButtonClickedSlot()
{
    this->close();
    result = false;
}
