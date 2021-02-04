Attribute VB_Name = "LibCoinMP"
Option Explicit

Public Const SOLV_CALL_SUCCESS   As Long = 0
Public Const SOLV_CALL_FAILED    As Long = -1

Public Const SOLV_METHOD_DEFAULT As Long = 0

Public Const SOLV_METHOD_PRIMAL  As Long = &H1
Public Const SOLV_METHOD_DUAL    As Long = &H2
Public Const SOLV_METHOD_NETWORK As Long = &H4
Public Const SOLV_METHOD_BARRIER As Long = &H8

Public Const SOLV_METHOD_BENDERS As Long = &H100
Public Const SOLV_METHOD_DEQ     As Long = &H200
Public Const SOLV_METHOD_EV      As Long = &H400


Public Const SOLV_FEATURE_LP     As Long = &H1
Public Const SOLV_FEATURE_QP     As Long = &H2
Public Const SOLV_FEATURE_QCP    As Long = &H4
Public Const SOLV_FEATURE_NLP    As Long = &H8

Public Const SOLV_FEATURE_MIP    As Long = &H10
Public Const SOLV_FEATURE_MIQP   As Long = &H20
Public Const SOLV_FEATURE_MIQCP  As Long = &H40
Public Const SOLV_FEATURE_MINLP  As Long = &H80

Public Const SOLV_FEATURE_SP     As Long = &H10000

Public Const SOLV_OBJSENS_MAX    As Long = -1
Public Const SOLV_OBJSENS_MIN    As Long = 1


Public Const SOLV_FILE_LOG       As Long = 0
Public Const SOLV_FILE_BASIS     As Long = 1
Public Const SOLV_FILE_MIPSTART  As Long = 2
Public Const SOLV_FILE_MPS       As Long = 3
Public Const SOLV_FILE_LP        As Long = 4
Public Const SOLV_FILE_BINARY    As Long = 5
Public Const SOLV_FILE_OUTPUT    As Long = 6
Public Const SOLV_FILE_BINOUT    As Long = 7
Public Const SOLV_FILE_IIS       As Long = 8


Public Declare Function CoinInitSolver Lib "CoinMP.dll" (ByVal licenseStr As String) As Long
Public Declare Function CoinFreeSolver Lib "CoinMP.dll" () As Long

Public Declare Function CoinGetSolverNameBuf Lib "CoinMP.dll" (ByVal solverName As String, ByVal buflen As Long) As Long
Public Declare Function CoinGetVersionStrBuf Lib "CoinMP.dll" (ByVal versionStr As String, ByVal buflen As Long) As Long
Public Declare Function CoinGetVersion Lib "CoinMP.dll" () As Double
Public Declare Function CoinGetFeatures Lib "CoinMP.dll" () As Long
Public Declare Function CoinGetMethods Lib "CoinMP.dll" () As Long
Public Declare Function CoinGetInfinity Lib "CoinMP.dll" () As Double

Public Declare Function CoinCreateProblem Lib "CoinMP.dll" (ByVal problemName As String) As Long

Public Declare Function CoinLoadMatrix Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal colCount As Long, ByVal rowCount As Long, ByVal nonZeroCount As Long, ByVal rangeCount As Long, _
    ByVal objectSense As Long, ByVal objectConst As Double, ByRef objectCoeffs As Double, _
    ByRef lowerBounds As Double, ByRef upperBounds As Double, ByRef rowType As Byte, _
    ByRef rhsValues As Double, ByRef rangeValues As Double, ByRef matrixBegin As Long, _
    ByRef matrixCount As Long, ByRef matrixIndex As Long, ByRef matrixValues As Double) As Integer

Public Declare Function CoinLoadNamesBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal colNamesBuf As String, ByVal rowNamesBuf As String, ByVal objName As String) As Integer

Public Declare Function CoinLoadProblemBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal colCount As Long, ByVal rowCount As Long, ByVal nonZeroCount As Long, ByVal rangeCount As Long, _
    ByVal objectSense As Long, ByVal objectConst As Double, ByRef objectCoeffs As Double, _
    ByRef lowerBounds As Double, ByRef upperBounds As Double, ByRef rowType As Byte, _
    ByRef rhsValues As Double, ByRef rangeValues As Double, ByRef matrixBegin As Long, _
    ByRef matrixCount As Long, ByRef matrixIndex As Long, ByRef matrixValues As Double, _
    ByVal colNamesBuf As String, ByVal rowNamesBuf As String, ByVal objName As String) As Integer

Public Declare Function CoinLoadInitValues Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef InitValues As Double) As Long

Public Declare Function CoinLoadInteger Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef ColumnType As Byte) As Long

Public Declare Function CoinLoadPriority Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal PriorCount As Long, ByRef PriorIndex As Long, _
    ByRef PriorValues As Long, ByRef BranchDir As Long) As Long

Public Declare Function CoinLoadSos Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal SosCount As Long, ByVal SosNZCount As Long, _
    ByRef SosType As Long, ByRef SosPrior As Long, ByRef SosBegin As Long, _
    ByRef SosIndex As Long, ByRef SosRef As Double) As Long

Public Declare Function CoinLoadSemiCont Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal SemiCount As Long, ByRef SemiIndex As Long) As Long

Public Declare Function CoinLoadQuadratic Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef QuadBegin As Long, ByRef QuadCount As Long, _
    ByRef QuadIndex As Long, ByRef QuadValues As Double) As Long

Public Declare Function CoinLoadNonlinear Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal NlpTreeCount As Long, ByVal NlpLineCount As Long, _
    ByRef NlpBegin As Long, ByRef NlpOper As Long, ByRef NlpArg1 As Long, _
    ByRef NlpArg2 As Long, ByRef NlpIndex1 As Long, ByRef NlpIndex2 As Long, _
    ByRef NlpValue1 As Double, ByRef NlpValue2 As Double) As Long

Public Declare Function CoinUnloadProblem Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinCheckProblem Lib "CoinMP.dll" (ByVal hProb As Long) As Long


Public Declare Function CoinGetProblemNameBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal ProbName As String, ByVal buflen As Long) As Long

Public Declare Function CoinGetColCount Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinGetRowCount Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinGetColNameBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal col As Long, ByVal colName As String, ByVal buflen As Long) As Long

Public Declare Function CoinGetRowNameBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal row As Long, ByVal rowName As String, ByVal buflen As Long) As Long


Public Declare Function CoinSetMsgLogCallback Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal MsgLogCallback As Long) As Long

Public Declare Function CoinSetIterCallback Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal IterCallback As Long) As Long

Public Declare Function CoinSetMipNodeCallback Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal MipNodeCallback As Long) As Long


Public Declare Function CoinOptimizeProblem Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal Method As Long) As Long


Public Declare Function CoinGetSolutionStatus Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinGetSolutionTextBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal solutionText As String, ByVal buflen As Long) As Long

Public Declare Function CoinGetObjectValue Lib "CoinMP.dll" (ByVal hProb As Long) As Double

Public Declare Function CoinGetMipBestBound Lib "CoinMP.dll" (ByVal hProb As Long) As Double

Public Declare Function CoinGetIterCount Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinGetMipNodeCount Lib "CoinMP.dll" (ByVal hProb As Long) As Long


Public Declare Function CoinGetSolutionValues Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef activity As Double, ByRef ReducedCost As Double, _
    ByRef SlackValues As Double, ByRef ShadowPrice As Double) As Long

Public Declare Function CoinGetSolutionRanges Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef ObjLoRange As Double, ByRef ObjUpRange As Double, _
    ByRef RhsLoRange As Double, ByRef RhsUpRange As Double) As Long

Public Declare Function CoinGetSolutionBasis Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByRef ColStatus As Long, ByRef RowStatus As Double) As Long


Public Declare Function CoinReadFile Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal FileType As Long, ByVal ReadFilename As String) As Long

Public Declare Function CoinWriteFile Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal FileType As Long, ByVal WriteFilename As String) As Long

Public Declare Function CoinOpenLogFile Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal LogFilename As String) As Long

Public Declare Function CoinCloseLogFile Lib "CoinMP.dll" (ByVal hProb As Long) As Long


Public Declare Function CoinGetOptionCount Lib "CoinMP.dll" (ByVal hProb As Long) As Long

Public Declare Function CoinLocateOptionID Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long

Public Declare Function CoinLocateOptionName Lib "CoinMP.dll" (ByVal hProb As Long, ByVal optionName As String) As Long


Public Declare Function CoinGetOptionID Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionNr As Long) As Long

Public Declare Function CoinGetOptionInfo Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionNr As Long, ByRef OptionID As Long, ByRef GroupType As Long, _
    ByRef OptionType As Long) As Long

Public Declare Function CoinGetIntOptionMinMax Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionNr As Long, ByRef MinValue As Long, ByRef MaxValue As Long) As Long

Public Declare Function CoinGetRealOptionMinMax Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionNr As Long, ByRef MinValue As Double, ByRef MaxValue As Double) As Long

Public Declare Function CoinGetOptionNamesBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionNr As Long, ByVal optionName As String, ByVal shortName As String, _
    ByVal buflen As Long) As Long


Public Declare Function CoinGetOptionGroup Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long

Public Declare Function CoinGetOptionType Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long

Public Declare Function CoinGetIntOptionDefaultValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long
Public Declare Function CoinGetIntOptionMaxValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long
Public Declare Function CoinGetIntOptionMinValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Long

Public Declare Function CoinGetRealOptionDefaultValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Double
Public Declare Function CoinGetRealOptionMaxValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Double
Public Declare Function CoinGetRealOptionMinValue Lib "CoinMP.dll" (ByVal hProb As Long, ByVal OptionID As Long) As Double

Public Declare Function CoinGetOptionNameBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, ByVal optionName As String, ByVal buflen As Long) As Long

Public Declare Function CoinGetOptionShortNameBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, ByVal shortName As String, ByVal buflen As Long) As Long


Public Declare Function CoinGetOptionChanged Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long) As Long

Public Declare Function CoinGetIntOption Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long) As Long

Public Declare Function CoinSetIntOption Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, IntValue As Long) As Long

Public Declare Function CoinGetRealOption Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long) As Double

Public Declare Function CoinSetRealOption Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, ByVal RealValue As Double) As Long

Public Declare Function CoinGetStringOptionBuf Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, ByVal StringValue As String, ByVal buflen As Long) As Long

Public Declare Function CoinSetStringOption Lib "CoinMP.dll" (ByVal hProb As Long, _
    ByVal OptionID As Long, ByVal StringValue As String) As Long


Public Function CoinGenerateNamesBuf(NamesList() As String, Count As Long) As String
    Dim i As Long
    Dim namesBuf As String

    If Count > 0 Then
        namesBuf = NamesList(0) + vbNullChar
        For i = 1 To Count - 1
            namesBuf = namesBuf + NamesList(i) + vbNullChar
        Next i
    End If
    CoinGenerateNamesBuf = namesBuf
End Function



Public Function CoinGetSolverName() As String
    Dim solverName As String
    Dim length As Long

    solverName = String(1000, vbNullChar)
    length = CoinGetSolverNameBuf(solverName, 1000)
    CoinGetSolverName = Left(solverName, length)
End Function


Public Function CoinGetVersionStr() As String
    Dim versionStr As String
    Dim length As Long

    versionStr = String(1000, vbNullChar)
    length = CoinGetVersionStrBuf(versionStr, 1000)
    CoinGetVersionStr = Left(versionStr, length)
End Function


Public Function CoinLoadNames(ByVal hProb As Long, _
    ByRef colNames() As String, ByRef rowNames() As String, ByVal objName As String) As Integer
    Dim colCount As Long
    Dim rowCount As Long

    colCount = CoinGetColCount(hProb)
    rowCount = CoinGetRowCount(hProb)
    CoinLoadNames = CoinLoadNamesBuf(hProb, CoinGenerateNamesBuf(colNames, colCount), _
        CoinGenerateNamesBuf(rowNames, rowCount), objName)
End Function


Public Function CoinLoadProblem(ByVal hProb As Long, _
    ByVal colCount As Long, ByVal rowCount As Long, _
    ByVal nonZeroCount As Long, ByVal rangeCount As Long, _
    ByVal objectSense As Long, ByVal objectConst As Double, ByRef objectCoeffs As Double, _
    ByRef lowerBounds As Double, ByRef upperBounds As Double, ByRef rowType As Byte, _
    ByRef rhsValues As Double, ByRef rangeValues As Double, ByRef matrixBegin As Long, _
    ByRef matrixCount As Long, ByRef matrixIndex As Long, ByRef matrixValues As Double, _
    ByRef colNames() As String, ByRef rowNames() As String, ByVal objName As String) As Integer

    CoinLoadProblem = CoinLoadProblemBuf(hProb, colCount, rowCount, nonZeroCount, rangeCount, _
                    objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds, rowType, _
                    rhsValues, rangeValues, matrixBegin, matrixCount, matrixIndex, matrixValues, _
                    CoinGenerateNamesBuf(colNames, colCount), CoinGenerateNamesBuf(rowNames, rowCount), _
                    objName)
End Function



Public Function CoinGetProblemName(ByVal hProb As Long) As String
    Dim problemName As String
    Dim length As Long

    problemName = String(1000, vbNullChar)
    length = CoinGetProblemNameBuf(hProb, problemName, 1000)
    CoinGetProblemName = Left(problemName, length)
End Function


Public Function CoinGetColName(ByVal hProb As Long, ByVal col As Long) As String
    Dim colName As String
    Dim length As Long

    colName = String(1000, vbNullChar)
    length = CoinGetColNameBuf(hProb, col, colName, 1000)
    CoinGetColName = Left(colName, length)
End Function


Public Function CoinGetRowName(ByVal hProb As Long, ByVal row As Long) As String
    Dim rowName As String
    Dim length As Long

    rowName = String(1000, vbNullChar)
    length = CoinGetRowNameBuf(hProb, row, rowName, 1000)
    CoinGetRowName = Left(rowName, length)
End Function


Public Function CoinGetSolutionText(ByVal hProb As Long) As String
    Dim solutionText As String
    Dim length As Long

    solutionText = String(1000, vbNullChar)
    length = CoinGetSolutionTextBuf(hProb, solutionText, 1000)
    CoinGetSolutionText = Left(solutionText, length)
End Function


Public Function CoinGetOptionName(ByVal hProb As Long, ByVal OptionID As Long) As String
    Dim optionName As String
    Dim length As Long

    optionName = String(1000, vbNullChar)
    length = CoinGetOptionNameBuf(hProb, OptionID, optionName, 1000)
    CoinGetOptionName = Left(optionName, length)
End Function


Public Function CoinGetOptionShortName(ByVal hProb As Long, ByVal OptionID As Long) As String
    Dim shortName As String
    Dim length As Long

    shortName = String(1000, vbNullChar)
    length = CoinGetOptionShortNameBuf(hProb, OptionID, shortName, 1000)
    CoinGetOptionShortName = Left(shortName, length)
End Function


Public Function CoinGetStringOption(ByVal hProb As Long, ByVal OptionID As Long) As String
    Dim stringOption As String
    Dim length As Long

    stringOption = String(1000, vbNullChar)
    length = CoinGetStringOptionBuf(hProb, OptionID, stringOption, 1000)
    CoinGetStringOption = Left(stringOption, length)
End Function
