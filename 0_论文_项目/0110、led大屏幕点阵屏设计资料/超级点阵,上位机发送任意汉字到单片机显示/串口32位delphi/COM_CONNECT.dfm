object FrmMain: TFrmMain
  Left = 559
  Top = 220
  Width = 696
  Height = 480
  Caption = 'FrmMain'
  Color = clMoneyGreen
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 344
    Top = 48
    Width = 257
    Height = 193
    TabOrder = 0
  end
  object btnSend: TButton
    Left = 16
    Top = 208
    Width = 57
    Height = 25
    Caption = #21457#36865
    TabOrder = 1
    OnClick = btnSendClick
  end
  object Button1: TButton
    Left = 96
    Top = 208
    Width = 57
    Height = 25
    Caption = #36864#20986
    TabOrder = 2
    OnClick = Button1Click
  end
  object Memo2: TMemo
    Left = 344
    Top = 264
    Width = 257
    Height = 97
    TabOrder = 3
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 161
    Height = 185
    Caption = #20018#21475#21021#22987#21270#35774#32622
    Color = clMoneyGreen
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentColor = False
    ParentFont = False
    TabOrder = 4
    object Label1: TLabel
      Left = 8
      Top = 24
      Width = 49
      Height = 16
      AutoSize = False
      Caption = #31471#21475
    end
    object Label2: TLabel
      Left = 8
      Top = 48
      Width = 65
      Height = 16
      AutoSize = False
      Caption = #27874#29305#29575
    end
    object ImageOn: TImage
      Left = 16
      Top = 80
      Width = 41
      Height = 33
      Picture.Data = {
        055449636F6E0000010001002020000001001800A80C00001600000028000000
        20000000400000000100180000000000800C0000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000FF000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000FF000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000000000000000FF00000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000FF0000000000000000000000000000000000
        000000000000000000000000000000000000000000000000000000000000FF00
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FF0000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000FFFFFFFFFFFFFFFFFF0000FF0000FF0000FFFFFFFFFFFFFFFFFFFF
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFFFFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FFFFFFFF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFFFFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        0000FFFFFFFF0000000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000FF0000FF000000000000000000FF
        FFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        0000FFFFFFFF0000000000000000000000FF0000FF0000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        0000FFFFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFFFFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF0000FF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFFFFFFFF0000FF0000FF0000FF0000FF0000FF0000FF0000FFFFFFFF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000FFFFFFFFFFFFFFFFFF0000FF0000FF0000FFFFFFFFFFFFFFFFFFFF
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000000000000000000000FF00
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000FF0000000000000000000000000000000000000000000000
        000000000000000000000000000000000000000000000000000000FF00000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FF0000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000FF000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000FF000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFFFEFFFF
        FFFFFFFFFEFFFF7FFF783EFFFFE00FFFFFC007FFFF8003FFFF8003FFFF0001FF
        FF0001FFF300019FFF0001FFFF0001FFFF8003FFFF8003FFFFC007FFFFE00FFF
        FF783DFFFEFFFEFFFFFFFFFFFFFEFFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFF}
    end
    object ImageOff: TImage
      Left = 16
      Top = 80
      Width = 41
      Height = 33
      Picture.Data = {
        055449636F6E0000010001002020000001001800A80C00001600000028000000
        20000000400000000100180000000000800C0000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000FFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFFFF
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFFFFFFFF000000000000000000000000000000000000000000FFFFFF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFF000000000000000000000000000000000000000000000000000000
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFFFFFFFF000000000000000000000000000000000000000000000000000000
        FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFF000000000000000000000000000000000000000000000000000000000000
        000000FFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFF000000000000000000000000000000000000000000000000000000000000
        000000FFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFF000000000000000000000000000000000000000000000000000000000000
        000000FFFFFF0000000000000000000000000000000000000000000000000000
        00000000000000000000000000000000000000000000000000000000000000FF
        FFFFFFFFFF000000000000000000000000000000000000000000000000000000
        FFFFFFFFFFFF0000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFF000000000000000000000000000000000000000000000000000000
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000FFFFFFFFFFFF000000000000000000000000000000000000000000FFFFFF
        FFFFFF0000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000FFFFFFFFFFFFFFFFFF000000000000000000FFFFFFFFFFFFFFFFFF
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFF000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        0000000000000000000000000000000000000000000000000000000000000000
        00000000FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFFFFFFFFFFFFF83FFFFFE00FFFFFC007FFFF8003FFFF8003FFFF0001FF
        FF0001FFFF0001FFFF0001FFFF0001FFFF8003FFFF8003FFFFC007FFFFE00FFF
        FFF83FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF
        FFFFFFFF}
    end
    object ComboBox1: TComboBox
      Left = 56
      Top = 24
      Width = 97
      Height = 21
      ItemHeight = 13
      TabOrder = 0
      OnChange = ComboBox1Change
    end
    object ComboBox2: TComboBox
      Left = 56
      Top = 48
      Width = 97
      Height = 21
      ItemHeight = 13
      TabOrder = 1
      OnChange = ComboBox2Change
      Items.Strings = (
        '1200'
        '4800'
        '9600')
    end
    object btnSwitch: TButton
      Left = 88
      Top = 88
      Width = 57
      Height = 25
      Caption = #25171#24320#20018#21475
      TabOrder = 2
      OnClick = btnSwitchClick
    end
  end
  object Comm1: TComm
    CommName = 'COM2'
    BaudRate = 9600
    ParityCheck = False
    Outx_CtsFlow = False
    Outx_DsrFlow = False
    DtrControl = DtrEnable
    DsrSensitivity = False
    TxContinueOnXoff = True
    Outx_XonXoffFlow = True
    Inx_XonXoffFlow = True
    ReplaceWhenParityError = False
    IgnoreNullChar = False
    RtsControl = RtsEnable
    XonLimit = 500
    XoffLimit = 500
    ByteSize = _8
    Parity = None
    StopBits = _1
    XonChar = #17
    XoffChar = #19
    ReplacedChar = #0
    ReadIntervalTimeout = 100
    ReadTotalTimeoutMultiplier = 0
    ReadTotalTimeoutConstant = 0
    WriteTotalTimeoutMultiplier = 0
    WriteTotalTimeoutConstant = 0
    OnReceiveData = Comm1ReceiveData
    Left = 232
    Top = 24
  end
end
