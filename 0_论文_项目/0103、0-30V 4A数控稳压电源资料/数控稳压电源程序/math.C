#include "reg52.h"
#include "math.h"
#define  uchar unsigned char
#define uint unsigned int

void draw(uchar x,uchar y);//画点函数   
void clear_xy(uchar x,uchar y);//清点函数  

/*画正弦波函数 */     
void sin_lcd(void)
{
    float x,y;
    for(x=0;x<128;x+=0.15)
    {
        draw(x,y=20*sin(x*1/4)+32);
    }
    for(x=0;x<128;x+=0.15)
    {
        clear_xy(x,y=20*sin(x*1/4)+32);
    }
}
/*画圆函数 */
void round(uchar x,uchar y,uchar r)
{
    float angle;
    uchar dx,dy;
    for(angle=0;angle<3.14/4;angle+=0.03)//0.03
    {
        dx=r*sin(angle);
        dy=r*cos(angle);
        draw(x+dx,y+dy);
        draw(x-dx,y+dy);
        draw(x+dx,y-dy);
        draw(x-dx,y-dy);
        draw(x+dy,y+dx);
        draw(x-dy,y+dx);
        draw(x+dy,y-dx);
        draw(x-dy,y-dx);
    }
}
/*画椭圆函数 */
void ellipsoid(uchar x,uchar y,uchar a,uchar b)
{
    float angle;
    uchar dx,dy;
    for(angle=0;angle<6.28;angle+=0.06)
    {
        dx=a*sin(angle);
        dy=b*cos(angle);
        draw(x+dx,y+dy);
        draw(x+dx,y-dy);
        draw(x-dx,y-dy);
        draw(x-dx,y+dy);
    }
}
/*抛物线 */
void parabalo(float x)
{
    for(;x<128;x+=0.4)
    draw(x,x*x/256);
}
/*对数 */
void fan(float x)
{
    for(;x<128;x+=0.2)
    draw(x,30*log10(x));
}
/**/
void nn(uchar x,uchar y,uchar R)
{
    float angle,x0,y0,r1=15,r2=5;
    for(angle=0;angle<2*3.14*3;angle+=0.03)
    {
        x0=(R-r1)*sin(angle)-r2*sin(R/r1*angle);
        y0=(R-r1)*cos(angle)+r2*cos(R/r1*angle);
        draw(x+x0,y+y0);
    }
}
/*
void beenline(float x,float y,float xe,float ye)
{
    float dx,k;
    k=(ye-y)/(xe-x);
    dx=(xe-x)/128;
    for(;x<=xe;x+=dx)
    {
        draw(x,k*x+y);
    }
}
*/
void  Lin(uchar x1,uchar y1,uchar x2,uchar y2)
{
    uint x=x1<<8,y=y1<<8;     //高8位为坐标值，低8位为积分累加器
    char dx=x2-x1,dy=y2-y1,mm,nn;

    while( abs(dx)<64 && abs(dy)<64 && dx*dy!=0 ) {dx*=6;dy*=6;} //调整插补速度	 //2

    while( (x>>8)!=x2 || (y>>8)!=y2)
    {
        draw(mm=x>>8,nn=y>>8);
        x+=dx;
        y+=dy;
    }

    draw(x>>8,y>>8);         //画终点
}
