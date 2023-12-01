Attribute VB_Name = "Module1"
Declare Function GetTickCount Lib "kernel32" () As Long

Sub TimeDelay(t As Long)
  '时间延迟子程序，单位是毫秒(ms)
  Dim TT&
  TT = GetTickCount()
  Do
   DoEvents
  Loop Until GetTickCount() - TT >= t
End Sub


'等待RS字符串返回，或是时间到达
'Comm是通信控件名称
'RS是欲等待的字符
'DT是最长的等待时间
'正常时返回值是所得的完整字符串，不正常时返回值是空字符串
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

