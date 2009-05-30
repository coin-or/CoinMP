Attribute VB_Name = "RunProblem"
Option Explicit

Const MAX_NAME_LEN As Long = 100

Private Function MsgLogCallback(ByVal MessageStr As String) As Long
    LogWriteLine MessageStr
End Function



Public Sub RunTestProblem(ByVal problemName As String, ByVal optimalValue As Double, _
    ByVal colCount As Long, ByVal rowCount As Long, ByVal nonZeroCount As Long, _
    ByVal rangeCount As Long, ByVal objectSense As Long, ByVal objectConst As Double, _
    ByRef objectCoeffs As Double, ByRef lowerBounds As Double, ByRef upperBounds As Double, _
    ByRef rowType As Byte, ByRef rhsValues As Double, ByRef rangeValues As Double, _
    ByRef matrixBegin As Long, ByRef matrixCount As Long, ByRef matrixIndex As Long, _
    ByRef matrixValues As Double, ByRef colNames() As String, ByRef rowNames() As String, _
    ByVal objectName As String, ByRef InitValues As Double, ByRef colType As Byte)

Dim hProb As Long
Dim solutionStatus As Long
Dim solutionText As String
Dim objectValue As Double
    
Dim activity() As Double
Dim reduced() As Double
Dim slack() As Double
Dim shadow() As Double

Dim colName As String

Dim result As Long
Dim length As Long
Dim i As Integer
Dim ch As String

    LogWriteLine ""
    LogWriteLine "Solve Problem: " & problemName

    hProb = CoinCreateProblem(problemName)
    result = CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount, _
                    objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds, _
                    rowType, rhsValues, rangeValues, matrixBegin, matrixCount, _
                    matrixIndex, matrixValues, colNames, rowNames, objectName)
    If (result <> 0) Then
        LogWriteLine "CoinLoadProblem failed"
    End If
    If colType Then
        result = CoinLoadInteger(hProb, colType)
        If (result <> 0) Then
            LogWriteLine "CoinLoadInteger failed"
        End If
    End If
    result = CoinCheckProblem(hProb)
    If result <> 0 Then
        LogWriteLine "Check Problem failed (result = " & result & ")"
    End If

    'result = CoinSetMsgLogCallback(hProb, AddressOf MsgLogCallback)
    result = CoinOptimizeProblem(hProb, 0)
    result = CoinWriteFile(hProb, SOLV_FILE_MPS, problemName & ".mps")
   

    'problemName = String(MAX_NAME_LEN, vbNullChar)
    'length = CoinGetProblemName(hProb, problemName, MAX_NAME_LEN)
    'problemName = Left(problemName, length)
    
    solutionStatus = CoinGetSolutionStatus(hProb)
    
    solutionText = String(MAX_NAME_LEN, vbNullChar)
    length = CoinGetSolutionTextBuf(hProb, solutionStatus, solutionText, MAX_NAME_LEN)
    solutionText = Left(solutionText, length)
    
    objectValue = CoinGetObjectValue(hProb)
      
    LogWriteLine "---------------------------------------------------------------"
    LogWriteLine "Problem Name:    " & problemName
    LogWriteLine "Solution Result: " & solutionText
    LogWriteLine "Solution Status: " & solutionStatus
    LogWriteLine "Optimal Value:   " & objectValue & " (" & optimalValue & ")"
    LogWriteLine "---------------------------------------------------------------"
    
    colCount = CoinGetColCount(hProb)
    rowCount = CoinGetRowCount(hProb)
    ReDim activity(colCount)
    ReDim reduced(colCount)
    ReDim slack(rowCount)
    ReDim shadow(rowCount)
    result = CoinGetSolutionValues(hProb, activity(0), reduced(0), slack(0), shadow(0))
    For i = 0 To colCount - 1
        If activity(i) <> 0# Then
            colName = String(MAX_NAME_LEN, vbNullChar)
            length = CoinGetColNameBuf(hProb, i, colName, MAX_NAME_LEN)
            colName = Left(colName, length)
            LogWriteLine colName & " = " & activity(i)
        End If
    Next i
    LogWriteLine "---------------------------------------------------------------"
    LogWriteLine ""
    
    result = CoinUnloadProblem(hProb)
          
End Sub


