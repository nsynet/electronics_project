object Form1: TForm1
  Left = 238
  Top = 100
  BorderStyle = bsSingle
  Caption = 'uIP TCP/IP test  By '#26197#26469#26197#21435
  ClientHeight = 346
  ClientWidth = 345
  Color = clBtnFace
  Font.Charset = GB2312_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = #23435#20307
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 264
    Width = 308
    Height = 13
    Cursor = crHandPoint
    Caption = #20165#38480#20010#20154#20351#29992','#21830#19994#29992#36884','#35831#19982#26412#20154#32852#31995'  271885846@QQ.com'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    OnClick = Label1Click
  end
  object Label2: TLabel
    Left = 8
    Top = 288
    Width = 217
    Height = 13
    Cursor = crHandPoint
    Hint = #27426#36814#20809#20020#28857#30707#37329#26234#33021#30005#23376
    Caption = 'http://shop34480016.taobao.com/'
    Color = clBtnFace
    ParentColor = False
    OnClick = Label2Click
  end
  object Label3: TLabel
    Left = 8
    Top = 304
    Width = 630
    Height = 13
    Cursor = crHandPoint
    Hint = #27426#36814#20809#20020#28857#30707#37329#26234#33021#30005#23376
    Caption = 
      'http://auction1.taobao.com/auction/item_detail-null-6f8b7290cca9' +
      'dea12e29f63d7f3e1986.jhtml'
    Color = clBtnFace
    ParentColor = False
    OnClick = Label3Click
  end
  object RichEdit1: TRichEdit
    Left = 0
    Top = 0
    Width = 345
    Height = 137
    Align = alTop
    HideSelection = False
    HideScrollBars = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Button1: TButton
    Left = 8
    Top = 208
    Width = 81
    Height = 25
    Caption = #21442#25968#35774#32622
    TabOrder = 1
    OnClick = Button1Click
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 144
    Width = 297
    Height = 49
    Caption = #25353#22238#36710#38190#21457#36865#25968#25454
    TabOrder = 2
    object Edit1: TEdit
      Left = 8
      Top = 16
      Width = 273
      Height = 21
      Hint = #25353#22238#36710#38190#21457#36865#25968#25454
      TabOrder = 0
      Text = 'Welcome to uIP0.9 test project !'
      OnKeyDown = Edit1KeyDown
    end
  end
  object Button2: TButton
    Left = 216
    Top = 208
    Width = 89
    Height = 25
    Caption = #36864#20986#31995#32479
    TabOrder = 3
    OnClick = Button2Click
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 326
    Width = 345
    Height = 20
    Panels = <>
    SimplePanel = True
    SimpleText = #26410#36830#25509
  end
  object Button3: TButton
    Left = 112
    Top = 208
    Width = 81
    Height = 25
    Caption = #36830#25509
    TabOrder = 5
    OnClick = Button3Click
  end
  object ClientSocket1: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = ClientSocket1Connect
    OnRead = ClientSocket1Read
    Left = 208
    Top = 32
  end
end
