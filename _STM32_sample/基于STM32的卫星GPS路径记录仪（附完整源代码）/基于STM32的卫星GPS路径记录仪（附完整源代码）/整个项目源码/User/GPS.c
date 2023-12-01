/*-------------------------------------------------------
      GPS ����ģ�� By wowbanui

 �汾��ʷ:
 Rev.2010--     ��ʼ�汾
 Rev.201103     1. �Ż��ṹ����, ��������ֱ�Ӵ������ֵ,�����ڴ�ռ��.
                2. �������ֱ�ӵ���LCD����,��ֲ�����
 Rev.201107     1. �޸ľ�γ�ȵ�Ϊ��ֵ��ʽ
                2. �޸ĳ�ʼ�����ݷ���

---------------------------------------------------------*/
#include "GPS.H"
#include "S1D15E06.h"
#include "Logger.h"
#include <stdio.h>
#include <stdlib.h>


static u8   NMEA_Start = 0;                 // NMEA ��俪ʼ. ��⵽ $ ʱ�� 1
static u8   NMEA_TypeParsed = 0;            // NMEA ���ͽ������
static u8   NMEA_MsgType = NMEA_NULL;       // NMEA �������
static char NMEA_MsgTypeBuff[] = "GPxxx,";  // NMEA �������ʶ�𻺴�
static u8   NMEA_MsgTypeIndex = 0;          // ��ȡ NMEA ����ʶ���ַ��ĸ���
static u8   NMEA_MsgBlock = 0;              // NMEA �����ֶκ� ��0��ʼ
static u8   NMEA_MsgBlockDatIndex = 0;      // NMEA ����ÿ���ֶ����ַ����� ��0��ʼ

static u8   GPS_Parse_Status = 0;           // ��ǰ����״̬.

static u8   SateInfoIndex = 0;              //


static struct_GPSRMC GPS_RMC_Data;
static struct_GPSGGA GPS_GGA_Data;
static struct_GPSGSA GPS_GSA_Data;
static struct_GPSGSV GPS_GSV_Data;


static void ParserGPGGA(char SBuf)
{
    switch (SBuf)
    {
    case '*':   //������
        NMEA_Start=0;
        break;
    case ',':   //���ֶν���
        NMEA_MsgBlock++;
        NMEA_MsgBlockDatIndex=0;
        break;
    default:    //�ֶ��ַ�
        switch (NMEA_MsgBlock)  // �жϵ�ǰ�����ĸ��ֶ�
        {
            /*
            case 0:             // <1> UTCʱ��,hhmmss
                break;
            case 1:             // <2> γ�� ddmm.mmmm
                break;
            case 2:             // <3> γ�Ȱ��� N/S
                break;
            case 3:             // <4> ���� dddmm.mmmm
                break;
            case 4:             // <5> ���Ȱ��� E/W
                break;
            */
        case 5:             // <6> GPS״̬ 0=δ��λ, 1=�ǲ�ֶ�λ, 2=��ֶ�λ, 6=���ڹ���
            GPS_GGA_Data.PositionFix=SBuf;
            break;
        case 6:             // <7> ����ʹ�õ��������� 00~12
            switch (NMEA_MsgBlockDatIndex)
            {
            case 0:
                GPS_GGA_Data.SatUsed=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GGA_Data.SatUsed+=(SBuf-'0');
                break;
            }
            break;
            /*
            case 7:             //<8> HDOPˮƽ�������� 0.5~99.9
                GPS_GGA_Data.HDOP[GPS_GGA_Data.BlockIndex]=SBuf;
                break;
            */
        case 8:         //<9> ���θ߶� -9999.9~99999.9
            GPS_GGA_Data.Altitude[NMEA_MsgBlockDatIndex]=SBuf;
            break;
        }
        NMEA_MsgBlockDatIndex++;     //�ֶ��ַ�����++, ָ����һ���ַ�
    }
}

static void ParserGPRMC(char SBuf)
{
    switch (SBuf)
    {
    case '*':
        NMEA_Start=0;
        GPS_Parse_Status=GPS_PARSE_OK;       //�������, ���Դ���
        break;
    case ',':
        NMEA_MsgBlock++;
        NMEA_MsgBlockDatIndex=0;
        break;
    default:
        switch (NMEA_MsgBlock)
        {
        case 0:         // <1> UTCʱ�� hhmmss.mmm
            switch (NMEA_MsgBlockDatIndex)
            {
            case 0: // hh
                GPS_RMC_Data.UTCDateTime[3]=(SBuf-'0')*10;
                break;
            case 1:
                GPS_RMC_Data.UTCDateTime[3]+=(SBuf-'0');
                GPS_RMC_Data.UTCDateTime[3]+=8;         // Դʱ���� UTC, ת���ɱ���ʱ�� +8, ����Ҫ�ж��Ƿ񳬹�23Сʱ
                break;
            case 2: // mm
                GPS_RMC_Data.UTCDateTime[4]=(SBuf-'0')*10;
                break;
            case 3:
                GPS_RMC_Data.UTCDateTime[4]+=(SBuf-'0');
                break;
            case 4: // ss
                GPS_RMC_Data.UTCDateTime[5]=(SBuf-'0')*10;
                break;
            case 5:
                GPS_RMC_Data.UTCDateTime[5]+=(SBuf-'0');
                break;
            }
            break;
        case 1:         // <2> ��λ״̬ A=��Ч��λ, V=��Ч��λ
            GPS_RMC_Data.Status=SBuf;
            break;
        case 2:         // <3> γ�� ddmm.mmmm
            //GPS_RMC_Data.Latitude[NMEA_MsgBlockDatIndex]=SBuf;    //DEBUG
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, �ֱ�ת������ֵ��
            case 0:     // dd
                GPS_RMC_Data.LatitudeD=(SBuf-'0')*10;
                break;
            case 1:
                GPS_RMC_Data.LatitudeD+=(SBuf-'0');
                break;
            case 2:     // mm
                GPS_RMC_Data.LatitudeM=(SBuf-'0')*10;
                break;
            case 3:
                GPS_RMC_Data.LatitudeM+=(SBuf-'0');
                break;
            case 4:     // С����
                break;
            case 5:     // mmmm
                GPS_RMC_Data.LatitudeS=(SBuf-'0')*1000;
                break;
            case 6:
                GPS_RMC_Data.LatitudeS+=(SBuf-'0')*100;
                break;
            case 7:
                GPS_RMC_Data.LatitudeS+=(SBuf-'0')*10;
                break;
            case 8:
                GPS_RMC_Data.LatitudeS+=(SBuf-'0');
                break;
            }
            break;
        case 3:         //<4> γ�Ȱ��� N/S
            GPS_RMC_Data.NS=SBuf;
            break;
        case 4:         //<5> ���� dddmm.mmmm
            //GPS_RMC_Data.Longitude[NMEA_MsgBlockDatIndex]=SBuf;   //DEBUG
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, �ֱ�ת������ֵ��
            case 0:     // ddd
                GPS_RMC_Data.LongitudeD=(SBuf-'0')*100;
                break;
            case 1:
                GPS_RMC_Data.LongitudeD+=(SBuf-'0')*10;
                break;
            case 2:
                GPS_RMC_Data.LongitudeD+=(SBuf-'0');
                break;
            case 3:     // mm
                GPS_RMC_Data.LongitudeM=(SBuf-'0')*10;
                break;
            case 4:
                GPS_RMC_Data.LongitudeM+=(SBuf-'0');
                break;
            case 5:     // С����
                break;
            case 6:     // mmmm
                GPS_RMC_Data.LongitudeS=(SBuf-'0')*1000;
                break;
            case 7:
                GPS_RMC_Data.LongitudeS+=(SBuf-'0')*100;
                break;
            case 8:
                GPS_RMC_Data.LongitudeS+=(SBuf-'0')*10;
                break;
            case 9:
                GPS_RMC_Data.LongitudeS+=(SBuf-'0');
                break;
            }
            break;
        case 5:         // <6> ���Ȱ��� E/W
            GPS_RMC_Data.EW=SBuf;
            break;
        case 6:         // <7> �������� 000.0~999.9 ��
            //GPS_RMC_Data.sSpeed[NMEA_MsgBlockDatIndex]=SBuf;  //DEBUG
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��, ��ֵx10
            case 0:
                GPS_RMC_Data.Speed=(SBuf-'0')*1000;
                break;
            case 1:
                GPS_RMC_Data.Speed+=(SBuf-'0')*100;
                break;
            case 2:
                GPS_RMC_Data.Speed+=(SBuf-'0')*10;
                break;
            case 3:
                break;
            case 4:
                GPS_RMC_Data.Speed+=(SBuf-'0');
                break;
            }
            break;
        case 7:         // <8> ���溽�� 000.0~359.9 ��, ���汱Ϊ�ο���׼
            //GPS_RMC_Data.sCourse[NMEA_MsgBlockDatIndex]=SBuf; //DEBUG
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��, ��ֵx10
            case 0:
                GPS_RMC_Data.Course=(SBuf-'0')*1000;
                break;
            case 1:
                GPS_RMC_Data.Course+=(SBuf-'0')*100;
                break;
            case 2:
                GPS_RMC_Data.Course+=(SBuf-'0')*10;
                break;
            case 3: // С����, ����
                break;
            case 4:
                GPS_RMC_Data.Course+=(SBuf-'0');
                break;
            }
            break;
        case 8:         // <9> UTC���� ddmmyy
            switch (NMEA_MsgBlockDatIndex)
            {
            case 0: // dd
                GPS_RMC_Data.UTCDateTime[2]=(SBuf-'0')*10;
                break;
            case 1:
                GPS_RMC_Data.UTCDateTime[2]+=(SBuf-'0');
                if (GPS_RMC_Data.UTCDateTime[3]>23)     // ���Сʱ����23, ���������Ҫ+1
                {
                    GPS_RMC_Data.UTCDateTime[3]-=24;    // Hour
                    GPS_RMC_Data.UTCDateTime[2]++;      // Day
                }
                break;
            case 2: // mm
                GPS_RMC_Data.UTCDateTime[1]=(SBuf-'0')*10;
                break;
            case 3:
                GPS_RMC_Data.UTCDateTime[1]+=(SBuf-'0');
                switch (GPS_RMC_Data.UTCDateTime[1])    // ���ݴ�С�����ж������Ƿ����, ������·�++
                {
                case 2:                             // �˴�δ��������29������
                    if (GPS_RMC_Data.UTCDateTime[2]>28)
                    {
                        GPS_RMC_Data.UTCDateTime[2]-=28;
                        GPS_RMC_Data.UTCDateTime[1]++;
                    }
                    break;
                case 1:                             // ���� 31 ��
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                    if (GPS_RMC_Data.UTCDateTime[2]>31)
                    {
                        GPS_RMC_Data.UTCDateTime[2]-=31;
                        GPS_RMC_Data.UTCDateTime[1]++;
                    }
                    break;
                case 4:                             // С�� 30 ��
                case 6:
                case 9:
                case 11:
                    if (GPS_RMC_Data.UTCDateTime[2]>30)
                    {
                        GPS_RMC_Data.UTCDateTime[2]-=30;
                        GPS_RMC_Data.UTCDateTime[1]++;
                    }
                    break;
                }
                break;
            case 4:
                GPS_RMC_Data.UTCDateTime[0]=(SBuf-'0')*10;
                break;
            case 5:
                GPS_RMC_Data.UTCDateTime[0]+=(SBuf-'0');
                if (GPS_RMC_Data.UTCDateTime[1]>12)     // ����·ݳ���, ���� ++
                {
                    GPS_RMC_Data.UTCDateTime[1]=1;
                    GPS_RMC_Data.UTCDateTime[0]++;
                }
                break;
            }
            break;
        }
        NMEA_MsgBlockDatIndex++;  // ���ֶ��ַ���� +1
    }
}

static void ParserGPGSA(char SBuf)
{
    switch (SBuf)
    {
    case '*':               // ������ݽ���, ����2λУ��ֵ
        NMEA_Start=0;
        break;
    case ',':               // ,�ָ���, �ֶ� +1, �ֶ��ڲ��ַ���Ź���
        NMEA_MsgBlock++;
        NMEA_MsgBlockDatIndex=0;
        //�˴�����ȷ�� GPS_GSA_Data.SatUsedList[] ��ʼ��.
        break;
    default:
        switch (NMEA_MsgBlock)
        {
        case 0:         // <1>ģʽ M=�ֶ�, A=�Զ�
            GPS_GSA_Data.Mode=SBuf;
            break;
        case 1:         // <2>��λ��ʽ 1=δ��λ, 2=��ά��λ, 3=��ά��λ
            GPS_GSA_Data.Mode2=SBuf;
            break;
        case 2:         // <3> PRN 01~32 ʹ���е����Ǳ��
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSA_Data.SatUsedList[NMEA_MsgBlock-2]=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GSA_Data.SatUsedList[NMEA_MsgBlock-2]+=(SBuf-'0');
                break;
            }
            break;
        case 14:        // <4> PDOP λ�þ������� 0.5~99.9
            GPS_GSA_Data.PDOP[NMEA_MsgBlockDatIndex]=SBuf;
            break;
        case 15:        // <5> HDOP ˮƽ�������� 0.5~99.9
            GPS_GSA_Data.HDOP[NMEA_MsgBlockDatIndex]=SBuf;
            break;
        case 16:        // <6> VDOP ��ֱ�������� 0.5~99.9
            GPS_GSA_Data.VDOP[NMEA_MsgBlockDatIndex]=SBuf;
            break;
        }
        NMEA_MsgBlockDatIndex++;  // ���ֶ��ַ���� +1
    }
}

static void ParserGPGSV(char SBuf)
{
    switch (SBuf)
    {
    case '*':               // ������ݽ���, ����2λУ��ֵ
        NMEA_Start=0;
        break;
    case ',':               // ,�ָ���, �ֶ� +1, �ֶ��ڲ��ַ���Ź���
        NMEA_MsgBlock++;
        NMEA_MsgBlockDatIndex=0;
        break;
    default:
        switch (NMEA_MsgBlock)
        {
            /*
            case 0:         // <1> GSV��������
                break;
                */
        case 1:         // <2> ����GSV�ı��
            if (SBuf=='1') SateInfoIndex=0;
            //��������һ�� GSV ��� ���ж�������Ϣ���¿�ʼ
            break;
        case 2:         // <3> �ɼ����ǵ����� 00~12
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSV_Data.SatInView=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GSV_Data.SatInView+=(SBuf-'0');
                break;
            }
            break;
        case 3:         // <4> ���Ǳ�� 01~32
        case 7:
        case 11:
        case 15:
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSV_Data.SatInfo[SateInfoIndex].SatID=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GSV_Data.SatInfo[SateInfoIndex].SatID+=(SBuf-'0');
                SateInfoIndex++;
                break;
            }
            break;
        case 4:         // <5>�������� 00~90 ��
        case 8:
        case 12:
        case 16:
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].Elevation=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].Elevation+=(SBuf-'0');
                break;
            }
            break;
        case 5:         // <6>���Ƿ�λ�� 000~359 ��
        case 9:
        case 13:
        case 17:
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].Azimuth=(SBuf-'0')*100;
                break;
            case 1:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].Azimuth+=(SBuf-'0')*10;
                break;
            case 2:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].Azimuth+=(SBuf-'0');
                break;
            }
            break;
        case 6:         // <7>Ѷ�������� C/No 00~99
        case 10:
        case 14:
        case 18:
            switch (NMEA_MsgBlockDatIndex)
            {           // ǰ��0Ҳ�����, ת������ֵ��
            case 0:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].SNR=(SBuf-'0')*10;
                break;
            case 1:
                GPS_GSV_Data.SatInfo[SateInfoIndex-1].SNR+=(SBuf-'0');
                break;
            }
            break;
        }
        NMEA_MsgBlockDatIndex++;  // ���ֶ��ַ���� +1
    }
}

u8 GPS_Parser(char SBuf)
{
    u8 i;
    GPS_Parse_Status=0;

    if (NMEA_Start)
    {               // ��������$��ʼ�� NMEA ���, ����NMEA ��������:
        if (NMEA_TypeParsed)
        {           // NMEA ������ͽ������, �������͵��ý�������
            switch (NMEA_MsgType)
            {
            case NMEA_GPGGA:
                ParserGPGGA(SBuf);
                break;
            case NMEA_GPGSA:
                ParserGPGSA(SBuf);
                break;
            case NMEA_GPGSV:
                ParserGPGSV(SBuf);
                break;
            case NMEA_GPRMC:
                ParserGPRMC(SBuf);
                break;
            default:    //�޷�ʶ��ĸ�ʽ, ��λ
                NMEA_Start=0;
                NMEA_TypeParsed=0;
                NMEA_MsgType=NMEA_NULL;
                NMEA_MsgTypeIndex=1;
            }
        }
        else
        {           // NMEA �������δ����, �������͵��ý�������
            switch (SBuf)
            {
            case ',':       // NMEA ��������ֶν���,��ʼ�ж�
                // GPS ���˳�� - 0
                if (NMEA_MsgTypeBuff[3]=='G'&&NMEA_MsgTypeBuff[4]=='A')
                {
                    //��ʼ����������

                    //��ʼ����λ��Ϣ����
                    //GPS_GGA_Data.PositionFix=0x00;
                    GPS_GGA_Data.SatUsed=0x00;
                    NMEA_MsgType=NMEA_GPGGA;
                }

                // GPS ���˳�� - 1
                if (NMEA_MsgTypeBuff[3]=='S'&&NMEA_MsgTypeBuff[4]=='A')
                {
                    //��ʼ��ʹ���е������б�
                    for (i=0;i<12;i++)
                    {
                        GPS_GSA_Data.SatUsedList[i]=0x00;
                    }
                    //��ʼ����������
                    for (i=0;i<5;i++)
                    {
                        GPS_GSA_Data.HDOP[i]=0x00;
                        GPS_GSA_Data.VDOP[i]=0x00;
                        GPS_GSA_Data.PDOP[i]=0x00;
                    }
                    //GPS_GSA_Data.Mode=0x00;
                    //GPS_GSA_Data.Mode2=0x00;

                    //����GSV���������, ���г�ʼ�����ܷ��� GSV��,
                    //����ᵼ��ǰ��������������ݱ���ʼ����
                    //��ʼ�������źŷ�λ������
                    for (i=0;i<12;i++)
                    {
                        GPS_GSV_Data.SatInfo[i].SatID=0x00;
                        GPS_GSV_Data.SatInfo[i].SNR=0x00;
                        GPS_GSV_Data.SatInfo[i].Elevation=0x00;
                        GPS_GSV_Data.SatInfo[i].Azimuth=0x0000;
                    }
                    //GPS_GSV_Data.SatInView=0x00;
                    NMEA_MsgType=NMEA_GPGSA;
                }

                // GPS ���˳�� - 2,3,4
                if (NMEA_MsgTypeBuff[4]=='V')
                {
                    NMEA_MsgType=NMEA_GPGSV;
                }

                // GPS ���˳�� - 5
                if (NMEA_MsgTypeBuff[4]=='C')
                {
                    //GPS_RMC_Data.Status='-';
                    //��ʼ����γ�����ݺ��ٶ�,����
                    GPS_RMC_Data.LatitudeD=0x00;
                    GPS_RMC_Data.LatitudeM=0x00;
                    GPS_RMC_Data.LatitudeS=0x0000;
                    GPS_RMC_Data.NS='-';

                    GPS_RMC_Data.LongitudeD=0x00;
                    GPS_RMC_Data.LongitudeM=0x00;
                    GPS_RMC_Data.LongitudeS=0x0000;
                    GPS_RMC_Data.EW='-';

                    GPS_RMC_Data.Speed=0x0000;
                    GPS_RMC_Data.Course=0x0000;

                    NMEA_MsgType=NMEA_GPRMC;
                }
                //�˴������������, ����䲻��ʶ��, ��NMEA_MsgTypeΪNULL������,
                //��תΪ�������ͽ���ʱ����ת���޷�ʶ��ĸ�ʽ, ����λ
                NMEA_TypeParsed=1;
                NMEA_MsgTypeIndex=1;
                NMEA_MsgBlock=0;
                NMEA_MsgBlockDatIndex=0;
                break;
            case '*':
                NMEA_Start=0;
                //GPSģ���ϵ�ʱ���
                //$PSRF Model Name : J3S31_DGCB1_496 *45
                //$PSRF *321.3*30
                //$PSRF*17
                //$PSRF*17
                //$PSRF Product by J communications Co., Ltd *4C
                //$PSRF Revision by Young Wook *69
                //$PSRF www.jcomco.com *06
                //�����ж�ʧЧ,
                break;
            default:        //���ڵ�һ���ֶ���, ��������
                NMEA_MsgTypeBuff[NMEA_MsgTypeIndex]=SBuf;
                NMEA_MsgTypeIndex++;
                if (NMEA_MsgTypeIndex>5) NMEA_Start=0;
                // NMEA ���ͳ��� 5 ���ַ�, (����Խ��, ��������)
                // ���жϲ��������� NMEA ���, ���Թ��˾�.
            }
        }
    }
    else
    {               //δ������$, ѭ�����ղ��ж� ֱ�� $
        if (SBuf=='$')
        {           //���յ�$, ��һ���ַ���Ϊ�����ж��ַ�, �Ƚ�����ر�����ʼ��
            NMEA_Start = 1;         //�´ε��������NMEA ��������:
            NMEA_MsgTypeIndex = 0;  //��ͷ���GPS�����ַ�������
            NMEA_TypeParsed = 0;
            NMEA_MsgType = NMEA_NULL;
            NMEA_MsgBlock = 0;
            NMEA_MsgBlockDatIndex = 0;
        }
    }

    return GPS_Parse_Status;
}

/*---------------------------------------------
| void GPS_View_Basic(void)
| ������ͼ: ������Ϣ
| ��ص��� ��ѹ ʹ�������� ��λ��ʽͼ�� ����ʱ��
| γ��
| ����
| ʹ����Ļ�ռ�: 160x40 (0-159),(0-9)
---------------------------------------------*/
void GPS_View_Basic(u8 ucLogStatus)
{
    char cStr[20];      //�ַ���

    LCD_FillBase00();   //�������Ļ

    LCD_Icon(0,0,ICON_INDEX_BAT25,GRAY_BLACK,GRAY_WHITE);     //��ر�־

    LCD_SetGray(GRAY_BLACK,GRAY_WHITE);
    sprintf(cStr,"%2d/%2d",GPS_GGA_Data.SatUsed,GPS_GSV_Data.SatInView);
    LCD_PutStr08(16,0,cStr);                //����ʹ����

    // ��λģʽͼ��
    switch(GPS_GGA_Data.PositionFix)
    {
    case '0':
        LCD_Icon(41,0,ICON_INDEX_FIX_G,GRAY_GB,GRAY_WHITE); // δ��λ ��ɫ G
        break;
    case '1':
        LCD_Icon(41,0,ICON_INDEX_FIX_G,GRAY_BLACK,GRAY_WHITE); // �ǲ�ֶ�λ ��ɫ G
        break;
    case '2':
        LCD_Icon(41,0,ICON_INDEX_FIX_DG,GRAY_BLACK,GRAY_WHITE); // ��ֶ�λ ��ɫ DG
        break;
    case '6':
        LCD_Icon(41,0,ICON_INDEX_FIX_DR,GRAY_BLACK,GRAY_WHITE); // ���� DR
        break;
    default:
        LCD_Icon(41,0,ICON_INDEX_FIX_NULL,GRAY_GB,GRAY_WHITE); // ����
        break;           
    }

    switch(GPS_GSA_Data.Mode2)
    {
    case '1':
        LCD_Icon(55,0,ICON_INDEX_FIX_3D,GRAY_GB,GRAY_WHITE); // δ��λ ��ɫ 3D
        break;
    case '2':
        LCD_Icon(55,0,ICON_INDEX_FIX_2D,GRAY_BLACK,GRAY_WHITE); // 2D
        break;
    case '3':
        LCD_Icon(55,0,ICON_INDEX_FIX_3D,GRAY_BLACK,GRAY_WHITE); // 3D
        break;
    default:
        LCD_Icon(55,0,ICON_INDEX_FIX_NULL,GRAY_GB,GRAY_WHITE); // ����
        break;           
    }

    switch(ucLogStatus)
    {
    case LOG_START:
        LCD_Icon(69,0,ICON_INDEX_LOG_START,GRAY_BLACK,GRAY_WHITE); 
        break;
    case LOG_STOP:
        LCD_Icon(69,0,ICON_INDEX_LOG_START,GRAY_GB,GRAY_WHITE); // 2D
        break;          
    }

    //-------------------------------------------------------
    //��ʾʱ��  0/00 00:00:00
    sprintf(cStr,"%2d/%02d %2d:%02d:%02d",GPS_RMC_Data.UTCDateTime[1],GPS_RMC_Data.UTCDateTime[2],
            GPS_RMC_Data.UTCDateTime[3],GPS_RMC_Data.UTCDateTime[4],GPS_RMC_Data.UTCDateTime[5]);
    LCD_SetGray(GRAY_BLACK,GRAY_WHITE);
    LCD_PutStr08(89,0,cStr);


    //-------------------------------------------------------
    //������λ��Чʱ, ��γ�Ȳ���ʾ�ɺ�ɫ, ������ʾ��ɫ
    //û������ʱ������ʾ
    switch (GPS_RMC_Data.Status)
    {
    case 'A':
        LCD_SetGray(GRAY_BLACK,GRAY_WHITE);
        break;
    case 'V':
        LCD_SetGray(GRAY_GB,GRAY_WHITE);
        break;
    default:
        LCD_SetGray(GRAY_WHITE,GRAY_BLACK);
    }

    //-------------------------------------------------------
    // ��ʾγ�� Latitude  dd'mm'ss.sss"
    sprintf(cStr,"γ�� %02d\x01%02d'%06.3f\"",GPS_RMC_Data.LatitudeD,GPS_RMC_Data.LatitudeM,
            (float)GPS_RMC_Data.LatitudeS*60/10000);
    LCD_PutStr16(0,2,cStr);

    switch (GPS_RMC_Data.NS) //��γ ��γ��־
    {
    case 'N':
        LCD_PutStr16(143,2,"��");
        break;
    case 'S':
        LCD_PutStr16(143,2,"��");
        break;
    default:
        LCD_PutStr16(143,2,"--");
        break;
    }

    //-------------------------------------------------------
    // ��ʾ���� Longitude ddd'mm'ss.sss"
    sprintf(cStr,"����%03d\x01%02d'%06.3f\"",GPS_RMC_Data.LongitudeD,GPS_RMC_Data.LongitudeM,
            (float)GPS_RMC_Data.LongitudeS*60/10000);
    LCD_PutStr16(0,6,cStr);

    switch (GPS_RMC_Data.EW) //����
    {
    case 'W':
        LCD_PutStr16(143,6,"��");
        break;
    case 'E':
        LCD_PutStr16(143,6,"��");
        break;
    default:
        LCD_PutStr16(143,6,"--");
        break;
    }
}

/*---------------------------------------------
| void GPS_View_SatInfo(void)
| ������Ϣ��ͼ: ���������ź���״ͼ�ͷ�λ����ͼ
| ʹ����Ļ�ռ�: 160x60 (0-159),(10-24)
---------------------------------------------*/
void GPS_View_SatInfo(void)
{
    u8 i,j;
    u8 SatUsed;
    char tempStr[10];

    LCD_SetGray(GRAY_BLACK,GRAY_GW);

    // ��ʾ�����ź���״ͼ
    // ��Ļ��Χ (0-96),page(13,24)
    for (i=0;i<12;i++)
    {
        SatUsed=0;
        for (j=0;j<12;j++)
        {
            if (GPS_GSV_Data.SatInfo[i].SatID==GPS_GSA_Data.SatUsedList[j])
                SatUsed=1;
        }
        LCD_GPS_SNR(2+i*8,15,GPS_GSV_Data.SatInfo[i].SatID,GPS_GSV_Data.SatInfo[i].SNR,SatUsed);
    }

    LCD_Icon(0,13,ICON_INDEX_DOP_DOP,GRAY_BLACK,GRAY_GW);
    LCD_Icon(17,13,ICON_INDEX_DOP_P,GRAY_BLACK,GRAY_GW);
    sprintf(tempStr,"%.1f",atof(GPS_GSA_Data.PDOP));
    LCD_PutStr08(24,13,tempStr);
    LCD_Icon(44,13,ICON_INDEX_DOP_H,GRAY_BLACK,GRAY_GW);
    sprintf(tempStr,"%.1f",atof(GPS_GSA_Data.HDOP));
    LCD_PutStr08(52,13,tempStr);
    LCD_Icon(72,13,ICON_INDEX_DOP_V,GRAY_BLACK,GRAY_GW);
    sprintf(tempStr,"%.1f",atof(GPS_GSA_Data.VDOP));
    LCD_PutStr08(80,13,tempStr);

    LCD_GPS_SatInfo(&GPS_GSV_Data,GPS_GSA_Data.SatUsedList);
}

void GPS_View_Log(void)
{
    char tempStr[50],tempStr2[50];
    u16 i;

    LCD_SetGray(GRAY_BLACK,GRAY_GW);

    i=LOG_GetDataMapIndex();
    LCD_GUI_ProgressBar(56,13,100,i*100/6368,0);
    LCD_PutStr12(2,13,"���ÿռ�:");

    sprintf(tempStr,"FLASH ADDR:%04d[%d]/6368 (%4.1f%%)",i,LOG_GetDataIndexInPage(),(float)i*100/6368);
    LCD_PutStr08(1,17,tempStr);

    //
    LOG_GetLastRecord(tempStr,tempStr2);

    LCD_GUI_Rect(2,19,157,24,0);
    LCD_PutStr08(25,20,tempStr);
    LCD_PutStr08(25,22,tempStr2);

    LCD_IconSave(5,20);
}

/*---------------------------------------------
| void GPS_View_Speed(void)
| ������Ϣ��ͼ: �����ٶ�,����,����,��̵���Ϣ
| ʹ����Ļ�ռ�: 160x60 (0-159),(10-24)
---------------------------------------------*/
void GPS_View_Speed(void)
{
    char tempStr[50];

    LCD_SetGray(GRAY_BLACK,GRAY_GW);

    LCD_PutStr12(0,14,"����");

    sprintf(tempStr,"%5.1f\x01",(float)GPS_RMC_Data.Course/10);
    LCD_PutStr12(124,14,tempStr);

    LCD_GPS_Course(27,13,GPS_RMC_Data.Course/10%360);

    sprintf(tempStr,"ʱ��:%.1fKM",((double)GPS_RMC_Data.Speed/10)*1.852);  //12*6=72pixel
    LCD_PutStr12(0,18,tempStr);

    sprintf(tempStr,"����:%.1f��",atof(GPS_GGA_Data.Altitude));
    LCD_PutStr12(80,18,tempStr);

    sprintf(tempStr,"���:%.2f����",0.5);
    LCD_PutStr12(0,22,tempStr);
}

void GPS_GetLogData(u8 * LogBuf)
{
    u8 CheckSum=0x00,i;

    if (GPS_RMC_Data.Status=='A')
        *LogBuf='$';
    else
        *LogBuf='-';
    CheckSum^=*LogBuf;


    for (i=0;i<6;i++)
    {
        LogBuf++;
        *LogBuf=GPS_RMC_Data.UTCDateTime[i];
        CheckSum^=*LogBuf;
    }

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LatitudeD;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LatitudeM;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LatitudeS>>8;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LatitudeS;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.NS;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LongitudeD;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LongitudeM;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LongitudeS>>8;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.LongitudeS;
    CheckSum^=*LogBuf;

    LogBuf++;
    *LogBuf=GPS_RMC_Data.EW;
    CheckSum^=*LogBuf;

}


void GPS_GetTime(u8* ucDataTime)        // ���� GPS ���ں�ʱ��
{
    u8 i;
    for (i=0;i<6;i++)
    {
        *ucDataTime=GPS_RMC_Data.UTCDateTime[i];
        ucDataTime++;
    }

}
