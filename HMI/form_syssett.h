#ifndef FORM_SYSSETT_H
#define FORM_SYSSETT_H

#include <QDialog>

namespace Ui {
class Form_SysSett;
}

class Form_SysSett : public QDialog
{
    Q_OBJECT

public:
    explicit Form_SysSett(QWidget *parent = 0);
    ~Form_SysSett();

private:
    Ui::Form_SysSett *ui;
private slots:
    void OKButtonSlot();
    void EscButtonSlot();

    void on_pushButton_clicked();
};

#endif // FORM_SYSSETT_H
