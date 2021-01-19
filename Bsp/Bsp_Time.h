#ifndef BSP_TIME_H
#define BSP_TIME_H

#include <QDialog>
#include "Bsp_Include.h"

class QTimer;

class CTimer : public QObject
{
    Q_OBJECT

public:
    CTimer(QObject *parent = NULL);
    ~CTimer();
signals:
public slots:
  void Timer1Out();
  void Timer2Out();
private:

  QTimer *timer1;
  QTimer *timer2;

};

#endif // BSP_TIME_H
