/**************定义输出1K,高电平为30%的脉冲数组*****************/
//低电平存在双数位
//
/**************************************************************/
u16 my_PWM[]={
              //*****产生1K(  64000000/(31999+32000+1)  )的脉冲***************
              //31999,32000,31999,32000,31999,32000,
              //31999,32000,31999,32000,31999,32000,
              //31999,32000,31999,32000,31999,32000,
              //31999,32000,31999,32000,31999,32000,
              //31999,32000,31999,32000,31999,32000};
  
              //*****产生64K(  64000000/(499+500+1)  )的脉冲***************
              //499,500,499,500,499,500,
              //499,500,499,500,499,500,
              //499,500,499,500,499,500,
              //499,500,499,500,499,500,
              //499,500,499,500,499,500};
  
              //*****产生40K左右(  64000000/(499+1500+1)  )的脉冲***************
               200,1400,200,1400,400,1200,//40K
               400,1200,800,800,800,800,//40K
               200,1480,400,1480,800,880,//38.095K
               200,1320,400,1120,800,720,//42.105K
               60000,5000,40000,25500,30000,30000};
               
              //************************************************************
              //44712,19166,44714,19164,44716,19170,/*0-5的数据*/
              //44712,19166,44712,19164,44720,19162,/*6-11的数据*/
              //44716,19172,44710,19164,44714,19166,/*12-17的数据*/
              //44716,19164,44714,19162,44718,19168,/*18-23的数据*/
              //44718,19168,44716,19166,44716,19168,/*24-29的数据*/
              //44718,19166,44722,19166,44720,19170,/*30-35的数据*/
              //44718,19166,44714,19166,44716,19166,/*36-41的数据*/
              //44722,19172,44716,19170,44716,19170,/*42-47的数据*/
              //44718,19166,44716,19166,44718,19166,/*48-53的数据*/
              //*******************************************************
              //300,50000,600,50000,199,65000,
              //10000,10000,15000,15000,15000,15000,
              //30000,30000,30000,30000,20000,20000};//54-59
              /*0-5的数据*/
              /*0-5的数据*/
              /*0-5的数据*/
              /*0-5的数据*/
              /*0-5的数据*/
              /*0-5的数据*/

