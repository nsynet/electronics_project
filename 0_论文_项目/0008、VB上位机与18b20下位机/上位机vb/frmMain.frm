VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form Form1 
   BorderStyle     =   5  'Sizable ToolWindow
   Caption         =   "DS18B20�¶�ȡֵ����                     ���ֻ���ѧԺ��ؼ���������ϵ"
   ClientHeight    =   5055
   ClientLeft      =   45
   ClientTop       =   60
   ClientWidth     =   7320
   BeginProperty Font 
      Name            =   "����_GB2312"
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
   StartUpPosition =   2  '��Ļ����
   Begin VB.PictureBox picVoltage 
      BackColor       =   &H00C0FFC0&
      BeginProperty Font 
         Name            =   "����"
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
         Caption         =   "        �¶ȼ��"
         BeginProperty Font 
            Name            =   "����_GB2312"
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
      Caption         =   "��ʼ����"
      BeginProperty Font 
         Name            =   "����"
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
      Caption         =   "��ͨ�Ŷ˿�"
      BeginProperty Font 
         Name            =   "����"
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
      Caption         =   "����"
      BeginProperty Font 
         Name            =   "����"
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
         Name            =   "����"
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
         Name            =   "����"
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
      Caption         =   "����:dzh   EMAIL:dzh2800@126.com"
      BeginProperty Font 
         Name            =   "����"
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
      Caption         =   "�����¶ȣ�"
      BeginProperty Font 
         Name            =   "����"
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
      Caption         =   "ͨ�Ŷ˿�"
      BeginProperty Font 
         Name            =   "����"
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
Dim NowX As Integer  '���ڵ�X��λ��
Dim MaxPlotNo As Long  '���X�᷶Χ
Dim PreValue As Single  'ǰһ������ֵ

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'��ѡ��ͨ�Ŷ˿ڵ�Combo�ؼ���ѡ�к󼤻���¼�
'���û��ı�ͨ�Ŷ˿�ʱ���ر�ͨ�Ŷ˿�
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmbCOM_Click()
  '��ͨ�Ŷ˿ں�������ڵ�ѡ��һ��ʱ�Ͳ�����ᣬֱ���������ӳ���
  If cmbCOM.ListIndex + 1 = MSComm1.CommPort Then Exit Sub
  Timer1.Enabled = False                      '�رն�ʱ��
  TimeDelay 100
  If MSComm1.PortOpen Then
    MSComm1.PortOpen = False                    '�ر�ͨ�Ŷ˿�
  End If
  lblMsg.Caption = "��ֹͣ��Ⲣ�ر�ͨѶ�˿�"
  cmdOpenCOM.Enabled = True     '����ʹ�á���ͨ�Ŷ˿ڡ���ť
End Sub

''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'��������������ť�󼤻���¼�
'ʹ��End���ϵͳ����
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdEnd_Click()
  MSComm1.PortOpen = False          '�ر�ͨ�Ŷ˿�
  End
End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'��������ͨ�Ŷ˿ڡ���ť�󼤻���¼�
'��MSComm�ؼ��Ĳ������úã�����
'�����ʼ��⡿��ť
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdOpenCOM_Click()
  '�ж϶˿ں����Ƿ�����1--16֮��
  If cmbCOM.ListIndex >= 0 And cmbCOM.ListIndex <= 16 Then
    MSComm1.CommPort = cmbCOM.ListIndex + 1
  Else
    MsgBox "ָ��ͨ�Ŷ˿�ʱ��������", vbCritical + vbOKOnly, "ϵͳ��Ϣ"
    Exit Sub
  End If
  '������������
  On Error GoTo comErr
  MSComm1.Settings = "9600,n,8,1"  '�趨ͨ�Ų���
  MSComm1.PortOpen = True          '��ͨ�Ŷ˿�
  cmdOpenCOM.Enabled = False       '���˰�ť��Ϊ����״̬
  cmdStart.Enabled = True          '�����ʼ��⡿��ť
  lblMsg.Caption = "�ɵ�������ʼ��⡿��ť��ִ�м��Ĺ�����"
  Exit Sub
comErr:
  MsgBox "��ͨ�Ŷ˿�ʱ����������ȷ��ͨ�Ŷ˿ڴ�����������", vbCritical + vbOKOnly, "ϵͳ��Ϣ"
End Sub


'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'��������ʼ��⡿��ť�󼤻���¼�
'����ʱ�������رգ�����ʾ��Ӧ�������ڰ�ť�ϣ���ָʾ�û�����
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub cmdStart_Click()
  Timer1.Enabled = Not Timer1.Enabled
  If Timer1.Enabled Then
    cmdStart.Caption = "ֹͣ���"
  Else
    cmdStart.Caption = "��ʼ���"
    lblMsg.Caption = "��ֹͣ���"
  End If
End Sub

'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'�����Load�¼�
'����ͼ����ʱ��Ϊ��ɫ����ʾ��״̬��Ϣ����
'��ͨѶ�˿ں��뼰վ������Combo�ؼ�����Ĭ�϶��ߵ�ѡ���ǵ�һ��
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
  '�����趨��ͼ��Χ��(Xmin,YMax)-(XMax,YMin)
  picVoltage.Scale (0, 50)-(MaxPlotNo, 0)
  picVoltage.DrawWidth = 2 'ʹ���������ؿ�ȵĻ���
End Sub



'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
'��ʱ����Timer�¼������󣬾Ͳ��ϵ�ִ�����еĳ���
'��ģ���ֵ�����ͳ�����ȡ�÷����ַ������жϡ�
'
''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
Private Sub Timer1_Timer()
  Dim Buf$, ValueStr As Single, Pos1%
 Buf$ = Buf$ + MSComm1.Input '��ȡ����
 TimeDelay 2150
  ValueStr = Val(Mid(Buf, Pos1 + 1, 7)) '����������Ժ����ֵ
  lblValue.Caption = ""                    '�����һ������
  lblValue.Caption = Format(ValueStr, "00.0") & "��"  '��ʾ�ڻ�����
  If NowX = 0 Then
    picVoltage.Cls '���ͼ��
    picVoltage.PSet (0, ValueStr)  '�趨���
  Else
    '�����ж����ڵĶ�ֵ�Ƿ����ǰһ�εĶ�ֵ�����ǣ����Ժ�ɫ����
    '����������ɫ����
    'If ValueStr > PreValue + 0.01 Then
      picVoltage.Line -(NowX, ValueStr), RGB(255, 0, 0) '����һ�ε�λ�û����˵�
    'Else
     ' picVoltage.Line -(NowX, ValueStr), RGB(0, 0, 255) '����һ�ε�λ�û����˵�
    'End If
  End If
  PreValue = ValueStr
  NowX = NowX + 1 'λ�ü�1
  If NowX > MaxPlotNo Then NowX = 0  '������Χ����ֵ����
End Sub

