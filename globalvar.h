#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include <qstring.h>
#include "typedefine.h"


#define THREAD_MAX_SOURCE 20





typedef struct
{

   INT8U uch_Flag;  //标志
   INT8U uch_Used;   //是否使用
   INT8U  uch_ID;    //编号
   void *p_this;

}STTHREAD,*STTHREADPTR;




//函数类型(通信结构中使用)类似钩子函数
typedef INT8U (* UCH_PFUN_UCH)(INT8U);

typedef INT8U (* UCH_PFUN_PUCH)(INT8U *);

typedef FP32 (* FP32_PFUN_VOID)(void);

typedef void(* VOID_PFUN_VOID)(void);


extern STTHREAD stThread[THREAD_MAX_SOURCE];
extern INT8U GetFreethreadID(void);
extern INT8U CreateSysThread(void);

#endif // GLOBALVAR_H
