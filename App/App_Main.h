#ifndef APP_MAIN_H
#define APP_MAIN_H

#include "typedefine.h"
#include <QThread>

class CMain : public QThread
{
public:
     CMain();
    ~CMain();
protected:
    void run();

};

#endif
