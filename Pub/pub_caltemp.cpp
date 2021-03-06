#include "pub_caltemp.h"


//NTC-10KPX3-42H-S1热敏电阻的温度表
#define NTC_TABLE_LEN	32
const FP32 gar_NtcTable[NTC_TABLE_LEN][2]=
{
        {519910,-55},{379890,-50},{280700,-45},{209600,-40},{158090,-35},{120370,-30},{92484,-25},
        {71668,-20}, {55993,-15}, {44087,-10}, {34971,-5},  {27936,0},   {22468,5},   {18187,10},
        {14813,15},	 {12136,20},  {10000,25},  {8284,30},   {6899,35},   {5774,40},	  {4856,45},
        {4103,50},	 {3482,55},	  {2967,60},   {2539,65},   {2182,70},   {1882,75},   {1629,80},
        {1415,85},   {1234,90},   {1079,95},   {946.6,100},
};
const float RT_code[] =
{
        //0   1     2     3     4     5     6     7     8     9
        70.03,66.66,63.47,60.45,57.60,54.89,52.33,49.90,47.60,45.42,//-20
        43.35,41.38,39.52,37.75,36.07,34.48,32.96,31.62,30.16,28.85,//-10
        27.62,26.44,25.32,24.25,23.23,22.27,21.34,20.46,19.63,18.83,//0
        18.07,17.34,16.65,15.98,15.35,14.75,14.17,13.62,13.09,12.59,//10
        12.11,11.65,11.21,10.79,10.39,10.00,9.631,9.277,8.938,8.613,//20
        8.302,8.004,7.718,7.444,7.180,6.928,6.686,6.454,6.230,6.016,//30
        5.810,5.613,5.423,5.240,5.065,4.897,4.734,4.578,4.429,4.284,//40
        4.145,4.012,3.833,3.759,3.640,3.525,3.415,3.308,3.205,3.106,//50
        3.011,2.919,2.830,2.744,2.662,2.582,2.505,2.431,2.359,2.290,//60
        2.223,2.158,2.096,2.036,1.978,1.921,1.867,1.814,1.764,1.714 //70
};


CCalTemp::CCalTemp()
{

}


FP32 CalOnTemp(FP32 f_v)
{
    FP32 f_r;
    //	FP32 f_t;

    INT8U t1,t2,i;

    t1 = 0;
    t2 = 99;
    //获得电阻值
    f_r = (FP32)((f_v*10)/(2.5 - f_v));

    //温度查表算法，折中查表
    if((f_r >= 2.158) && (f_r <= 73.60))
    {
        while(t2 != (t1+1))
        {
            i = (INT8U)((t1+t2)>>1);

            if(RT_code[i] > f_r)
            {
                    t1 = i;
            }
            else
            {
                    t2 = i;
            }
        }

        return((FP32)t2-(f_r-RT_code[t2])/(RT_code[t1]-RT_code[t2])-20);
    }
    else
    {
        return(70.0);
    }
}
