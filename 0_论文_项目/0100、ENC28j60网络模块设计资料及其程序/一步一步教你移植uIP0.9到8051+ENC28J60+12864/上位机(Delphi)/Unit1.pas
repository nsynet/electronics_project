unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, StdCtrls, ComCtrls, ScktComp, Sockets, ShellAPI;

type
  TForm1 = class(TForm)

    RichEdit1: TRichEdit;
    Button1: TButton;
    GroupBox2: TGroupBox;
    Edit1: TEdit;
    Button2: TButton;
    StatusBar1: TStatusBar;
    ClientSocket1: TClientSocket;
    Button3: TButton;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    procedure Edit1KeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure Button1Click(Sender: TObject);
    procedure Button2Click(Sender: TObject);
    procedure ClientSocket1Connect(Sender: TObject;
      Socket: TCustomWinSocket);
    procedure ClientSocket1Read(Sender: TObject; Socket: TCustomWinSocket);
    procedure Button3Click(Sender: TObject);
    procedure Label2Click(Sender: TObject);
    procedure Label3Click(Sender: TObject);
    procedure Label1Click(Sender: TObject);
    //procedure Edit1Change(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }

  end;

var
  Form1: TForm1;

implementation
    var
    Nickname:string;
{$R *.dfm}

procedure TForm1.Edit1KeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
  var str1:string;
begin

    if key=13 then
    begin
    if clientsocket1.Socket.Connected then

    begin

       clientsocket1.Socket.SendText(edit1.Text );
       str1:='发送数据: '+edit1.text;
       richedit1.Lines.Add(str1);
       edit1.text:='';
    end
else showmessage('未连接');
end;
end;

procedure TForm1.FormClose(Sender: TObject; var Action: TCloseAction);
begin

   clientsocket1.Active :=false;
   clientsocket1.close;
end;

procedure TForm1.Button1Click(Sender: TObject);
var IPADDR,PORT:string;
begin
   IPADDR:=inputbox('Create connect','Please input IP','192.168.1.13');
   PORT:=inputbox('Create connect','Please input PORT','8001');
    if trim(IPADDR)<>'' then
    begin
       clientsocket1.Port:=strtoint(PORT);//server port
       clientsocket1.host:=IPADDR;

    end;
end;
procedure TForm1.Button2Click(Sender: TObject);
begin

    close;
end;
procedure TForm1.ClientSocket1Connect(Sender: TObject;
  Socket: TCustomWinSocket);
begin
    StatusBar1.SimpleText:='连接成功';
end;

procedure TForm1.ClientSocket1Read(Sender: TObject;
  Socket: TCustomWinSocket);
var str1,str2:string;

begin

    str1:=socket.ReceiveText;
    str1:='收到数据: ' + str1;
    richedit1.Lines.Add(str1);

end;

procedure TForm1.Button3Click(Sender: TObject);

begin
       if clientsocket1.Socket.Connected then
       begin

       clientsocket1.Active:=false;
       statusbar1.SimpleText:='未连接';
       button3.Caption:='连接';
       end
       else

       try
       clientsocket1.active:=true;
       button3.Caption:='断开';
       clientsocket1.Socket.SendText(Nickname);
       except
       showmessage('参数不正确,请重新设定');
       end;
end;

procedure TForm1.Label2Click(Sender: TObject);
begin
  ShellExecute(handle,nil,pchar('http://shop34480016.taobao.com'),nil,nil,
                            sw_shownormal);
end;

procedure TForm1.Label3Click(Sender: TObject);
begin
  ShellExecute(handle,nil,
    pchar('http://auction1.taobao.com/auction/item_detail-null-6f8b7290cca9dea12e29f63d7f3e1986.jhtml'),
      nil,nil,
      sw_shownormal);
end;

procedure TForm1.Label1Click(Sender: TObject);
begin
  ShellExecute(handle,nil,pchar('mailto:271885846@QQ.com'),nil,nil,
                            sw_shownormal);
end;

end.
