program connect;

uses
  Forms,
  COM_CONNECT in 'COM_CONNECT.pas' {FrmMain};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TFrmMain, FrmMain);
  Application.Run;
end.
