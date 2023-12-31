#define EN  3
#define DI  4
#define CSA 5
#define CSB 6
#define RW  7
unsigned char hzzhi[]={};//��
unsigned char hznen[]={};//��
unsigned char hzxin[]={};//��
unsigned char hzchong[]={};//��
unsigned char hzdian[]={};//��
unsigned char hzqi[]={};//��
unsigned char hzchi[]={};//��
unsigned char hzzhuang[]={};//״
unsigned char hztai[]={};//̬
unsigned char hzya[]={};//ѹ
unsigned char hzliu[]={};//��
unsigned char hzwen[]={};//��
unsigned char hzdu[]={};//��
unsigned char hzman[]={};//��
unsigned char hzjuan[]={};//�
unsigned char hzkuai[]={};//��
unsigned char hzfang[]={};//��
unsigned char hznie[]={};//��
unsigned char hzge[]={};//��
unsigned char hzqing[]={};//��
unsigned char hzli[]={};//�

unsigned char DATA;
//PORTB-->data
//PA3->EN;PA4->DI;PA5->CSA;PA6->CSB;PA7->RW;

//дLCDָ��
void WriteCom(unsigned char area,unsigned char com)
{
  //area=0-->left;1-->mid;2-->right
	//area=[csa,csb]00:L 01:M 10:R
  unsigned char lcd_busy;
	switch(area)
	{
	  case 0:
		  PORTA &=~ (1<<CSA);
			PORTA &=~ (1<<CSB);
		  break;
		case 1:
		  PORTA &=~ (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
		case 2:
		  PORTA |=  (1<<CSA);
			PORTA &=~ (1<<CSB);
			break;
		default:
		  PORTA |=  (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
	}
	PORTA &=~ (1<<DI);
	PORTA |=  (1<<RW);
	PORTC &=~ (1<<PC7);//EN 245
	do{
	  PORTB=0xFF;
	  PORTA |=  (1<<EN);
	  PORTC |=  (1<<PC6);//LET 245 A=B;
	  lcd_busy=PINB&0x80;
	  PORTA &=~ (1<<EN);
	}while(lcd_busy==1);
	PORTC &=~ (1<<PC6);
	PORTB  =  com;
	PORTA &=~ (1<<RW);
	PORTA |=  (1<<EN);
	PORTA &=~ (1<<EN);
}

//дLCD����
void WriteDat(unsigned char area,unsigned char dat)
{
  //area=0-->left;1-->mid;2-->right
	//area=[csa,csb]00:L 01:M 10:R
  unsigned char lcd_busy;
	switch(area)
	{
	  case 0:
		  PORTA &=~ (1<<CSA);
			PORTA &=~ (1<<CSB);
		  break;
		case 1:
		  PORTA &=~ (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
		case 2:
		  PORTA |=  (1<<CSA);
			PORTA &=~ (1<<CSB);
			break;
		default:
		  PORTA |=  (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
	}
	PORTA &=~ (1<<DI);//DI=0
	PORTA |=  (1<<RW);//RW=1
	PORTC &=~ (1<<PC7);//EN 245
	do{
	  PORTB=0xFF;
	  PORTA |=  (1<<EN);//EN=1
	  PORTC |=  (1<<PC6);//LET 245 A=B;
	  lcd_busy=PINB&0x80;
	  PORTA &=~ (1<<EN);
	}while(lcd_busy==1);
	PORTC &=~ (1<<PC6);
	PORTB  =  dat;
	PORTA |=  (1<<DI);
	PORTA &=~ (1<<RW);
	PORTA |=  (1<<EN);
	PORTA &=~ (1<<EN);
}

//��LCD����
unsigned char ReadDat(unsigned char area,unsigned char dat)
{
  //area=0-->left;1-->mid;2-->right
	//area=[csa,csb]00:L 01:M 10:R
  unsigned char lcd_busy;
	switch(area)
	{
	  case 0:
		  PORTA &=~ (1<<CSA);
			PORTA &=~ (1<<CSB);
		  break;
		case 1:
		  PORTA &=~ (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
		case 2:
		  PORTA |=  (1<<CSA);
			PORTA &=~ (1<<CSB);
			break;
		default:
		  PORTA |=  (1<<CSA);
			PORTA |=  (1<<CSB);
			break;
	}
	PORTA &=~ (1<<DI);
	PORTA |=  (1<<RW);
	PORTC &=~ (1<<PC7);//EN 245
	do{
	  PORTB=0xFF;
	  PORTA |=  (1<<EN);
	  PORTC |=  (1<<PC6);//LET 245 A=B;
	  lcd_busy=PINB&0x80;
	  PORTA &=~ (1<<EN);
	}while(lcd_busy==1);
	PORTA |=  (1<<DI);
	PORTB  =0xFF;
	PORTA |=  (1<<EN);
	PORTC &=~ (1<<PC6);
	dat    =   PINB;
	PORTA &=~ (1<<EN);
	return(dat);
}

//LCD��ʼ��
void lcd_ini(void)
{
  unsigned char i;
  for (i=0;i<3;i++)
  {
    WriteCom(i,0xC0);
    WriteCom(i,0x3F);
  }
}

//���LCD��
void lcd_clr()
{
  int r4,r3,a,area;
  int wData=0;
  r4=0;
	do{
	  a=r4;
		a=a|0xb8;
		for(area=0;area<3;area++)
		  WriteCom(area,a);
		for(area=0;area<3;area++)
			WriteCom(area,0x40);
		r3=0x40;
		do{
		  for(area=0;area<3;area++)
			  WriteDat(area,wData);
			r3--;
		}while(r3!=0);
		r4++;
	}while(r4!=8);
}

void dis(int lr,int x,int y,int data)
{
	lcd_ini();
	WriteCom(lr,x);
	WriteCom(lr,y);
	WriteCom(lr,data);
}

int trans_data(int a,int b)
{
  int i;
	if(b==0)	return 1;
	else
	{
	for (i=0;i<b-1;i++)
		a=a*2;
	return a;
	}
}
void writehz(unsigned char lr,unsigned char x,unsigned char y,unsigned char *s)
{
  //column-->y,code
	//page-->x
	unsigned char count;
	unsigned char temp;
	unsigned char i;
	count=16;
	temp = (x&0x07)0xB8;
	for(i=0;i<3;i++)
	  WriteCom(i,temp);
	
	 
}
void text(int lr,int x,int y,char *s)
	{
	int oDat[8][8],nDat[8];
	int k,i,j;
	unsigned int ch;
	long int order;
	char dot[33],*hz_matrix;
	FILE *fp;
	int m;
	x=x+0xb8;
	y=y+0x40;
	if ((fp=fopen("c:\\ucdos\\hzk16","rb"))==NULL)
	{
	printf ("Can open the hzk16 %s\n");
	getch ( );
	exit (1);
	}
	if ((hz_matrix=(char*)malloc(256))==NULL)
	{
	printf ("Not enough memory\n");
	exit (1);
	}
	while (*s)
	{
		k=0;
		order=(long)((*(s+1)&0x007f)-0x21+94*((*s&0x7f)-0x30)+15*94)*32;
		fseek (fp,(long)order,SEEK_SET);
		fread (dot,32,1,fp);
		for (i=0;i<16;i++,k+=2)
		{
			ch=((unsigned char)dot[k]<<8)+(unsigned char)dot[k+1];
			for (j=0;j<16;j++,ch<<=1)
			{if (ch&0x8000) hz_matrix[j+16*i]=1;
			else hz_matrix[j+16*i]=0;
			}
		}
		s++;
		s++;
	}
	for (i=0;i<8;i++)
		for (j=0;j<8;j++)
			oDat[i][j]=hz_matrix[j+16*i];
	for (i=0;i<8;i++)
	{nDat[i]=0;
		for (j=0;j<8;j++)
			nDat[i]=nDat[i]+oDat[j][i]*sgl(2,j);
	dis(lr,x,y+i,nDat[i]);
	}
	for (i=0;i<8;i++)
		for(j=0;j<8;j++)
			oDat[i][j]=hz_matrix[8+j+16*i];
	for (i=0;i<8;i++)
	{nDat[i]=0;
		for(j=0;j<8;j++)
			nDat[i]=nDat[i]+oDat[j][i]*sgl(2,j);
	dis(lr,x,y+8+i,nDat[i]);
	}

	for (i=0;i<8;i++)
		for(j=0;j<8;j++)
			oDat[i][j]=hz_matrix[j+16*(i+8)];
	for (i=0;i<8;i++)
	{nDat[i]=0;
		for(j=0;j<8;j++)
			nDat[i]=nDat[i]+oDat[j][i]*sgl(2,j);
	dis(lr,x+1,y+i,nDat[i]);
	}
	for (i=0;i<8;i++)
		for(j=0;j<8;j++)
			oDat[i][j]=hz_matrix[8+j+16*(i+8)];
	for (i=0;i<8;i++)
	{nDat[i]=0;
		for(j=0;j<8;j++)
			nDat[i]=nDat[i]+oDat[j][i]*sgl(2,j);
	dis(lr,x+1,y+8+i,nDat[i]);
	wrleftcom(0x3f);}

	fclose (fp);
	free (hz_matrix);
}

