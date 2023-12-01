#include<reg52.h>             
#include<math.h>                
#include<intrins.h>              
#define	uchar unsigned char      
#define	uint unsigned int
#define	 data_IO P2

sbit     rs=P3^0;	//指令和数据寄存器 “1是数据，” “0是指令”
sbit     rw=P3^1;//读写控制，“1读” “0写”
sbit	  e=P3^2; //片选信号
sbit      p=P2^7;
sbit   deng=P3^3;///响音乐的端口
sbit   md=P3^4;///修改密码时的灯


uchar  pwflag;//密码是否正确的标志位
uchar  count;//
uchar  gg=0 ;//只有输入正确的密码才能修改密码
uchar  cs=3 ;//输入密码的错误次数


uchar m[]={1,2,3,4,5,6}; ///初始密码
static uchar mbuf[6];///输入的密码


 char d[32]={' ','I','M','P','O','R','T',' ','C','I','P','H','E','R',':',' ',
             ' ',' ','_',' ',' ','_','_','_','_','_','_',' ',' ',' ',' ',' '};//输入密码

code  char ks[32]={'C','O','N','G','R','A','T','U','L','A','T','E',' ','Y','O','U',
			  ' ','O','P','E','N','I','N','G',' ',' ','L','O','C','K','!',' '};////开锁

code char cw[32]={' ','C','I','P','H','E','R',' ',' ','E','R','R','O','R',' ',' ',
			      '~','~','~','~','~','~','~','~','~','~','~','~','~','~','~','~' };////错误	  

code char xc[32]={' ',' ','A','M','E','N','D',' ','S','U','C','C','E','E','D',' ',
                  ' ',' ',' ',' ',' ','!','!','!','!','!','!',' ',' ',' ',' ',' '};//修改成功  

code char xg[32]={' ','A','M','E','N','D',' ',' ','C','I','P','H','E','R',':',' ',
                  ' ',' ',' ',' ',' ','_','_','_','_','_','_',' ',' ',' ',' ',' '};///修改密码*/


void yanshi(uint n)///////////////延时函数///////////////////////////////////
{
 uint i;
  for(i=0;i<n;i++){};
}

void delay(uint n)///////////////延时函数///////////////////////////////////
{
 uint i,j;
  for(i=0;i<1000;i++)
   {for(j=0;j<n;j++){}}
   
}

void busy() ////////////////////判断是否忙////////////////////////////////////////
{data_IO=0xff;
 rs=0; //写指令        ////////////////////
 rw=1; //表示读状态    // 三个的状态表示 //
 e=1;  //下降延//      // 判忙状态       //
 while(!p);         ////////////////////
// e=0;
} 
void xiezl(uchar shuju) /////////////////////*写指令*/////////////////////////////
{
 busy();/*yanshi(6500);*/
 data_IO=shuju;
 rs=0; //写指令        ////////////////////
 rw=0; //表示写状态    // 三个的状态表示 //
 e=1;  //下降延//      // 写指令         //
 e=0;  /////////       ////////////////////

}

void w_dat(uint n)//////////////////写数据//////////////////////////
{
 busy(); /*yanshi(6500) ;*/
  data_IO=n;
   rs=1; //写数据       ////////////////////
   rw=0; //表示写状态    // 三个的状态表示 //
   e=1;  //下降延//      //写数据         //
   e=0;  /////////       ////////////////////
  // return;
}

void xianshi(uchar js[]) //////////////////显示函数///////////////////////////////
{
  uchar h,v;
  xiezl(0x01); //清屏显示
  yanshi(2000) ;
  xiezl(0x80); // DDRAM地址的设置
 for(h=0;h<16;h++)
   {w_dat(js[h]);  ///n表示有写几位数据
   yanshi(100) ;
   }
   xiezl(0xc0);//换行
 for(v=(16);v<32;v++)
   {w_dat(js[v]);  ///n表示有写几位数据
   yanshi(100) ;
   }
}

void chushihua(void)///////////////////*液晶模块初始化*/
 { uchar i;
   rs=0; //写指令
   rw=0; //表示写状态
   e=0;  //使能
   for(i=0;i<3;i++)
   {
    xiezl(0x38);  //功能设置,8位,一行显示,5*7点阵
    yanshi(1000);
   }
    xiezl(0x38); //两行显示
    xiezl(0x0f); //显示开关控制位
    xiezl(0x18); //移位控制,光标和显示一起左移  
    xiezl(0x06); //设置输入模式
    xiezl(0x01); //清屏显示
    xiezl(0x80); // DDRAM地址的设置
    }

bit pwcmp(void)
{
	bit flag;
	uchar i;
	for (i=0;i<6;i++)
	{
		if (m[i]==mbuf[i])   flag = 1;   //*密码正确*/
		else  {	flag = 0;  i=6; } //*密码错误*/
	}
	return(flag);
}

void gb(void)/////////转移光标
{  uchar c;
   xiezl(0xc0);//换行
   yanshi(3000) ;
   for(c=0;c<5;c++)
   {w_dat(' ');///n表示有写几位数据
    yanshi(2000) ;
   } 
}
uchar getkey(void)///得到键值
{ uchar h,l,k;
  while(P1!=0x0f)
  {
   h=P1&0x0f;
   P1=0xf0;
   l=P1&0xf0;
   k=h|l;
   return(k);
  }
}


void diaoyong(uchar i)///////////调用各键的函数//////输入密码
{  uchar lq;
         switch(i)
	    {
          case 0x7e: if (count<6) { mbuf[count]=1; count++;  w_dat('*');}  break;  // 01111110       1
          case 0xbe:  if (count<6) { mbuf[count]=2; count++;  w_dat('*');}  break;  // 10111110		 2
          case 0xde: if (count<6) { mbuf[count]=3; count++;  w_dat('*');}  break;  // 11011110		 3
          case 0xee:  if (count<6) { mbuf[count]=4; count++;  w_dat('*');}  break;  // 11101110		 4

          case 0x7d:  if (count<6) { mbuf[count]=5; count++;  w_dat('*');}  break;  // 01111101		 5
          case 0xbd:  if (count<6) { mbuf[count]=6; count++;  w_dat('*');} break; // 10111101		 6
          case 0xdd: if (count<6) { mbuf[count]=7; count++;  w_dat('*');}  break;  // 11011101		 7
          case 0xed: if (count<6) { mbuf[count]=8; count++;  w_dat('*');} break;  // 11101101		 8

          case 0x7b:  if (count<6) { mbuf[count]=9; count++;  w_dat('*');}  break;  // 01111011		 9
          case 0xbb:  if (count<6) { mbuf[count]=0; count++;  w_dat('*');} break;  // 10111011		 0
          case 0xdb: if (count==6) { pwflag = pwcmp();}   else  pwflag = 0;// 只有输入6个密码后按确认键才作密码比较
			           if(pwflag) {deng=0; xianshi(ks);gg=1; }   else { cs--;deng=1; xianshi(cw); delay(200); xianshi(d);gb();count = 0; }
			            break;  // 11011011		 a	   
          case 0xeb:  count = 0;deng=1; xianshi(d);  gb();gg=0; 	/*取消键*/
		               break;  // 11101011		 b	 

          case 0x77: if(gg==1){ count=0;xianshi(xg); gb();} //修该密码
		               break;  // 01110111		 c
          
		  case 0xb7: if((count==6)&&(gg==1)) { count=0;
		  for (lq=0;lq<6;lq++){ m[lq]=mbuf[lq]; yanshi(10000);} md=1; xianshi(xc); delay(200); xianshi(d);yanshi(2000);gb();yanshi(200);gg=0;}//保存密码
		               break;  // 10110111		 d
          /*case 0xd7: sr[jm]='e'; break;  // 11010111		 e
            case 0xe7: sr[jm]='f'; break;  // 11100111		 f	*/

          default: break; 
          }
       
}


void main(void)
{uchar key;
chushihua();
xianshi(d);//显示函数
gb(); //移光标到第二行第六位
while(cs)
{
  P1=0x0f;
  if(P1!=0x0f)  
 {
   yanshi(20); 
   key=getkey(); 
	md=0;
   diaoyong(key); 
   yanshi(20000);
   md=1;
}
}while(1);
}