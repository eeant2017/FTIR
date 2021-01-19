#ifndef SOFTKEYLINEEDIT_H
#define SOFTKEYLINEEDIT_H

#include "Exter/softkeylineedit.h"
#include "HMI/numkeyboard.h"
#include <QLineEdit>
#include <QMouseEvent>


class SoftKeyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SoftKeyLineEdit(QWidget *parent = 0);
    void SetRange(signed int sin_min,signed int sin_max,char uch_InPutType);
    void SetRange(float min,float max);
    void SetPSDMode();


protected:
    void mousePressEvent(QMouseEvent *e);

private:
    signed int sin_Min,sin_Max;
    float f_Min,f_Max;
    char uch_InputType;
    NumKeyboard *numkeyboard;
    
signals:
    
public slots:
    
};

#endif // SOFTKEYLINEEDIT_H
