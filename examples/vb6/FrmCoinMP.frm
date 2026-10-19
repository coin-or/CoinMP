VERSION 5.00
Begin VB.Form frmCoinMP
   Caption         =   "CoinMP Test"
   ClientHeight    =   6720
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   7485
   LinkTopic       =   "Form1"
   ScaleHeight     =   6720
   ScaleWidth      =   7485
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox txt
      Height          =   5535
      Left            =   240
      MultiLine       =   -1  'True
      ScrollBars      =   2  'Vertical
      TabIndex        =   2
      Top             =   240
      Width           =   6975
   End
   Begin VB.CommandButton cmdClear
      Caption         =   "Clear"
      Height          =   495
      Left            =   4200
      TabIndex        =   1
      Top             =   6000
      Width           =   1335
   End
   Begin VB.CommandButton cmdRunTest
      Caption         =   "Run Test"
      Height          =   495
      Left            =   1560
      TabIndex        =   0
      Top             =   6000
      Width           =   1335
   End
End
Attribute VB_Name = "frmCoinMP"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Const MAX_NAME_LEN As Long = 100

Private Sub cmdClear_Click()
    LogEmptyText
End Sub

Private Sub cmdRunTest_Click()

Call SolveProblemBakery
Call SolveProblemCoinTest
Call SolveProblemAfiro
Call SolveProblemP0033
Call SolveProblemExMip1

End Sub


Private Sub Form_Load()

Dim result As Long
Dim length As Long
Dim solverName As String
Dim version As Double


    result = CoinInitSolver("")

    solverName = String(MAX_NAME_LEN, vbNullChar)
    length = CoinGetSolverNameBuf(solverName, MAX_NAME_LEN)
    solverName = Left(solverName, length)

    version = CoinGetVersion()

    LogEmptyText
    LogWriteLine "Solver: " & solverName
    LogWriteLine "Version:  " & version


End Sub


Private Sub Form_Unload(Cancel As Integer)

Dim result As Long

result = CoinFreeSolver()
LogWriteLine "All unit tests completed successfully"

End Sub

