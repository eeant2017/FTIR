#ifndef NUMKEYBOARD_H
#define NUMKEYBOARD_H

#include <QDialog>
#include <QtDebug>
#include <QKeyEvent>
#include <QEvent>
//#include <QMessageBox>
#include "../globalvar.h"
//#include "form_alarm.h"

namespace Ui {
class NumKeyboard;
}

class NumKeyboard : public QDialog
{
    Q_OBJECT
    
public:
    //form_alarm *formalarm;

    //explicit NumKeyboard(QWidget *parent = 0,signed int sin_Min = -99999,int sin_Max= 99999,float f_Min =-99999,float f_Max=99999,char uch_Type=FLOAT_KEY);
    ~NumKeyboard();
    bool valid;


    void setText(QString str);
    QString getText();

protected:
    void changeEvent(QEvent *e);
//    void keyPressEvent(QKeyEvent *e);
    bool eventFilter(QObject *obj, QEvent *event);
        
private slots:
    void on_num1Button_clicked();
    void on_num2Button_clicked();
    void on_num3Button_clicked();
    void on_num4Button_clicked();
    void on_num5Button_clicked();
    void on_num6Button_clicked();
    void on_num7Button_clicked();
    void on_num8Button_clicked();
    void on_num9Button_clicked();
    void on_dotButton_clicked();
    void on_num0Button_clicked();
    void on_signButton_clicked();
    void on_leftButton_clicked();
    //void on_rightButton_clicked();
    void on_backspaceButton_clicked();
    void on_cancelButton_clicked();
    void on_okButton_clicked();

private:
    signed int sin_Min,sin_Max;
    float f_Min,f_Max;
    char KeyType;
    Ui::NumKeyboard *ui;
    QString strContent;
};

#endif // NUMKEYBOARD_H
