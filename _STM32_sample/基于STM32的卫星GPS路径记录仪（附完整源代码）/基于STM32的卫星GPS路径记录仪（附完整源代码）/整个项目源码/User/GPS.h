#ifndef __GPS_H
#define __GPS_H

#include "common.h"

typedef struct
{
    u8 UTCDateTime[6];  // YMDHMS
    char Status;        // A/V [ASCII]

    u8 LatitudeD;       // dd
    u8 LatitudeM;       // mm
    u16 LatitudeS;      // mmmm
    char NS;            // N/S [ASCII] 

    u8 LongitudeD;      // ddd
    u8 LongitudeM;      // mm
    u16 LongitudeS;     // mmmm
    char EW;            // E/W [ASCII]

    u16 Speed;          // 速率000.0~999.9节  x10
    char sSpeed[5];     // DEBUG
    u16 Course;         // 航向000.0~359.9度  x10
    char sCourse[5];    // DEBUG
}struct_GPSRMC;

typedef struct
{
//  u8 UTCTime[10];	    // hhmmss.mmm
//  u8 Latitude[9];	    // ddmm.mmmm
//  char NS;		    // N/S
//  u8 Longitude[10];	// dddmm.mmmm
//  char EW;		    // E/W
    char PositionFix;	// 0,1,2,6 [ASCII]
    u8 SatUsed;	        // 00~12
//  u8 HDOP[4];		    // 0.5~99.9
    char Altitude[8];	// -9999.9~99999.9 [ASCII]
}struct_GPSGGA;

typedef struct
{
    char Mode;		    // A/M [ASCII]
    char Mode2;	        // 0,1,2,3 [ASCII]
    u8 SatUsedList[12];	// 使用中的卫星号
    char PDOP[5];       // 位置精度 0.5-99.9
    char HDOP[5];       // 水平精度 0.5-99.9
    char VDOP[5];       // 垂直精度 0.5-99.9
}struct_GPSGSA;

typedef struct
{
    u8 SatID;           // 卫星号
    u8 Elevation;       // 卫星仰角   00-90 度
    u16 Azimuth;        // 卫星方位角 000-359 度
    u8 SNR;             // 讯号噪声比 00-99 dbHz
}struct_SatInfo;

typedef struct
{
    u8 SatInView;
    struct_SatInfo SatInfo[12];
}struct_GPSGSV;

#define NMEA_NULL   0x00        //GPS语句类型
#define NMEA_GPGGA  0x01
#define NMEA_GPGSA  0x02
#define NMEA_GPGSV  0x04
#define NMEA_GPRMC  0x08

#define GPS_PARSE_FAILURE           0x00
#define GPS_PARSE_START             0x01
#define GPS_PARSE_CHECKSUM_ERROR    0x02
#define GPS_PARSE_OK                0x03

u8 GPS_Parser(char SBuf);
void GPS_View_Basic(u8 ucLogStatus);
void GPS_View_SatInfo(void);
void GPS_View_Log(void);
void GPS_View_Speed(void);
void GPS_GetLogData(u8 * LogBuf);
void GPS_GetTime(u8* ucDataTime);


#endif
