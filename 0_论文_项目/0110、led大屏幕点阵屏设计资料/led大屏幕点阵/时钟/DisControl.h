
ChangeData(unsigned char row)
{
   switch(row)
   {
	   case 0 : RowA=RowB=RowC=1;  break;
		case 1 : RowC=RowB=1;RowA=0;break;
		case 2 : RowA=RowC=1;RowB=0;break;
		case 3 : RowA=RowB=0;RowC=1;break;
		case 4 : RowA=RowB=1;RowC=0;break;
		case 5 : RowA=RowC=0;RowB=1;break;
		case 6 : RowB=RowC=0;RowA=1;break;
		case 7 : RowA=RowB=RowC=0;  break;
		default :break;
	}
   UpData=1;
   UpData=0;
   DisSwitch=1;
   //delay(20);
   DS1302_GetTime(&CurrentTime);
   DisSwitch=0;
}
Out_byte(unsigned char row,unsigned char  cs,unsigned char *byte) //行，
{
   if(cs==2) WRled = byte[8+row];
   else      WRled = byte[row];
}

Out_word(unsigned char row,unsigned char  cs,unsigned char *word)
{ 
  if(cs==2)
  {
     WRled=word[16+row*2+1];    //先发高字节，但是最终是写在后面。          
	  WRled=word[16+row*2];
  }
  else  
  {
     WRled=word[row*2+1]; //先发高字节，但是最终是写在后面。       
	  WRled=word[row*2];
  }
}

Out_row(unsigned char row)
{  
   unsigned char cs;
   for(cs=2;cs>0;cs--)
   {		    
      Out_word(row,cs,ri);
		Out_word(row,cs,qi);	 //日期
		Out_byte(row,cs,jiantou);

	   Out_byte(row,cs,num[CurrentTime.Year/10]); //年
      Out_byte(row,cs,num[CurrentTime.Year%10]);

		Out_byte(row,cs,ruandian);

	   Out_byte(row,cs,num[CurrentTime.Month/10]); //月
      Out_byte(row,cs,num[CurrentTime.Month%10]);

		Out_byte(row,cs,ruandian);

	   Out_byte(row,cs,num[CurrentTime.Day/10]); //日
      Out_byte(row,cs,num[CurrentTime.Day%10]);

	   Out_word(row,cs,wen);
		Out_word(row,cs,du);	  //温度

    	Out_byte(row,cs,jiantou);

	   Out_byte(row,cs,num2); //温度
      Out_byte(row,cs,num5); 
   }
   for(cs=2;cs>0;cs--)
   {      
	   Out_word(row,cs,shi);
		Out_word(row,cs,jian);	 //时间
		Out_byte(row,cs,jiantou);

	   Out_byte(row,cs,num[CurrentTime.Hour/10]);	//时
      Out_byte(row,cs,num[CurrentTime.Hour%10]);

		Out_byte(row,cs,ruandian);

	   Out_byte(row,cs,num[CurrentTime.Minute/10]);	//分
      Out_byte(row,cs,num[CurrentTime.Minute%10]);

		Out_byte(row,cs,ruandian);

	   Out_byte(row,cs,num[CurrentTime.Second/10]);	//秒
      Out_byte(row,cs,num[CurrentTime.Second%10]);

		Out_word(row,cs,xing);
		Out_word(row,cs,qi);	  //星期

    	//Out_byte(row,cs,jiantou);

	   Out_byte(row,cs,mie);
      Out_byte(row,cs,num[CurrentTime.Week]);	//星期
	  Out_byte(row,cs,mie);       
   }
}
Out_count(unsigned char row)
{
   unsigned char cs;
   for(cs=2;cs>0;cs--)
   {	   
	   Out_byte(row,cs,mie);
	   Out_byte(row,cs,mie);
      Out_word(row,cs,zi);	  //子		   	    
      Out_word(row,cs,jing);
		Out_word(row,cs,sai);	 //竞赛

		Out_word(row,cs,hai);	 
		Out_word(row,cs,you);	 //还有

	   Out_byte(row,cs,num[CountDay()/10]); //
      Out_byte(row,cs,num[CountDay()%10]);

		Out_word(row,cs,tian);	 //天
		Out_word(row,cs,gan_tan_hao);//天

		Out_byte(row,cs,mie);
	   Out_byte(row,cs,mie);

   }
   for(cs=2;cs>0;cs--)
   {        
	   Out_byte(row,cs,mie);
	   Out_byte(row,cs,mie);
		   	    
      Out_word(row,cs,ju);
		Out_word(row,cs,li);	 //距离

		Out_word(row,cs,quan);	 
		Out_word(row,cs,guo);	 //全国

      Out_word(row,cs,da);
		Out_word(row,cs,xue);	 //大学

		Out_word(row,cs,sheng);	 
		Out_word(row,cs,dian);	 //生电

		Out_byte(row,cs,mie);
	   Out_byte(row,cs,mie);       
   }
}

Out_pic(unsigned char Row,unsigned char *pic)
{
   unsigned char Page;
   unsigned char count;			    
   for(Page=4;Page>0;Page--)
   {
	   for(count=20;count>0;count--)
	   {
		   WRled=pic[160*(Page-1)+20*Row+count-1];
		}
   }
}



















