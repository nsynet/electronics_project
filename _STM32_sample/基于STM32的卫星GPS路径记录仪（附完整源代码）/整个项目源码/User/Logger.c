/*-------------------------------------------------------
      路径记录和浏览转换 by wowbanui 2011
 
 版本历史:
 Rev.201108     初始版本
---------------------------------------------------------*/

#include "logger.h"
#include "S1D15E06.h"
#include "GPS.h"
#include "W25Q16.h"
#include "keyboard.h"
#include "FATFS/ff.h"
#include <stdio.h>
#include <string.h>


static u16  DataMapIndex = 0;               // 映射区地址 每字节对应一页
static u8   DataIndexInPage = 0;            // 每页中的记录索引 0-7
static u8   FlashStatus = FLASH_STATUS_UNKNOW;    // 当前 Flash 状态

const unsigned short BrowseStepArr[]={1,8,10,100,128,256,512,1000};
 
void BrowseRecord(void)
{      
    u16 BrowseIndex=0x00000,BrowseStep=0x0001;
    u8 BrowseStepIndex=0;
    char cBrowseBuffDate[]="00/00/00";
    char cBrowseBuffTime[]="00:00:00";
    char cBrowseBuffLati[]="北纬 00'00'00.000'";
    char cBrowseBuffLong[]="东经000'00'00.000'";

    u8 ucBuf[32];
    u8 MapShowBuf[398];
    u16 uiTemp;
    u8 NeedUpdate=1;


    LCD_GUI_BG1();
    LCD_SetGray(GRAY_WHITE,GRAY_BLACK);
    LCD_PutStr12(0,0,"浏览记录");
    LCD_SetGray(GRAY_BLACK,GRAY_GW); 
    LCD_PutStr12(55,0,"退出 \x10   \x11");

    LOG_ScanPageMapShow(MapShowBuf);
    LCD_GUI_Rect(0,4,160,11,0);

    while(1)
    {
        switch(KBD_GetKey())
        {
        case KEY_MENU:
            return;
            //break;

        case KEY_DOWN:
            BrowseIndex+=BrowseStep;
            if (BrowseIndex> 1592*1024/32 -1 )
                BrowseIndex=0;
            NeedUpdate=1;
            break;

        case KEY_UP:
            BrowseIndex-=BrowseStep;
            if (BrowseIndex> 1592*1024/32 -1 )
                BrowseIndex=1592*1024/32 -1;
            NeedUpdate=1;
            break;

        case KEY_ENTER:
            BrowseStepIndex++;
            if (BrowseStepIndex>7) BrowseStepIndex=0;
            BrowseStep=BrowseStepArr[BrowseStepIndex];
            NeedUpdate=1;
            break;
        }

        if (NeedUpdate)
        {
            W25Q_ReadMultiBytes(ucBuf,DATA_BASE_ADDR + BrowseIndex*32,32);
        
            switch (ucBuf[0])
            {
            case '$':
                sprintf(cBrowseBuffDate,"%2d/%2d/%2d",  ucBuf[1],ucBuf[2],ucBuf[3]);
                sprintf(cBrowseBuffTime,"%2d:%02d:%02d",ucBuf[4],ucBuf[5],ucBuf[6]);
                if ((ucBuf[7]==0x00)&&(ucBuf[8]==0x00)&&(ucBuf[0x0C]==0x00)&&(ucBuf[0x0D]==0x00))   //经纬度 0
                {                         //"东经000'00'00.000'"
                    sprintf(cBrowseBuffLati,"纬度 *DATA NULL   ");
                    sprintf(cBrowseBuffLong,"经度 *数据为零    ");
                }
                else
                {
                    uiTemp=ucBuf[9];
                    uiTemp<<=8;
                    uiTemp|=ucBuf[10];
                    sprintf(cBrowseBuffLati,"北纬 %02d\x01%02d'%06.3f\"",ucBuf[7],ucBuf[8],(float)uiTemp*60/10000); 
                
                    uiTemp=ucBuf[0x0E];
                    uiTemp<<=8;
                    uiTemp|=ucBuf[0x0F];
                    sprintf(cBrowseBuffLong,"东经%03d\x01%02d'%06.3f\"",ucBuf[0x0C],ucBuf[0x0D],(float)uiTemp*60/10000);        
                }
                break;
        
            case '-':
                sprintf(cBrowseBuffDate,"%2d/%2d/%2d",  ucBuf[1],ucBuf[2],ucBuf[3]);
                sprintf(cBrowseBuffTime,"%2d:%02d:%02d",ucBuf[4],ucBuf[5],ucBuf[6]);
                sprintf(cBrowseBuffLati,"纬度 *DATA INVALID");
                sprintf(cBrowseBuffLong,"经度 *未定位的数据");
                break;                //"东经000'00'00.000'"

            case 0xFF:
                sprintf(cBrowseBuffDate,"--/--/--");
                sprintf(cBrowseBuffTime,"--:--:--");
                sprintf(cBrowseBuffLati,"** FLASH BLANK ** ");
                sprintf(cBrowseBuffLong,"**    空白     ** ");
                break;                //"东经000'00'00.000'"
                        
            default:
                sprintf(cBrowseBuffDate,"--/--/--");
                sprintf(cBrowseBuffTime,"--:--:--");
                sprintf(cBrowseBuffLati,"**  DATA ERROR  **");
                sprintf(cBrowseBuffLong,"** 数据格式错误 **");
            }
                                     //"东经000'00'00.000'"
            LCD_SetGray(GRAY_BLACK,GRAY_GW);
            LCD_PutStr08(4,7,cBrowseBuffDate);
            LCD_PutStr08(4,9,cBrowseBuffTime);
            LCD_PutStr12(48,5,cBrowseBuffLati);
            LCD_PutStr12(48,8,cBrowseBuffLong);

            LCD_SetGray(GRAY_BLACK,GRAY_WHITE);
            sprintf(cBrowseBuffTime,">%4d:%d<",BrowseIndex/8,BrowseIndex%8);
            LCD_PutStr08(4,5,cBrowseBuffTime);
            
            sprintf(cBrowseBuffTime,"+%4d",BrowseStep);
            LCD_PutStr12(130,0,cBrowseBuffTime);

            LCD_GUI_FlashMap(MapShowBuf,BrowseIndex/8/16);
            NeedUpdate=0;
        }
    }
}


void LOG_Point(void)
{
    u8 FlashLogBuf[32];

    if (FlashStatus == FLASH_STATUS_NEEDSTART)
    {
        W25Q_WriteByte(DATA_MAP_PAGE_TRACK, DATA_MAP_BASE_ADDR + DataMapIndex );
        FlashStatus = FLASH_STATUS_MOUNTED;
    }

    if (FlashStatus == FLASH_STATUS_MOUNTED)
    {
        GPS_GetLogData(FlashLogBuf);
        W25Q_WritePageBytes(FlashLogBuf, DATA_BASE_ADDR + DataMapIndex*256 + DataIndexInPage*32,32);
        
        DataIndexInPage++;

        if (DataIndexInPage > 7)
        {  
            W25Q_WriteByte(DATA_MAP_PAGE_FULL, DATA_MAP_BASE_ADDR + DataMapIndex );
            DataMapIndex ++; 
            W25Q_WriteByte(DATA_MAP_PAGE_USED, DATA_MAP_BASE_ADDR + DataMapIndex );   
            DataIndexInPage = 0;

            if (DataMapIndex>MAX_DATA_PAGES-1)  // 空间已满
            {
                FlashStatus = FLASH_STATUS_DISKFULL;
            }
        }
    }
}

u8 LOG_MountFlash(void)       // 装载 Flash,
{
    // u16 i;

    DataMapIndex = LOG_ScanPageMap();   // 映射表中可用页的地址

    if (DataMapIndex > (MAX_DATA_PAGES - 1))      // 空间已满
    {
        FlashStatus = FLASH_STATUS_DISKFULL;
        return FlashStatus;
    }

    DataIndexInPage = 0;


    FlashStatus = FLASH_STATUS_NEEDSTART;

    return 0;
}

u16 LOG_GetDataMapIndex(void)
{
    return DataMapIndex;
}

u8 LOG_GetDataIndexInPage(void)
{
    return DataIndexInPage;
}

void LOG_GetLastRecord(char* pStr1, char* pStr2)
{
    u8 ucBuf[32];
    u16 uiTemp;


    W25Q_ReadMultiBytes(ucBuf,DATA_BASE_ADDR + DataMapIndex*256 + DataIndexInPage*32-32,32);

    switch (ucBuf[0])
    {
    case '$':
        if ((ucBuf[7]==0x00)&&(ucBuf[8]==0x00)&&(ucBuf[0x0C]==0x00)&&(ucBuf[0x0D]==0x00))   //经纬度 0
        {
            sprintf(pStr1,"%2d/%2d/%2d - DATA NULL -",ucBuf[1],ucBuf[2],ucBuf[3]);
            sprintf(pStr2,"%2d:%02d:%02d - DATA NULL -",ucBuf[4],ucBuf[5],ucBuf[6]);
        }
        else
        {
            uiTemp=ucBuf[9];
            uiTemp<<=8;
            uiTemp|=ucBuf[10];
            sprintf(pStr1,"%2d/%2d/%2d %3d\x02%02d'%06.3f\"N",ucBuf[1],ucBuf[2],ucBuf[3],ucBuf[7],ucBuf[8],(float)uiTemp*60/10000); 
        
            uiTemp=ucBuf[0x0E];
            uiTemp<<=8;
            uiTemp|=ucBuf[0x0F];
            sprintf(pStr2,"%2d:%02d:%02d %3d\x02%02d'%06.3f\"E",ucBuf[4],ucBuf[5],ucBuf[6],ucBuf[0x0C],ucBuf[0x0D],(float)uiTemp*60/10000);        
        }
        break;

    case '-':
        sprintf(pStr1,"%2d/%2d/%2d -DATA INVALID-",ucBuf[1],ucBuf[2],ucBuf[3]);
        sprintf(pStr2,"%2d:%02d:%02d -DATA INVALID-",ucBuf[4],ucBuf[5],ucBuf[6]);
        break;

    case 0xFF:
        sprintf(pStr1,"--/--/--  - EMPTY - ");
        sprintf(pStr2,"--:--:--  - EMPTY - ");
        break;

    default:
        sprintf(pStr1," --- DATA ERROR --- ");
        sprintf(pStr2," --- DATA ERROR --- ");
    }
}


void LOG_DumpFlashToTF(void)
{
    //--------------------
    // FATFS 相关变量
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //  由于内部变量较大, 需要手动设置栈的大小, 
    //  否则会溢出, 导致错误, 并且不容易检查!
    // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    FATFS   fsFAT;
    FIL     TFFile;
    FRESULT fRes;
    UINT    br;
    //--------------------
    u8 FileBuffer[2050];
    char filename[]="GPS/FDUMP000.bin";
    char tempStr[50];
    u16 filenameCount=0x0000;
    u32 i;

    LCD_GUI_Rect(5,1,155,23,0);
    LCD_SetGray(GRAY_BLACK,GRAY_GB);
    LCD_PutStr12(10,2,"转储 Flash 数据到 TF 卡"); 
    LCD_SetGray(GRAY_BLACK,GRAY_GW);
       
    fRes = f_mount(0, &fsFAT);  // 装载 TF 卡
    do{
        sprintf(filename,"GPS/FDUMP%03X.bin",filenameCount);
        fRes = f_open(&TFFile,filename, FA_CREATE_NEW | FA_WRITE);
        
        switch(fRes)
        {
        case FR_EXIST:
            filenameCount++;
            if (filenameCount>0xFFF)
            {
                LCD_PutStr12(10,6,"文件太多了! ");
                LCD_GUI_Button(120,18,"确定",0);
                f_mount(0,NULL);
                while(1)
                {
                    if (KBD_GetKey()==KEY_ENTER) return;
                }
            }
            break;

        case FR_OK:
            break;

        default:
            sprintf(tempStr,"发生错误! Code 0x%02X",fRes);
            LCD_PutStr12(20,12,tempStr);
            LCD_GUI_Button(120,18,"确定",0);
            f_mount(0,NULL);
            while(1)
            {
                if (KBD_GetKey()==KEY_ENTER) return;
            }
        }
    }while(fRes!=FR_OK);
    // 文件已经打开
    LCD_PutStr12(10,6,"文件名:");
    LCD_PutStr12(55,6,filename);
    LCD_PutStr12(10,9,"进度: ");
                
    for (i=0;i<800;i++)
    {
        W25Q_ReadMultiBytes(FileBuffer,DATA_MAP_BASE_ADDR +i*2048,2048);
        fRes = f_write(&TFFile, FileBuffer, 2048, &br);
        sprintf(tempStr,"%3d/800",i+1);
        LCD_PutStr12(55,9,tempStr);
        sprintf(tempStr,"DEBUG: 0x%02X,%4d",fRes,br);
        LCD_PutStr12(10,12,tempStr);
    }
               
    fRes = f_close(&TFFile);
    if (fRes!=FR_OK) return ;
       
    LCD_PutStr12(10,15,"转储完成! ");
    LCD_GUI_Button(120,18,"确定",0);
    fRes = f_mount(0,NULL);
    sprintf(tempStr,"DEBUG: 0x%02X",fRes);
    LCD_PutStr12(10,18,tempStr);
    while(1)
    {
        if (KBD_GetKey()==KEY_ENTER) return;
    }
}
          
const char KML_H01[]="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n";
const char KML_H02[]="<kml xmlns=\"http://earth.google.com/kml/2.2\">\r\n";
const char KML_H03[]="  <Document>\r\n";
const char KML_H04[]="    <name>Path from GPS Logger V2</name>\r\n";
const char KML_H05[]="    <Placemark>\r\n";                             // 可以有多个 Placemark
const char KML____[]="      <name>2011/08/12 00:00:00</name>";
const char KML_H06[]="      <Style>\r\n";
const char KML_H07[]="        <LineStyle>\r\n";
const char KML_H08[]="          <color>ff00ffff</color>\r\n";           // 线条颜色
const char KML_H09[]="          <width>5</width>\r\n";                  // 线条宽度
const char KML_H10[]="        </LineStyle>\r\n";
const char KML_H11[]="      </Style>\r\n";
const char KML_H12[]="     <LineString>\r\n";
const char KML_H13[]="        <tessellate>1</tessellate>\r\n";              
const char KML_H14[]="        <altitudeMode>absolute</altitudeMode>\r\n";
const char KML_H15[]="        <coordinates>";
//                   此处经纬度 空格割开, 没有换行
const char KML_H16[]="</coordinates>\r\n";
const char KML_H17[]="      </LineString>\r\n";
const char KML_H18[]="    </Placemark>\r\n";
const char KML_H19[]="  </Document>\r\n";
const char KML_H20[]="</kml>\r\n";

//u8 LOG_
           
void LOG_KMLToTF(void)
{
    //--------------------
    // FATFS 相关变量
    FATFS   fsFAT;
    FIL     TFFile;
    FRESULT fRes;
    UINT    br;
    //--------------------
    u8 DateTime[6];
    char filename[]="GPS/YYMMDD00.KML";
    char tempStr[50];
    u8 PointData[32];
    u16 filenameCount=0x0000;
    u32 i;
    double la,lo;

    LCD_GUI_Rect(5,1,155,23,0);
    LCD_SetGray(GRAY_BLACK,GRAY_GB);          // 
    LCD_PutStr12(10,2,"保存为 KML 文件"); 
    LCD_SetGray(GRAY_BLACK,GRAY_GW);
    
    GPS_GetTime(DateTime);      // 获取 GPS 的时间
      
    fRes = f_mount(0, &fsFAT);  // 装载 TF 卡
    do{
        sprintf(filename,"GPS/%02d%02d%02d%02X.KML",DateTime[0],DateTime[1],DateTime[2],filenameCount);
        fRes = f_open(&TFFile,filename, FA_CREATE_NEW | FA_WRITE);
        
        switch(fRes)
        {
        case FR_EXIST:
            filenameCount++;
            if (filenameCount>0xFF)
            {
                LCD_PutStr12(10,6,"文件太多了! ");
                LCD_GUI_Button(120,18,"确定",0);
                f_mount(0,NULL);
                while(1)
                {
                    if (KBD_GetKey()==KEY_ENTER) return;
                }
            }
            break;

        case FR_OK:
            break;

        default:
            sprintf(tempStr,"发生错误! Code 0x%02X",fRes);
            LCD_PutStr12(20,12,tempStr);
            LCD_GUI_Button(120,18,"确定",0);
            f_mount(0,NULL);
            while(1)
            {
                if (KBD_GetKey()==KEY_ENTER) return;
            }
        }
    }while(fRes!=FR_OK);
    // 文件已经打开
    LCD_PutStr12(10,6,"文件名:");
    LCD_PutStr12(55,6,filename);
    LCD_PutStr12(10,9,"进度: ");
    //strlen

    fRes = f_write(&TFFile, KML_H01, strlen(KML_H01), &br);
    fRes = f_write(&TFFile, KML_H02, strlen(KML_H02), &br);
    fRes = f_write(&TFFile, KML_H03, strlen(KML_H03), &br);
    fRes = f_write(&TFFile, KML_H04, strlen(KML_H04), &br);
    fRes = f_write(&TFFile, KML_H05, strlen(KML_H05), &br);
    fRes = f_write(&TFFile, KML____, strlen(KML____), &br);
    fRes = f_write(&TFFile, KML_H06, strlen(KML_H06), &br);
    fRes = f_write(&TFFile, KML_H07, strlen(KML_H07), &br);
    fRes = f_write(&TFFile, KML_H08, strlen(KML_H08), &br);
    fRes = f_write(&TFFile, KML_H09, strlen(KML_H09), &br);
    fRes = f_write(&TFFile, KML_H10, strlen(KML_H10), &br);
    fRes = f_write(&TFFile, KML_H11, strlen(KML_H11), &br);
    fRes = f_write(&TFFile, KML_H12, strlen(KML_H12), &br);
//    fRes = f_write(&TFFile, KML_H13, strlen(KML_H13), &br);
//    fRes = f_write(&TFFile, KML_H14, strlen(KML_H14), &br);
    fRes = f_write(&TFFile, KML_H15, strlen(KML_H15), &br);


    for (i=0;i<((1600-8)*1024/32);i++)
    {
        W25Q_ReadMultiBytes(PointData,DATA_BASE_ADDR +i*32,32);
        if ((PointData[0]=='$')&&PointData[0x07]&&PointData[0x0C])
        {
            filenameCount=0x0000;
            filenameCount= PointData[0x0E];
            filenameCount<<=8;
            filenameCount|= PointData[0x0F]; // 经度 mmmm
            lo= filenameCount;
            lo=lo/10000;
            lo+=PointData[0x0D]; // 经度 mm
            lo=lo/60;
            lo+=PointData[0x0C]; // 经度 ddd
            filenameCount=0x0000;
            filenameCount= PointData[0x09];
            filenameCount<<=8;
            filenameCount|= PointData[0x0A]; // 纬度 mmmm
            la= filenameCount;
            la=la/10000;
            la+=PointData[0x08]; // 纬度 mm
            la=la/60;
            la+=PointData[0x07]; // 纬度 dd

            sprintf(tempStr,"%f,%f ",lo,la);
            fRes = f_write(&TFFile, tempStr, strlen(tempStr), &br);

        }
        sprintf(tempStr,"%5d/%d",i+1,(1600-8)*1024/32);
        LCD_PutStr12(55,9,tempStr);    
    
    }
    fRes = f_write(&TFFile, KML_H16, strlen(KML_H16), &br);
    fRes = f_write(&TFFile, KML_H17, strlen(KML_H17), &br);
    fRes = f_write(&TFFile, KML_H18, strlen(KML_H18), &br);
    fRes = f_write(&TFFile, KML_H19, strlen(KML_H19), &br);
    fRes = f_write(&TFFile, KML_H20, strlen(KML_H20), &br);
               
    fRes = f_close(&TFFile);
    if (fRes!=FR_OK) return ;
       
    LCD_PutStr12(10,15,"转储完成! ");
    LCD_GUI_Button(120,18,"确定",0);
    fRes = f_mount(0,NULL);
    sprintf(tempStr,"DEBUG: 0x%02X",fRes);
    LCD_PutStr12(10,18,tempStr);
    while(1)
    {
        if (KBD_GetKey()==KEY_ENTER) return;
    }
}

