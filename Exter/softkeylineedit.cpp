#include "softkeylineedit.h"

SoftKeyLineEdit::SoftKeyLineEdit(QWidget *parent) :
    QLineEdit(parent)
{
    sin_Min = -99999;
    sin_Max =  99999;

}

void SoftKeyLineEdit::mousePressEvent(QMouseEvent *e)
{
    numkeyboard = new NumKeyboard(this,sin_Min,sin_Max,f_Min,f_Max,uch_InputType);

    if(e->button() == Qt::LeftButton)
    {
        //numkeyboard->setText(this->text());
        numkeyboard->exec();
        if(numkeyboard->valid)
        {
            this->setText(numkeyboard->getText());
            delete numkeyboard;
        }
    }
}

void SoftKeyLineEdit::SetRange(signed int min,signed int max,char InPutType)
{
    sin_Min = min;
    sin_Max = max;
    uch_InputType = InPutType;
}
void SoftKeyLineEdit::SetRange(float min,float max)
{
    f_Min = min;
    f_Max = max;
    uch_InputType = FLOAT_KEY;
}
void SoftKeyLineEdit::SetPSDMode()
{
    uch_InputType = PASSWORD_KEY;
}
