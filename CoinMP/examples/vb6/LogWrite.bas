Attribute VB_Name = "LogWrite"
Option Explicit

Public Sub LogWriteText(text As String)
    frmCoinMP.txt = frmCoinMP.txt & text
End Sub


Public Sub LogWriteLine(text As String)
    frmCoinMP.txt = frmCoinMP.txt & text & vbCrLf
End Sub


Public Sub LogEmptyText()
    frmCoinMP.txt = ""
End Sub


