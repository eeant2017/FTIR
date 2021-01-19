#include "sqlite.h"
#include <QMutex>
#include "Mod/mod_mnt.h"


CSQlite m_SQLite;

CSQlite::CSQlite()
{
}


//==================================================================================================
//| 函数名称 |InitSQlite
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据库初始化
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::InitSQlite()
{

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../Config.db");//

    if(1)
    {
        if(!db.open())
        {
            qDebug()<<db.lastError();

            return false;
        }
        QSqlQuery query(db);
        qDebug("Connected!");



        if(!query.exec("select count(*) from sqlite_master where type = 'table' and name = 'VolCali' ")) //
        {
            qDebug()<<query.lastError();
            db.close();

            return false;
        }
        else
        {
            if(query.next())
            {
                qDebug("VolCali table num %d",query.value(0).toInt());
                if(query.value(0).toInt() == 0)
                {

                     //参数校准表
                    if (!query.exec("CREATE TABLE IF NOT EXISTS VolCali(ID INT PRIMARY KEY NOT NULL,f_Max REAL,"
                                    " f_Min REAL,f_default REAL,f_SetK REAL,f_SetB REAL,f_ReadK REAL,f_ReadB REAL)"))
                    {
                        qDebug()<<query.lastError();

                    }
                    else
                    {
                            STRCALIPARA st_CaliPara;

                            st_CaliPara.uch_ID = 0;
                            st_CaliPara.f_Max = 0;
                            st_CaliPara.f_Min = 0;
                            st_CaliPara.f_Default = 0;
                            st_CaliPara.f_SetK = 1;
                            st_CaliPara.f_SetB = 0;
                            st_CaliPara.f_ReadK = 1;
                            st_CaliPara.f_ReadB = 0;

                            for(int i=0;i<80;i++)
                            {
                                st_CaliPara.uch_ID = i;
                                m_SQLite.InsertCaliPara(st_CaliPara);

                            }


                       // qDebug("")
                    }

                }
                else
                {


                }
            }



        }

    }

    if(1)
    {
        if(!db.open())
        {
            qDebug()<<db.lastError();

            return false;
        }

        QSqlQuery query(db);
        //仪器ID
        if(!query.exec("select count(*) from sqlite_master where type = 'table' and name = 'deviceID' ")) //
        {
            qDebug()<<query.lastError();
            db.close();

            return false;
        }
        else
        {
            if(query.next())
            {
                qDebug("deviceID table num %d",query.value(0).toInt());
                if(query.value(0).toInt() == 0)
                {
                    if (!query.exec("CREATE TABLE IF NOT EXISTS deviceID(ID INT PRIMARY KEY NOT NULL,text TEXT)"))
                    {
                        qDebug()<<query.lastError();
                        qDebug("ERR to crete");

                    }
                    else //默认配置
                    {
                        qDebug("ok to crete");
                        char ID[12] = "XXXXXXXXXXX";
                        SetDeviceID(ID);
    //                    GetDeviceID(ID);


                    }
                }





             }
        }

    }



    //---------------------仪器型号 /仪器功能 /仪器组件 配置-----------------------------

    if(1)
    {

        if(!db.open())
        {
            qDebug()<<db.lastError();
            return false;
        }

        QSqlQuery query(db);


        if(!query.exec("select count(*) from sqlite_master where type = 'table' and name = 'deviceConfig' ")) //
        {
            qDebug()<<query.lastError();
            db.close();
            return false;
        }
        else
        {
            if(query.next())
            {
                qDebug("deviceConfig table num %d ",query.value(0).toInt());
                if(query.value(0).toInt() == 0)
                {
                    if (!query.exec("CREATE TABLE IF NOT EXISTS deviceConfig(ID INT PRIMARY KEY NOT NULL, data INT)"))
                    {
                        qDebug()<<query.lastError();
                        qDebug("ERR to crete");

                    }
                    else //默认配置 第一次
                    {
                        qDebug("deviceConfig First");

                        //init device config
                        SetDeviceModel(DEVICE_MODEL_EXPEC5210);
                        g_clMnt.m_uchDeviceModel = DEVICE_MODEL_EXPEC5210;
//                        INT8U type;
//                        GetDeviceModel(&type);
//                        qDebug("%d",type);
                        SetSampleSelect(SOURCE_ESI);
                        SetCIDVacuumGaugeSelect(UNIT_CIDVACUUMGAUGE_MKS);



                    }
                }
                else //对每个储存量进行验证，如果有问题就恢复为默认
                {
                    INT8U rtn;
                    INT8U type;
                    rtn = GetDeviceModel(&type);
                    if(rtn == false || type > 1)
                    {
                        qDebug("Device Model Init After Err");
                        SetDeviceModel(DEVICE_MODEL_EXPEC5210);
                    }


                    rtn = GetSampleSelect(&type);
                    if(rtn == false || type > 1)
                    {
                        qDebug("SampleSelect Init After Err");
                        SetSampleSelect(SOURCE_ESI);
                    }
                    rtn = GetCIDVacuumGaugeSelect(&type);
                    if(rtn == false || type > 1)
                    {
                        qDebug("CIDVacuumGaugeSelect Init After Err");
                        SetCIDVacuumGaugeSelect(UNIT_CIDVACUUMGAUGE_MKS);
                    }

                }


             }
        }
        if(g_clMnt.m_uchDeviceModel == DEVICE_MODEL_EXPEC5210)
        {
            qDebug("Device Model DEVICE_MODEL_EXPEC5210");
        }
        else
        {
            qDebug("Device Model DEVICE_MODEL_EXPEC5250");
        }

        if(g_clMnt.m_uchSourceSelect == SOURCE_ESI)
        {
            qDebug("SourceSelect SOURCE_ESI");
        }
        else
        {
            qDebug("SourceSelect SOURCE_EI");
        }

        if(g_clMnt.m_uchCIDVacuumSelect == UNIT_CIDVACUUMGAUGE_MKS)
        {
            qDebug("CIDVacuumSelect UNIT_CIDVACUUMGAUGE_MKS");
        }
        else
        {
            qDebug("CIDVacuumSelect UNIT_CIDVACUUMGAUGE_CIRCU");
        }




    }





    db.close();

    return true;
}

INT8U CSQlite::OpenDB(INT8U uch_Type)
{

    //todo 按仪表类型打开相应的数据库
    if(!m_SQLite.db.open())
    {
        qDebug()<<m_SQLite.db.lastError();

        return false;
    }

    return true;
}


//==================================================================================================
//| 函数名称 | InsertCaliPara
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 增加校准参数到数据库
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::InsertCaliPara(STRCALIPARA st_CaliPara)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();
        return false;

    }

    QString Sql = QString("INSERT INTO VolCali VALUES(%1,%2,%3,%4,%5,%6,%7,%8)").arg(st_CaliPara.uch_ID)
                    .arg(st_CaliPara.f_Max).arg(st_CaliPara.f_Min).arg(st_CaliPara.f_Default).arg(st_CaliPara.f_SetK)
                    .arg(st_CaliPara.f_SetB).arg(st_CaliPara.f_ReadK).arg(st_CaliPara.f_ReadB);
    QSqlQuery query(db);
    //insert data

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();

    return true;

}

//==================================================================================================
//| 函数名称 | UpdateCaliPara
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 更新数据库的校准系数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::UpdateCaliPara(STRCALIPARA st_CaliPara)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();

        return false;
    }

    QString Sql = QString("UPDATE VolCali SET f_Max = %1 ,"
                    " f_Max = %2 ,f_default = %3 ,f_SetK = %4,f_SetB = %5,f_ReadK = %6,f_ReadB = %7 WHERE ID = %8")   \
                    .arg(st_CaliPara.f_Max).arg(st_CaliPara.f_Min).arg(st_CaliPara.f_Default).arg(st_CaliPara.f_SetK) \
                    .arg(st_CaliPara.f_SetB).arg(st_CaliPara.f_ReadK).arg(st_CaliPara.f_ReadB).arg(st_CaliPara.uch_ID);
    QSqlQuery query(db);
    //insert data

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();

    return true;

}




INT8U CSQlite::GetCali()
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();

        return false;
    }

    QSqlQuery query(db);
    if(!query.exec("SELECT * FROM VolCali"))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        while(query.next())
        {
//            qDebug()<< query.value(0);
//            qDebug()<< query.value(1);
//            qDebug()<< query.value(2);
//            qDebug()<< query.value(3);
//            qDebug()<< query.value(4);
//            qDebug()<< query.value(5);
//            qDebug()<< query.value(6);
//            qDebug()<< query.value(7);
        }
        db.close();

        return true;
    }
}


INT8U CSQlite::GetCali(INT8U uch_ID,STRCALIPARA &st_CaliPara)
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();

        return false;
    }

    QSqlQuery query(db);

    QString Sql = QString("SELECT * FROM VolCali WHERE ID IN (%1)").arg(uch_ID);
    //qDebug("ID = %d",uch_ID);

    if(!query.exec(Sql))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        if(query.next())
        {
            st_CaliPara.uch_ID = query.value(0).toInt();
            st_CaliPara.f_Max =  query.value(1).toFloat();
            st_CaliPara.f_Min =  query.value(2).toFloat();
            st_CaliPara.f_Default =  query.value(3).toFloat();
            st_CaliPara.f_SetK =  query.value(4).toFloat();
            st_CaliPara.f_SetB =  query.value(5).toFloat();
            st_CaliPara.f_ReadK =  query.value(6).toFloat();
            st_CaliPara.f_ReadB =  query.value(7).toFloat();
            //qDebug("IDdadfad = %d",st_CaliPara.uch_ID);
        }
        db.close();

        return true;
    }
}



//==================================================================================================
//| 函数名称 | SetDeviceID
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置仪器ID
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::SetDeviceID(char * puchID)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();

        return false;
    }



    QString id = QString(QLatin1Literal(puchID));

    QString Sql = QString("REPLACE INTO deviceID VALUES(0,\"%1\")").arg(id);
    QSqlQuery query(db);
//    qDebug(Sql.toUtf8().data());
    //insert data

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();

    return true;

}

//==================================================================================================
//| 函数名称 | SetDeviceID
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 设置仪器ID
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::GetDeviceID(char* puchID)
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();
        return false;
    }

    QSqlQuery query(db);

    QString Sql = QString("SELECT * FROM deviceID WHERE ID == 0");
    //qDebug("ID = %d",uch_ID);

    if(!query.exec(Sql))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        if(query.next())
        {
            QByteArray id = query.value(1).toByteArray();
            char* p_id = id.data();
            memcpy(puchID,p_id,id.length());
        }
        db.close();

        return true;
    }
}

//==================================================================================================
//| 函数名称 | SetDeviceType
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 配置仪器型号
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::SetDeviceModel(INT8U uch_Type)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();

        return false;
    }

    QString Sql = QString("REPLACE INTO deviceConfig VALUES(0,%1)").arg(uch_Type);
    QSqlQuery query(db);
//    qDebug(Sql.toUtf8().data());

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();
    //g_clMnt.m_uchDeviceModel = uch_Type;

    return true;

}

//==================================================================================================
//| 函数名称 | GetDeviceType
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得仪器型号
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::GetDeviceModel(INT8U *puch_Type)
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();

        return false;
    }

    QSqlQuery query(db);

    QString Sql = QString("SELECT * FROM deviceConfig WHERE ID == 0");
    //qDebug("ID = %d",uch_ID);

    if(!query.exec(Sql))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        if(query.next())
        {
            *puch_Type = query.value(1).toInt();
//            qDebug("%d",*puch_Type);
        }
        db.close();
        g_clMnt.m_uchDeviceModel = *puch_Type;


        return true;
    }
}


//==================================================================================================
//| 函数名称 | SetSampleSelect
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 仪器进样选择
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U uch_Type 0 esi 进样 1 ei进样
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::SetSampleSelect(INT8U uch_Type)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();

        return false;
    }

    QString Sql = QString("REPLACE INTO deviceConfig VALUES(1,%1)").arg(uch_Type);
    QSqlQuery query(db);
//    qDebug(Sql.toUtf8().data());

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();
    g_clMnt.m_uchSourceSelect = uch_Type;

    return true;

}

//==================================================================================================
//| 函数名称 | GetSampleSelect
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得仪器进样选择
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U *puch_Type 指针，
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2020.04.02
//==================================================================================================
INT8U CSQlite::GetSampleSelect(INT8U *puch_Type)
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();

        return false;
    }

    QSqlQuery query(db);

    QString Sql = QString("SELECT * FROM deviceConfig WHERE ID == 1");
    //qDebug("ID = %d",uch_ID);

    if(!query.exec(Sql))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        if(query.next())
        {
            *puch_Type = query.value(1).toInt();
//            qDebug("%d",*puch_Type);
        }
        db.close();
        g_clMnt.m_uchSourceSelect = *puch_Type;

        return true;
    }
}


//==================================================================================================
//| 函数名称 | SetCIDVacuumGaugeSelect
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 配置池真空归选择
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U uch_Type 0 MKS925Com485 1 环境板的浦发真空规
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2019.12.12
//==================================================================================================
INT8U CSQlite::SetCIDVacuumGaugeSelect(INT8U uch_Type)
{

    if(!db.open())
    {
        qDebug()<<db.lastError();

        return false;
    }

    QString Sql = QString("REPLACE INTO deviceConfig VALUES(10,%1)").arg(uch_Type);
    QSqlQuery query(db);
//    qDebug(Sql.toUtf8().data());

    if(!query.exec(Sql))
    {
        qDebug()<<query.lastError();
    }

    db.close();
    g_clMnt.m_uchCIDVacuumSelect = uch_Type;

    return true;

}

//==================================================================================================
//| 函数名称 | GetSampleSelect
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 获得仪器进样选择
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | INT8U *puch_Type 指针，
//|----------|--------------------------------------------------------------------------------------
//| 返回参数 |
//|----------|--------------------------------------------------------------------------------------
//| 函数设计 | lyb 2020.04.02
//==================================================================================================
INT8U CSQlite::GetCIDVacuumGaugeSelect(INT8U *puch_Type)
{

    if(!db.open())
    {
        //qDebug()<<db.lastError();

        return false;
    }

    QSqlQuery query(db);

    QString Sql = QString("SELECT * FROM deviceConfig WHERE ID == 10");
    //qDebug("ID = %d",uch_ID);

    if(!query.exec(Sql))
    {
        //qDebug()<<query.lastError();
        db.close();

        return false;
    }
    else
    {
        if(query.next())
        {
            *puch_Type = query.value(1).toInt();
//            qDebug("%d",*puch_Type);
        }
        db.close();
        g_clMnt.m_uchCIDVacuumSelect = *puch_Type;

        return true;
    }
}


