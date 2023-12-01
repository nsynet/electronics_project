#include<reg51.h>
#define uchar unsigned char

sbit dp=P1^4;
sbit sh=P1^5;

void delay(unsigned int t);
uchar Getkey(void);
void Disp(uchar n);

uchar Disptab[10]={0x11,0x7d,0x23,0x29,0x4d,0x89,0x81,0x3d,0x1,0x9};

main()
{
    uchar key;
    char Vol1=0x00,Vol2=0x00;
    Disp(Disptab[Vol2]);
	Disp(Disptab[Vol1]-1);
	P0=0x00;
    while(1)
	{
	    key=Getkey();
		switch(key)
		{
		    case 0x01:break;
			case 0x02:break;
			case 0x04:if(Vol1==0&&Vol2==0)break;
			          Vol2--;
			          if(Vol2<0)
                      {
                          Vol2=9;Vol1--;
                      }
                      Disp(Disptab[Vol2]);
		              Disp(Disptab[Vol1]-1);
		              P0=4.3*(Vol1*10+Vol2);
			          break;
			case 0x08:if(Vol1==9&&Vol2==9)break;
			          Vol2++;
			          if(Vol2==10)
			          {
			             Vol2=0x00;Vol1++;
                      }
                      Disp(Disptab[Vol2]);
		              Disp(Disptab[Vol1]-1);
		              P0=4.3*(Vol1*10+Vol2);
                      break;
			default:break;
		}
	}
}

uchar Getkey(void)
{
    uchar key;
    P1|=0x0f;
	if(!(~(P1|0xf0))) return 0x00;
	delay(0x20);
    if(!(~(P1|0xf0))) return 0x00;
	key=~(P1|0xf0);
	while(~(P1|0xf0));
	return key;
}

void delay(unsigned int t)
{
    while(--t);
}

void Disp(uchar n)
{
    uchar i;
    for(i=0;i<8;i++){
        sh=0;
        if(0x80&n){
            dp=1;
        }
        else{
            dp=0;
        }
        sh=1;        
        n=(n<<1)+1;
    }
}
