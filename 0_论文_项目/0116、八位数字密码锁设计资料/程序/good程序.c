#include<reg52.h>             
#include<math.h>                
#include<intrins.h>              
#define	uchar unsigned char      
#define	uint unsigned int
#define	 data_IO P2

sbit     rs=P3^0;	//ָ������ݼĴ��� ��1�����ݣ��� ��0��ָ�
sbit     rw=P3^1;//��д���ƣ���1���� ��0д��
sbit	  e=P3^2; //Ƭѡ�ź�
sbit      p=P2^7;
sbit   deng=P3^3;///�����ֵĶ˿�
sbit   md=P3^4;///�޸�����ʱ�ĵ�


uchar  pwflag;//�����Ƿ���ȷ�ı�־λ
uchar  count;//
uchar  gg=0 ;//ֻ��������ȷ����������޸�����
uchar  cs=3 ;//��������Ĵ������


uchar m[]={1,2,3,4,5,6}; ///��ʼ����
static uchar mbuf[6];///���������


 char d[32]={' ','I','M','P','O','R','T',' ','C','I','P','H','E','R',':',' ',
             ' ',' ','_',' ',' ','_','_','_','_','_','_',' ',' ',' ',' ',' '};//��������

code  char ks[32]={'C','O','N','G','R','A','T','U','L','A','T','E',' ','Y','O','U',
			  ' ','O','P','E','N','I','N','G',' ',' ','L','O','C','K','!',' '};////����

code char cw[32]={' ','C','I','P','H','E','R',' ',' ','E','R','R','O','R',' ',' ',
			      '~','~','~','~','~','~','~','~','~','~','~','~','~','~','~','~' };////����	  

code char xc[32]={' ',' ','A','M','E','N','D',' ','S','U','C','C','E','E','D',' ',
                  ' ',' ',' ',' ',' ','!','!','!','!','!','!',' ',' ',' ',' ',' '};//�޸ĳɹ�  

code char xg[32]={' ','A','M','E','N','D',' ',' ','C','I','P','H','E','R',':',' ',
                  ' ',' ',' ',' ',' ','_','_','_','_','_','_',' ',' ',' ',' ',' '};///�޸�����*/


void yanshi(uint n)///////////////��ʱ����///////////////////////////////////
{
 uint i;
  for(i=0;i<n;i++){};
}

void delay(uint n)///////////////��ʱ����///////////////////////////////////
{
 uint i,j;
  for(i=0;i<1000;i++)
   {for(j=0;j<n;j++){}}
   
}

void busy() ////////////////////�ж��Ƿ�æ////////////////////////////////////////
{data_IO=0xff;
 rs=0; //дָ��        ////////////////////
 rw=1; //��ʾ��״̬    // ������״̬��ʾ //
 e=1;  //�½���//      // ��æ״̬       //
 while(!p);         ////////////////////
// e=0;
} 
void xiezl(uchar shuju) /////////////////////*дָ��*/////////////////////////////
{
 busy();/*yanshi(6500);*/
 data_IO=shuju;
 rs=0; //дָ��        ////////////////////
 rw=0; //��ʾд״̬    // ������״̬��ʾ //
 e=1;  //�½���//      // дָ��         //
 e=0;  /////////       ////////////////////

}

void w_dat(uint n)//////////////////д����//////////////////////////
{
 busy(); /*yanshi(6500) ;*/
  data_IO=n;
   rs=1; //д����       ////////////////////
   rw=0; //��ʾд״̬    // ������״̬��ʾ //
   e=1;  //�½���//      //д����         //
   e=0;  /////////       ////////////////////
  // return;
}

void xianshi(uchar js[]) //////////////////��ʾ����///////////////////////////////
{
  uchar h,v;
  xiezl(0x01); //������ʾ
  yanshi(2000) ;
  xiezl(0x80); // DDRAM��ַ������
 for(h=0;h<16;h++)
   {w_dat(js[h]);  ///n��ʾ��д��λ����
   yanshi(100) ;
   }
   xiezl(0xc0);//����
 for(v=(16);v<32;v++)
   {w_dat(js[v]);  ///n��ʾ��д��λ����
   yanshi(100) ;
   }
}

void chushihua(void)///////////////////*Һ��ģ���ʼ��*/
 { uchar i;
   rs=0; //дָ��
   rw=0; //��ʾд״̬
   e=0;  //ʹ��
   for(i=0;i<3;i++)
   {
    xiezl(0x38);  //��������,8λ,һ����ʾ,5*7����
    yanshi(1000);
   }
    xiezl(0x38); //������ʾ
    xiezl(0x0f); //��ʾ���ؿ���λ
    xiezl(0x18); //��λ����,������ʾһ������  
    xiezl(0x06); //��������ģʽ
    xiezl(0x01); //������ʾ
    xiezl(0x80); // DDRAM��ַ������
    }

bit pwcmp(void)
{
	bit flag;
	uchar i;
	for (i=0;i<6;i++)
	{
		if (m[i]==mbuf[i])   flag = 1;   //*������ȷ*/
		else  {	flag = 0;  i=6; } //*�������*/
	}
	return(flag);
}

void gb(void)/////////ת�ƹ��
{  uchar c;
   xiezl(0xc0);//����
   yanshi(3000) ;
   for(c=0;c<5;c++)
   {w_dat(' ');///n��ʾ��д��λ����
    yanshi(2000) ;
   } 
}
uchar getkey(void)///�õ���ֵ
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


void diaoyong(uchar i)///////////���ø����ĺ���//////��������
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
          case 0xdb: if (count==6) { pwflag = pwcmp();}   else  pwflag = 0;// ֻ������6�������ȷ�ϼ���������Ƚ�
			           if(pwflag) {deng=0; xianshi(ks);gg=1; }   else { cs--;deng=1; xianshi(cw); delay(200); xianshi(d);gb();count = 0; }
			            break;  // 11011011		 a	   
          case 0xeb:  count = 0;deng=1; xianshi(d);  gb();gg=0; 	/*ȡ����*/
		               break;  // 11101011		 b	 

          case 0x77: if(gg==1){ count=0;xianshi(xg); gb();} //�޸�����
		               break;  // 01110111		 c
          
		  case 0xb7: if((count==6)&&(gg==1)) { count=0;
		  for (lq=0;lq<6;lq++){ m[lq]=mbuf[lq]; yanshi(10000);} md=1; xianshi(xc); delay(200); xianshi(d);yanshi(2000);gb();yanshi(200);gg=0;}//��������
		               break;  // 10110111		 d
          /*case 0xd7: sr[jm]='e'; break;  // 11010111		 e
            case 0xe7: sr[jm]='f'; break;  // 11100111		 f	*/

          default: break; 
          }
       
}


void main(void)
{uchar key;
chushihua();
xianshi(d);//��ʾ����
gb(); //�ƹ�굽�ڶ��е���λ
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