VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "DS18B20温度取值程序                     吉林化工学院测控技术与仪器系"
   ClientHeight    =   5055
   ClientLeft      =   45
   ClientTop       =   60
   ClientWidth     =   7320
   BeginProperty Font 
      Name            =   "楷体_GB2312"
      Size            =   7.5
      Charset         =   134
      Weight          =   700
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   21.063
   ScaleMode       =   4  'Character
   ScaleWidth      =   61
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  '屏幕中心
   Begin VB.PictureBox picVoltage 
      BackColor       =   &H00C0FFC0&
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   9
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00C0C0FF&
      Height          =   2805
      Left            =   585
      ScaleHeight     =   2745
      ScaleWidth      =   6480
      TabIndex        =   10
      Top             =   1485
      Width           =   6540
      Begin VB.Label Label2 
         BackColor       =   &H00C0FFC0&
         Caption         =   "        温度检测"
         BeginProperty Font 
            Name            =   "楷体_GB2312"
            Size            =   15.75
            Charset         =   134
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H000000FF&
         Height          =   735
         Left            =   360
         TabIndex        =   11
         Top             =   240
         Width           =   5775
      End
   End
   Begin VB.CommandButton cmdStart 
      Caption         =   "开始测温"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   600
      Left            =   4905
      TabIndex        =   5
      Top             =   90
      Width           =   1590
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   135
      Top             =   900
   End
   Begin VB.CommandButton cmdOpenCOM 
      Caption         =   "打开通信端口"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   600
      Left            =   3195
      TabIndex        =   4
      Top             =   90
      Width           =   1590
   End
   Begin VB.ComboBox cmbCOM 
      BackColor       =   &H00FFC0C0&
      BeginProperty Font 
         Name            =   "Times New Roman"
         Size            =   12
         Charset         =   0
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   405
      Left            =   450
      TabIndex        =   3
      Text            =   "Combo1"
      Top             =   405
      Width           =   1845
   End
   Begin VB.CommandButton cmdEnd 
      Caption         =   "结束"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   600
      Left            =   4950
      TabIndex        =   0
      Top             =   810
      Width           =   1590
   End
   Begin MSCommLib.MSComm MSComm1 
      Left            =   3960
      Top             =   900
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "50"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Index           =   3
      Left            =   135
      TabIndex        =   9
      Top             =   1440
      Width           =   240
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "0"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   240
      Index           =   2
      Left            =   225
      TabIndex        =   8
      Top             =   4050
      Width           =   120
   End
   Begin VB.Label lblMsg 
      BackColor       =   &H00C0FFFF&
      BorderStyle     =   1  'Fixed Single
      Caption         =   "作者:dzh   EMAIL:dzh2800@126.com"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   420
      Left            =   585
      TabIndex        =   7
      Top             =   4500
      Width           =   6540
   End
   Begin VB.Label lblValue 
      AutoSize        =   -1  'True
      BackColor       =   &H00000000&
      BeginProperty Font 
         Name            =   "Arial Black"
         Size            =   18
         Charset         =   0
         Weight          =   900
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H000000FF&
      Height          =   495
      Left            =   2475
      TabIndex        =   6
      Top             =   945
      Width           =   1080
   End
   Begin VB.Label Label1 
      AutoSize        =   -1  'True
      Caption         =   "现在温度："
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   14.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   285
      Index           =   1
      Left            =   900
      TabIndex        =   2
      Top             =   1080
      Width           =   1425
   End
   Begin VB.Label Label1 
      Caption         =   "通信端口"
      BeginProperty Font 
         Name            =   "宋体"
         Size            =   12
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   330
      Index           =   0
      Left            =   450
      TabIndex        =   1
      Top             =   135
      Width           =   1125
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim NowX As Integer  '现在的X轴位置
Dim MaxPlotNo As Long  '最长的X轴范围
Dim PreValue As Single  '前一个测量值

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'当选择通信端口的Combo控件被选中后激活此事件
'若用户改变通信端口时，关闭通信端口
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmbCOM_Click()
  '若通信端口号码和现在的选择一样时就不必理会，直接跳出此子程序
  If cmbCOM.ListIndex + 1 = MSComm1.CommPort Then Exit Sub
  Timer1.Enabled = False                      '关闭定时器
  TimeDelay 100
  If MSComm1.PortOpen Then
    MSComm1.PortOpen = False                    '关闭通信端口
  End If
  lblMsg.Caption = "已停止检测并关闭通讯端口"
  cmdOpenCOM.Enabled = True     '允许使用【打开通信端口】按钮
End Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'单击【结束】按钮后激活此事件
'使用End命令将系统结束
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdEnd_Click()
  MSComm1.PortOpen = False          '关闭通信端口
  End
End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'单击【打开通信端口】按钮后激活此事件
'将MSComm控件的参数设置好，并打开
'激活【开始检测】按钮
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdOpenCOM_Click()
  '判断端口号码是否落在1--16之间
  If cmbCOM.ListIndex >= 0 And cmbCOM.ListIndex <= 16 Then
    MSComm1.CommPort = cmbCOM.ListIndex + 1
  Else
    MsgBox "指定通信端口时发生错误！", vbCritical + vbOKOnly, "系统信息"
    Exit Sub
  End If
  '激活错误检测机制
  On Error GoTo comErr
  MSComm1.Settings = "9600,n,8,1"  '设定通信参数
  MSComm1.PortOpen = True          '打开通信端口
  cmdOpenCOM.Enabled = False       '将此按钮设为禁用状态
  cmdStart.Enabled = True          '激活【开始检测】按钮
  lblMsg.Caption = "可单击【开始检测】按钮，执行检测的工作。"
  Exit Sub
comErr:
  MsgBox "打开通信端口时发生错误！请确定通信端口存在且正常。", vbCritical + vbOKOnly, "系统信息"
End Sub


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'单击【开始检测】按钮后激活此事件
'将定时器激活或关闭，并显示对应的文字在按钮上，以指示用户操作
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdStart_Click()
  Timer1.Enabled = Not Timer1.Enabled
  If Timer1.Enabled Then
    cmdStart.Caption = "停止检测"
  Else
    cmdStart.Caption = "开始检测"
    lblMsg.Caption = "已停止检测"
  End If
End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'窗体的Load事件
'输入图形暂时设为灰色，表示无状态信息进入
'将通讯端口号码及站号填入Combo控件；并默认二者的选项是第一个
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub Form_Load()
  Dim i%
  MaxPlotNo = 100
  cmbCOM.Clear
  cmbCOM.AddItem "COM1"
  cmbCOM.AddItem "COM2"
   cmbCOM.AddItem "COM3(USB)"
  cmbCOM.AddItem "COM4(USB)"
   cmbCOM.AddItem "COM5"
  cmbCOM.AddItem "COM6"
   cmbCOM.AddItem "COM7"
  cmbCOM.AddItem "COM8"
   cmbCOM.AddItem "COM9"
  cmbCOM.AddItem "COM10"
   cmbCOM.AddItem "COM11"
  cmbCOM.AddItem "COM12"
   cmbCOM.AddItem "COM12"
  cmbCOM.AddItem "COM14"
   cmbCOM.AddItem "COM15"
  cmbCOM.AddItem "COM16"
  cmbCOM.ListIndex = 0
  cmdStart.Enabled = False
  '以下设定绘图范围，(Xmin,YMax)-(XMax,YMin)
  picVoltage.Scale (0, 50)-(MaxPlotNo, 0)
  picVoltage.DrawWidth = 2 '使用两个像素宽度的画笔
End Sub



'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'定时器的Timer事件引发后，就不断地执行其中的程序。
'将模拟读值命令送出，再取得返回字符串并判断。
'
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub Timer1_Timer()
  Dim Buf$, ValueStr As Single, Pos1%
 Buf$ = Buf$ + MSComm1.Input '读取变量
 TimeDelay 2150
  ValueStr = Val(Mid(Buf, Pos1 + 1, 7)) '分离出正号以后的数值
  lblValue.Caption = ""                    '清空上一次数据
  lblValue.Caption = Format(ValueStr, "00.0") & "℃"  '显示在画面上
  If NowX = 0 Then
    picVoltage.Cls '清除图形
    picVoltage.PSet (0, ValueStr)  '设定起点
  Else
    '以下判断现在的读值是否大于前一次的读值，若是，则以红色绘线
    '若否，则以蓝色绘线
    'If ValueStr > PreValue + 0.01 Then
      picVoltage.Line -(NowX, ValueStr), RGB(255, 0, 0) '由上一次的位置画至此点
    'Else
     ' picVoltage.Line -(NowX, ValueStr), RGB(0, 0, 255) '由上一次的位置画至此点
    'End If
  End If
  PreValue = ValueStr
  NowX = NowX + 1 '位置加1
  If NowX > MaxPlotNo Then NowX = 0  '超过范围则数值归零
End Sub

