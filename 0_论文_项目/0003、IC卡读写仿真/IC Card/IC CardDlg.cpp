// IC CardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IC Card.h"
#include "IC CardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CICCardDlg dialog

CICCardDlg::CICCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CICCardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CICCardDlg)
	m_id = _T("");
	m_money = 0.0f;
	m_money2 = 0.0f;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CICCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CICCardDlg)
	DDX_Control(pDX, IDC_LED, m_LED);
	DDX_Control(pDX, IDC_OPENCOM, m_OpenCom);
	DDX_Control(pDX, IDC_COMBO1, m_ComNum);
	DDX_Control(pDX, IDC_MSCOMM, m_comm);
	DDX_Text(pDX, IDC_ID, m_id);
	DDX_Text(pDX, IDC_MONEY, m_money);
	DDX_Text(pDX, IDC_ADD_DEC, m_money2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CICCardDlg, CDialog)
	//{{AFX_MSG_MAP(CICCardDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPENCOM, OnOpencom)
	ON_BN_CLICKED(IDC_WR, OnWr)
	ON_BN_CLICKED(IDC_RD, OnRd)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEC, OnDec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CICCardDlg message handlers

BOOL CICCardDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_hIcon1 = AfxGetApp()->LoadIcon(IDI_ICON_OFF);
	m_hIcon2 = AfxGetApp()->LoadIcon(IDI_ICON_RED);
//初始化代码
	CString strSettings="9600,n,8,1";
	//m_comm.SetCommPort(2);	//选择串口号
	m_ComNum.SetCurSel(0);
	m_comm.SetInBufferSize(1204);//输入缓冲
	m_comm.SetOutBufferSize(1204);//输出缓冲	
	if(!m_comm.GetPortOpen())//打开串口
	{
		m_LED.SetIcon(m_hIcon1);//红灯亮
		m_OpenCom.SetWindowText("关闭串口");
		m_comm.SetPortOpen(TRUE);//打开串口
	}
	else
	{
		m_LED.SetIcon(m_hIcon2);//红灯灭
		m_OpenCom.SetWindowText("打开串口");
		m_comm.SetPortOpen(FALSE);//关闭串口
	}
	m_comm.SetInputMode(1);//设置输入方式为二进制方式 
	m_comm.SetSettings(strSettings);//参数	
	m_comm.SetRThreshold(9); //一个字符引发一个事件
	m_comm.SetInputLen(0);
//=====end========		

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CICCardDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CICCardDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BEGIN_EVENTSINK_MAP(CICCardDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CICCardDlg)
	ON_EVENT(CICCardDlg, IDC_MSCOMM, 1 /* OnComm */, OnOnCommMscomm, VTS_NONE)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CICCardDlg::OnOnCommMscomm() //数据接收
{
	// TODO: Add your control notification handler code here
/*	
	
	UpdateData(TRUE);
	VARIANT variant_inp;//接收缓冲暂存
	COleSafeArray safearray_inp;
	LONG len,k;
	
	CString strtemp;
	m_getstr="";
	if(m_comm.GetCommEvent()==2)//事收件值为2表示接缓冲区内有字符
	{
		variant_inp=m_comm.GetInput();//读缓冲区
		safearray_inp=variant_inp;//VARIANT型变量转换为ColeSafeArray型变量
		len=safearray_inp.GetOneDimSize();//得到有效数据长度
		
		for(k=0;k<len;k++)
		safearray_inp.GetElement(&k,rxdata+k);//转换为BYTE型数组

		for(k=0;k<len;k++)
		{
			BYTE bt=*(char *)(rxdata+k);
			strtemp.Format("%02x",bt);
			//if(m_bStopdis)
			{
				//m_getstr=strtemp;
				m_getstr=m_getstr+' '+strtemp;
			}
		}
		
		m_comm.SetInBufferCount(0);	
		DealWithData();//外理数据		
	}
	
	*/
	VARIANT variant_inp;//接收缓冲暂存
	COleSafeArray safearray_inp;
	LONG len,k;
	
	if(m_comm.GetCommEvent()==2)//事收件值为2表示接缓冲区内有字符
	{
		variant_inp=m_comm.GetInput();	//读缓冲区
		safearray_inp=variant_inp;		//VARIANT型变量转换为ColeSafeArray型变量
		len=safearray_inp.GetOneDimSize();//得到有效数据长度
		
		for(k=0;k<len;k++)	//下面有一个rxdata成员变量，要定义一下,它是一个BYTE数组，其实就是unsigned char 数组
			safearray_inp.GetElement(&k,rxdata+k);	//转换为BYTE型数组

		if(rxdata[0]==0x67)	//写卡完毕
		{
			MessageBox("写卡完毕");
		}
	
	}


}

void CICCardDlg::OnOpencom() //打开和关闭串口控制
{
	// TODO: Add your control notification handler code here
	m_bOpenCom=!m_bOpenCom;
	if(m_bOpenCom)//如果已经打开串口,则关闭串口
	{		
		m_LED.SetIcon(m_hIcon1);//红灯灭
		m_OpenCom.SetWindowText("打开串口");
		m_comm.SetPortOpen(FALSE);//关闭串口
		//UpdateData(FALSE);	
	}else //如果已经关闭串口,则打开串口
	{	
		m_comm.SetCommPort(m_ComNum.GetCurSel()+1);//选择串口号
		m_LED.SetIcon(m_hIcon2);//红灯亮
		m_OpenCom.SetWindowText("关闭串口");
		m_comm.SetPortOpen(TRUE);//打开串口
	//	UpdateData(FALSE);
	//	UpdateStatusEdit();
	}	
}

void CICCardDlg::OnWr() //写卡函数
{
	// TODO: Add your control notification handler code here
	CByteArray hexdata;
	CByteArray strGet;
	int i;
	unsigned int uimoney;
	CString str;
	int flag;
	UpdateData(TRUE);//更新数据
	str=m_id;
	for(i=0;i<str.GetLength();i++)
	{	
		if(!((str[i]<='9' &&str[i]>='0') || (str[i]<='f' &&str[i]>='a') || (str[i]<='F' &&str[i]>='A') ))
		{
			flag=1;
		}else flag=0;
	}
		if(flag==1)
		{
			MessageBox("请输入十位0~F的\"数字\"");			
		}
		else
		{
			if(str.GetLength()!=10)MessageBox("请输入十位0~F的\"数字\"");
			else
			{
				
			
					hexdata.SetSize(9);				

					hexdata[0]=0x67;
					hexdata[1]=7;

					hexdata[2]=ConvertHexChar(str[0])*16+ConvertHexChar(str[1]);		
					hexdata[3]=ConvertHexChar(str[2])*16+ConvertHexChar(str[3]);
					hexdata[4]=ConvertHexChar(str[4])*16+ConvertHexChar(str[5]);	
					hexdata[5]=ConvertHexChar(str[6])*16+ConvertHexChar(str[7]);	
					hexdata[6]=ConvertHexChar(str[8])*16+ConvertHexChar(str[9]);	
				
					uimoney=(unsigned int)(m_money*10);

					hexdata[7]=uimoney>>8;
					hexdata[8]=uimoney&0xff;
					//hexdata[13]=strGet[10];
				//	m_Port.SetOutBufferCount(0);
					m_comm.SetOutput(COleVariant(hexdata));//以十六进制格式化发送
			}

		}	
}

char CICCardDlg::ConvertHexChar(char ch)
{
	if('0'<=ch&&ch<='9')
		return ch-'0';
	else if('A'<ch&&ch<='F')
		return ch-'A'+10;
	else if('a'<ch&&ch<='f')
		return ch-'a'+10;
	else return -1;
}

void CICCardDlg::OnRd() 
{
	// TODO: Add your control notification handler code here
	CByteArray hexdata;
	//int len=String2Hex(m_strsend,hexdata);
	hexdata.SetSize(9);
	hexdata[0]=0x45;
	hexdata[1]=7;

	m_comm.SetOutput(COleVariant(hexdata));//以十六进制格式化发送	
}

void CICCardDlg::DealWithData()
{
		CString strID;
		CString strtemp;
		strtemp="";
		char temp_h,temp_l;

		if(rxdata[0]==0x45)	//读卡完毕
		{
				temp_h=Num2Char(rxdata[1]>>4);				
				temp_l=Num2Char(rxdata[1]&0x0f);
				strtemp+=temp_h;				
				strtemp+=temp_l;

				temp_h=Num2Char(rxdata[2]>>4);				
				temp_l=Num2Char(rxdata[2]&0x0f);
				strtemp+=temp_h;
				strtemp+=temp_l;

				temp_h=Num2Char(rxdata[3]>>4);				
				temp_l=Num2Char(rxdata[3]&0x0f);
				strtemp+=temp_h;
				strtemp+=temp_l;

				temp_h=Num2Char(rxdata[4]>>4);				
				temp_l=Num2Char(rxdata[4]&0x0f);
				strtemp+=temp_h;
				strtemp+=temp_l;

				temp_h=Num2Char(rxdata[5]>>4);				
				temp_l=Num2Char(rxdata[5]&0x0f);

				strtemp+=temp_h;
				strtemp+=temp_l;

				m_id=strtemp;	//卡标识
				m_money=(float)(rxdata[6]*256+rxdata[7])/10;	//钱
				
		}else if(rxdata[0]==0x67)	//写卡完毕
			{
				MessageBox("写卡完毕");
				OnRd();
			}else if(rxdata[0]==0xdd)	//写卡错误
			{
				MessageBox("写卡错误,请确认IC卡没有损坏!");
			}

		UpdateData(TRUE);
		
}

char CICCardDlg::Num2Char(unsigned char ch)
{
	if(ch<=9)
 		return (ch+'0');
 	else return (ch-10+'a');
}

void CICCardDlg::OnAdd() //充值函数
{
	// TODO: Add your control notification handler code here
	CByteArray hexdata;

	unsigned int uimoney;

	UpdateData(TRUE);//更新数据

	if(m_money2<=6553.5 && m_money2>=0)
	{

		hexdata.SetSize(9);				

		hexdata[0]=0xab;	//充钱命令
		hexdata[1]=7;	

		uimoney=(unsigned int)(m_money2*10);

		hexdata[7]=uimoney>>8;
		hexdata[8]=uimoney&0xff;
	
		m_comm.SetOutput(COleVariant(hexdata));//以十六进制格式化发送	
	}else
	{
		MessageBox("请入一个0~6553.5的数!");
	}
		
}

void CICCardDlg::OnDec() 
{
	// TODO: Add your control notification handler code here
	CByteArray hexdata;
	unsigned int uimoney;

	UpdateData(TRUE);//更新数据

		if(m_money2<=6553.5 && m_money2>=0)
		{
				hexdata.SetSize(9);				

				hexdata[0]=0x89;	//消费命令
				hexdata[1]=7;

				uimoney=(unsigned int)(m_money2*10);

				hexdata[7]=uimoney>>8;
				hexdata[8]=uimoney&0xff;
			
				m_comm.SetOutput(COleVariant(hexdata));//以十六进制格式化发送
		}else
		{
			MessageBox("请入一个0~6553.5的数!");
		}	
}
