unit COM_CONNECT;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ExtCtrls,ShellApi, SPComm;

type
	ByteArray = array [0..0] of byte;
  byteall = array[0..10000] of byte;
	PpByte = ^ByteArray;

type
  TFrmMain=class(TForm)
    Memo1: TMemo;
    btnSend: TButton;
    Button1: TButton;
    Memo2: TMemo;
    GroupBox1: TGroupBox;
    Label1: TLabel;
    Label2: TLabel;
    ComboBox1: TComboBox;
    ComboBox2: TComboBox;
    btnSwitch: TButton;
    ImageOn: TImage;
    ImageOff: TImage;
    Comm1: TComm;
    procedure FormCreate(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure btnSendClick(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure ComboBox2Change(Sender: TObject);
    procedure btnSwitchClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure SendString(const str: string);
    procedure Comm1ReceiveData(Sender: TObject; Buffer: Pointer;
      BufferLength: Word);
    Function ByteToStr(NameArr :Array of byte):String;
    function ByteToHex(Src: Byte): String;
  private
    { Private declarations }

		hhzk : HGlOBAL;
		hasc : HGLOBAL;
		phzk : PpByte;
		pasc : PpByte;

		procedure HandleRes;
    procedure get_code(str: string);
  public
    { Public declarations }
  end;
var
 frmmain: Tfrmmain;
     all : byteall;
implementation

type
  ERegError = class(Exception);

{$R *.dfm}
{$R hzk.res}

procedure Tfrmmain.HandleRes;
var
	hres : HRSRC;
begin
	hres := FindResource(0,PChar(201),'HZK16');
	if hres <> 0 then begin
		hhzk := LoadResource(0,hres);
		if hhzk <> 0 then begin
			phzk := LockResource(hhzk);
		end;
	end;
	hres := FindResource(0,PChar(202),'ASC16');
	if hres <> 0 then begin
		hasc := LoadResource(0,hres);
		if hasc <> 0 then begin
			pasc := LockResource(hasc);
		end;
	end;
	if (phzk = nil) or (pasc = nil) then begin
		MessageDlg('Can not load resource!'#10#13'Program stopped!',
				mtWarning,[mbOk],0);
		Application.Terminate;
	end;
end;

procedure TFrmMain.get_code(str: string);
var
 i,j : integer;
 m : integer;
 leng : integer;
 off : integer;

begin
 i := 1;
 j:=0;
 leng := Length(str);
  while(i<=leng) do begin
   if (i<leng) and boolean(byte(str[i]) and $80)
       and boolean(byte(str[i+1]) and $80) then begin
   off := ((byte(str[i])-$a1) and $7f) * 94 + ((byte(str[i+1])-$a1) and $7f);
   off := off * 32;
   //获取汉字库点阵代码 32个字节
     for m:=0 to 31 do begin
        all[j]:=phzk^[off+m];
        //showmessage(bytetohex(all[j]));
        inc(j);
     end;
     inc(i,2);
       end else begin
        off := byte(str[i])*16;
         //获取ASC库点阵代码 16个字节
          for m:=0 to 15 do begin
           all[j]:=pasc^[off+m];
            inc(j);
          end;
          inc(i);
          end;
  end;
end;

//获取串口列表
procedure EnumComPorts(Ports: TStrings);
var
  KeyHandle: HKEY;
  ErrCode, Index: Integer;
  ValueName, Data: string;
  ValueLen, DataLen, ValueType: DWORD;
  TmpPorts: TStringList;
begin
  ErrCode := RegOpenKeyEx(HKEY_LOCAL_MACHINE, 'HARDWARE\DEVICEMAP\SERIALCOMM', 0,
    KEY_READ, KeyHandle);

  if ErrCode <> ERROR_SUCCESS then
    raise ERegError.Create('打开串口列表的注册表项出错');

  TmpPorts := TStringList.Create;
  try
    Index := 0;
    repeat
      ValueLen := 256;
      DataLen := 256;
      SetLength(ValueName, ValueLen);
      SetLength(Data, DataLen);
      ErrCode := RegEnumValue(KeyHandle, Index, PChar(ValueName),
        Cardinal(ValueLen), nil, @ValueType, PByte(PChar(Data)), @DataLen);

      if ErrCode = ERROR_SUCCESS then
      begin
        SetLength(Data, DataLen);
        TmpPorts.Add(Data);
        Inc(Index);
      end
      else if ErrCode <> ERROR_NO_MORE_ITEMS then
        raise ERegError.Create('打开串口列表的注册表项出错');

    until (ErrCode <> ERROR_SUCCESS);

    TmpPorts.Sort;
    Ports.Assign(TmpPorts);
  finally
    RegCloseKey(KeyHandle);
    TmpPorts.Free;
  end;
end;



procedure TFrmMain.FormCreate(Sender: TObject);

begin
  hhzk := 0;
	hasc := 0;
	phzk := nil;
	pasc := nil;
	HandleRes;
  //设置窗体的最小大小
  FrmMain.Constraints.MinHeight := FrmMain.Height;
  FrmMain.Constraints.MinWidth := FrmMain.Width;
  btnSend.Enabled:=false;
  ImageOff.Visible:=true;
  ImageOn.Visible:=false;

  //串口初始化
  EnumComPorts(ComboBox1.Items);    //得到串口列表
  ComboBox1.ItemIndex := 0;
  Comm1.CommName :=ComboBox1.Text;
  ComboBox2.ItemIndex := 2;
  Comm1.BaudRate := 9600;
  Comm1.Parity := None;
  Comm1.ByteSize := _8;
  Comm1.StopBits := _1;
end;

procedure TFrmMain.ComboBox1Change(Sender: TObject);
begin
  Comm1.CommName := ComboBox1.Text;
end;


procedure TFrmMain.SendString(const str: string);
begin
//get_code(memo1.Text);
//frmmain.comm1.writecommdata(@all[0],sizeof(all));

end;
procedure TFrmMain.btnSendClick(Sender: TObject);  //发送函数
var
i:integer;
begin
get_code(memo1.Text);
//for i:=1 to length(memo1.text)*16 do begin
frmmain.comm1.writecommdata(@all[0],length(memo1.text)*16);
//sleep(2);
//showmessage(bytetostr(all[i]));
//showmessage('ok');

end;

procedure TFrmMain.Button1Click(Sender: TObject);
begin
close;
end;

procedure TFrmMain.ComboBox2Change(Sender: TObject);
begin
comm1.BaudRate :=StrToInt(comboBox2.text);
end;

procedure TFrmMain.btnSwitchClick(Sender: TObject);
begin
  //判断按键的状态可以避免打开串口出错时，要按两次按键
  if btnSwitch.Caption = '打开串口' then
  begin
    Comm1.StartComm;  //打开串口
    btnSwitch.Caption := '关闭串口';
    ComboBox1.Enabled := false;
    btnSend.Enabled   := true;
    ComboBox2.Enabled :=false;
    ImageOff.Visible  := false;
    ImageOn.Visible   := true;
  end
  else //if Button1.Caption = '关闭串口' then
  begin
    Comm1.StopComm;  // 关闭串口
    btnSwitch.Caption := '打开串口';
    ComboBox1.Enabled := true;
    ComboBox2.Enabled := true;
    btnSend.Enabled   := false;
    ImageOn.Visible   := false;
    ImageOff.Visible  := true;
  end;
end;
procedure TFrmMain.FormDestroy(Sender: TObject);
begin
	if hhzk <> 0 then FreeResource(hhzk);
	if hasc <> 0 then FreeResource(hasc);
end;

procedure TFrmMain.Comm1ReceiveData(Sender: TObject; Buffer: Pointer;
  BufferLength: Word);
var  s:string;
begin
 setlength(s,bufferlength);
 move(buffer^,s[1],bufferlength);
 Memo2.Lines.Add(s);
end;
Function tfrmmain.ByteToStr(NameArr :Array of byte):String;
var I:Integer;
   S:String;
Begin
Setlength(S,High(NameArr)+1);
For I:=Low(NameArr) to High(NameArr) do S[I+1]:=Char(NameArr[I]);
Result :=Pchar(S);
End;
function tfrmmain.ByteToHex(Src: Byte): String;
begin
SetLength(Result, 2);
asm
MOV EDI, [Result]
MOV EDI, [EDI]
MOV AL, Src
MOV AH, AL // Save to AH
SHR AL, 4 // Output High 4 Bits
ADD AL, '0'
CMP AL, '9'
JBE @@OutCharLo
ADD AL, 'A'-'9'-1
@@OutCharLo:
AND AH, $f
ADD AH, '0'
CMP AH, '9'
JBE @@OutChar
ADD AH, 'A'-'9'-1
@@OutChar:
STOSW
end;
end;

end.


