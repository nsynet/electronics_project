
#include "USART.h"

bool UASRT_Signal = FALSE;

static u8 USART_RxBuf[RX_BUFFER_LEN];
static u8 USART_TxBuf[TX_BUFFER_LEN];
static bool UASRT_UsePage = TRUE;
static u32 UASRT_RecvDataLen = 0;
static bool USART_TxBusy = FALSE;
static u32 USART_SendDataLen = 0;
static OS_TID rx_task;
static OS_TID tx_task;

static USART_TypeDef *UsartBase = USART2;

void USART_Initial(u32 baudrate)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;     
    GPIO_InitTypeDef GPIO_InitStructure; 

    // GPIOA clock enable 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
                                         
    //Enable clock for UsartBase
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);   
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);   
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);    

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);       

    // Enable the UsartBase Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQChannel;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //Config UsartBase
    USART_InitStructure.USART_BaudRate = baudrate ;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  
    
    USART_Init(UsartBase, &USART_InitStructure);
    
    USART_ITConfig(UsartBase, USART_IT_RXNE, ENABLE); 

    USART_Cmd(UsartBase, ENABLE);   
}

bool USART_AnalyzePackage(u8 data)
{
	static u8 USART_LastByte = 0;
	static bool USART_BeginFlag = FALSE;
	static bool USART_CtrlFlag = FALSE;
	static u16 USART_RevOffset = 0;
    static u8 CheckSum = 0;

	if( (data==USART_FRAMEHEAD)&&(USART_LastByte==USART_FRAMEHEAD) )
	{
		//RESET
		USART_RevOffset = 0;
		USART_BeginFlag = TRUE;
		USART_LastByte = data ;
		return FALSE;
	}
	if( (data==USART_FRAMETAIL)&&(USART_LastByte==USART_FRAMETAIL)&&USART_BeginFlag )
	{
		//Signal
		USART_RevOffset--;//得到出去头尾和控制符的数据的个数
		UASRT_RecvDataLen = USART_RevOffset;
        CheckSum -= USART_FRAMETAIL;
        CheckSum -= USART_RxBuf[UASRT_RecvDataLen -1];
        USART_LastByte = data;
        USART_BeginFlag = FALSE; 
        if(CheckSum == USART_RxBuf[UASRT_RecvDataLen -1])
        {                          		   		
    		CheckSum = 0;
            UASRT_RecvDataLen -- ;
            UASRT_Signal = TRUE;
            //关闭发送中断
            USART_ITConfig(UsartBase, USART_IT_RXNE, DISABLE);
            //通知接收完成
            isr_evt_set (RX_EVENT, rx_task);      
    	 	return TRUE;
        }
        CheckSum = 0;                    
        return FALSE;
	}
	USART_LastByte = data ;
	if(USART_BeginFlag)
	{
		if(USART_CtrlFlag)
		{
		    USART_RxBuf[USART_RevOffset++] = data;
            CheckSum += data;
			USART_CtrlFlag = FALSE;
			USART_LastByte = USART_FRAMECTRL;
		}
		else if(data == USART_FRAMECTRL)
		{
		    USART_CtrlFlag = TRUE;
		}
		else
		{
		    USART_RxBuf[USART_RevOffset++] = data;
            CheckSum += data;
		}
	}

	return FALSE;
}

bool USART_FillRxBuff(u8 data)
{
    static u16 USART_RevOffset = 0;

    USART_RxBuf[USART_RevOffset++] = data;    

    if(USART_RevOffset == UASRT_RecvDataLen)
    {
        
        USART_RevOffset = 0;
        UASRT_Signal = TRUE; 
        //关闭接收中断
        USART_ITConfig(UsartBase, USART_IT_RXNE, DISABLE);     
        //通知接收完成
        isr_evt_set (RX_EVENT, rx_task);      
        return TRUE;
    } 
    return FALSE;
}
/*================================================================

*/
bool USART_RxCallBack(u8 data)
{
    if(UASRT_UsePage)
    {
        return USART_AnalyzePackage(data);    
    }
    else
    {
        return USART_FillRxBuff(data);        
    }
}

void USART_TxCallBack(void)
{
	static u16 TxCnt = 0;
	
	if(USART_SendDataLen <= TxCnt)
	{
		TxCnt = 0;
		//关闭发送中断
		USART_SendDataLen = 0;
		USART_TxBusy = FALSE;      		
        USART_ITConfig(UsartBase, USART_IT_TXE, DISABLE);
        //通知发送完成
        isr_evt_set (TX_EVENT, tx_task);
        return ;
	}
    USART_SendData(UsartBase, USART_TxBuf[TxCnt++]);
}

/////////////////////////////////////////////////////////////////////////
//User Interface
/////////////////////////////////////////////////////////////////////////


OS_RESULT USART_SendBuf(u8 *buf,u16 n, u16 timeout)
{
    if(USART_TxBusy)
		return OS_R_EVT;

    USART_TxBusy = TRUE;

    memcpy(USART_TxBuf,buf,n);

    USART_SendDataLen = n;

    tx_task = os_tsk_self ();
    os_evt_clr (TX_EVENT, tx_task);

    //使能发送中断
    USART_ITConfig(UsartBase, USART_IT_TXE, ENABLE);

    //等待发送完成
    return os_evt_wait_or (TX_EVENT, timeout);


}

u16 USART_RecvBuf(u8 *buf, u16 n , u16 timeout)
{             
    UASRT_RecvDataLen = n;
    UASRT_UsePage = FALSE;

    rx_task = os_tsk_self ();
    os_evt_clr (RX_EVENT, rx_task);

    //开接收中断
    USART_ITConfig(UsartBase, USART_IT_RXNE, ENABLE);
    
    //等待接收完成 
    os_evt_wait_or (RX_EVENT, timeout);

    memcpy(buf,USART_RxBuf,UASRT_RecvDataLen);
    return UASRT_RecvDataLen;    
}
  
OS_RESULT USART_SendPackage(s16 packid, u8 *data,u16 n , u16 timeout)
{
	s16 i;
	u8 *pBuf;
    u8 CheckSum = 0;

	if(USART_TxBusy)
		return OS_R_EVT;

    USART_TxBusy = TRUE;

    tx_task = os_tsk_self ();
    os_evt_clr (TX_EVENT, tx_task); 	

	pBuf = USART_TxBuf;
    *pBuf++ = USART_FRAMEHEAD;
	*pBuf++ = USART_FRAMEHEAD;

    if(packid >= 0)
    {
        //Set ID
        if (packid==USART_FRAMECTRL||packid==USART_FRAMEHEAD||packid==USART_FRAMETAIL)
    	{
    		*pBuf++ = USART_FRAMECTRL;
    	}
        *pBuf++ = packid;
        CheckSum += packid;
    }

	for (i=0;i<n;i++)
	{
		if (data[i]==USART_FRAMECTRL||data[i]==USART_FRAMEHEAD||data[i]==USART_FRAMETAIL)
		{
			*pBuf++ = USART_FRAMECTRL;
		}
		*pBuf++ = data[i];
        CheckSum += data[i];
	}

    //校验和
    if (CheckSum==USART_FRAMECTRL||CheckSum==USART_FRAMEHEAD||CheckSum==USART_FRAMETAIL)
	{
		*pBuf++ = USART_FRAMECTRL;
	}
    *pBuf++ = CheckSum;

	//Send Tail USART_FRAMETAIL USART_FRAMETAIL
	*pBuf++ = USART_FRAMETAIL;
	*pBuf++ = USART_FRAMETAIL;   

	USART_SendDataLen = pBuf - USART_TxBuf;
	//使能发送中断
	USART_ITConfig(UsartBase, USART_IT_TXE, ENABLE);
    //等待发送完成做
    return os_evt_wait_or (TX_EVENT, timeout);   
}

u16 USART_RecvPackage(u8 *packid, u8 *data , u16 timeout)
{
    
    rx_task = os_tsk_self ();
    os_evt_clr (RX_EVENT, rx_task);

    UASRT_UsePage = TRUE;

    //开接收中断
    USART_ITConfig(UsartBase, USART_IT_RXNE, ENABLE);
    
    //等待解包完成 
    if(os_evt_wait_or (RX_EVENT, timeout) == OS_R_TMO)
         return 0;        

    //除去ID，将剩下的数据拷贝到用户缓冲区
    if(packid)
    {
        *packid = USART_RxBuf[0];
        memcpy(data,USART_RxBuf+1,UASRT_RecvDataLen-1);
        return UASRT_RecvDataLen - 1; 
    }
    else
    {
        memcpy(data,USART_RxBuf,UASRT_RecvDataLen);
        return UASRT_RecvDataLen; 
    }      
}

u16 USART_SendPackageWithACK(u8 *txdata,u16 txn, u8 *ackdata, u16 timeout)
{  
    u8 static PackageID = 0;
    u8 i,ackid;
    u16 datalen;
    //重试两次
    if(timeout != 0xFFFF)
        timeout >>= 1;

    for(i = 0;i < 2;i++)
    {
        //发送封包
        PackageID++;
        USART_SendPackage(PackageID,txdata,txn,0xFFFF);                         
        //等待ACK
        datalen = USART_RecvPackage(&ackid,ackdata,timeout);
        if(datalen)
        {
            if(PackageID == ackid)
                return datalen;               
        }
    }
    return 0;  
}

u16 USART_RecvPackageWithACK(u8 *rxdata, u8 *ackdata, u16 ackn, u16 timeout)
{
    u8 rxid;
    u16 datalen;
    
    datalen = USART_RecvPackage(&rxid,rxdata,timeout);
    if(datalen)
    {
        USART_SendPackage(rxid,ackdata,ackn,0xFFFF);      
        return datalen;
    }
    return 0;      
}

void USART2_IRQHandler(void)
{
    if(USART_GetITStatus(UsartBase, USART_IT_RXNE) != RESET)
    {
        USART_RxCallBack(USART_ReceiveData(UsartBase));
    }
        
    if(USART_GetITStatus(UsartBase, USART_IT_TXE) != RESET)
    {   
        USART_TxCallBack();   
    }
}




