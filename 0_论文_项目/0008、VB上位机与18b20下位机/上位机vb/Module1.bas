Attribute VB_Name = "Module1"
Declare Function GetTickCount Lib "kernel32" () As Long

Sub TimeDelay(t As Long)
  'ʱ���ӳ��ӳ��򣬵�λ�Ǻ���(ms)
  Dim TT&
  TT = GetTickCount()
  Do
   DoEvents
  Loop Until GetTickCount() - TT >= t
End Sub


'�ȴ�RS�ַ������أ�����ʱ�䵽��
'Comm��ͨ�ſؼ�����
'RS�����ȴ����ַ�
'DT����ĵȴ�ʱ��
'����ʱ����ֵ�����õ������ַ�����������ʱ����ֵ�ǿ��ַ���
Function WaitRS(Comm As MSComm, RS As String, DT As Long) As String
  Dim Buf$, TT As Long
  Buf = ""
  TT = GetTickCount
  Do
    Buf = Buf & Comm.Input
  Loop Until InStr(1, Buf, RS) > 0 Or GetTickCount - TT >= DT
  If InStr(1, Buf, RS) > 0 Then
    WaitRS = Buf
  Else
    WaitRS = ""
  End If
End Function

