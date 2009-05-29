Attribute VB_Name = "SolveCoinTest"
Option Explicit

    
'*************************************************************************
'*   This problem is copied from coinutils\test\CoinPackedMatrixTest
'*
'*    3x1 +  x2         -  2x4 - x5               -    x8
'*          2x2 + 1.1x3
'*                   x3              +  x6
'*                       2.8x4             -1.2x7
'*  5.6x1                      + x5               + 1.9x8
'*
'*************************************************************************


Public Sub SolveProblemCoinTest()

Const NUM_COLS As Long = 8
Const NUM_ROWS As Long = 5
Const NUM_NZ As Long = 14
    
Dim problemName As String
Dim colCount As Long
Dim rowCount As Long
Dim nonZeroCount As Long
Dim rangeCount As Long

Dim objectName As String
Dim objectSense As Long
Dim objectConst As Double
Dim objectCoeffs(NUM_COLS) As Double

Dim lowerBounds(NUM_COLS) As Double
Dim upperBounds(NUM_COLS) As Double

Dim rowType(NUM_ROWS) As Byte
Dim rhsValues(NUM_ROWS) As Double
Dim rangeValues(NUM_ROWS) As Double

Dim matrixBegin(NUM_COLS + 1) As Long
Dim matrixCount(NUM_COLS) As Long
Dim matrixIndex(NUM_NZ) As Long
Dim matrixValues(NUM_NZ) As Double

Dim colNames(NUM_COLS) As String
Dim rowNames(NUM_ROWS) As String

Dim colNamesBuf As String
Dim rowNamesBuf As String

Dim InitValues(NUM_COLS) As Double

Dim colType(NUM_COLS) As Byte

Dim optimalValue As Double


problemName = "CoinTest"

colCount = NUM_COLS
rowCount = NUM_ROWS
nonZeroCount = NUM_NZ
rangeCount = 0

objectName = "obj"
objectSense = SOLV_OBJSENS_MAX
objectConst = 0#

objectCoeffs(0) = 1#
objectCoeffs(1) = 1#
objectCoeffs(2) = 1#
objectCoeffs(3) = 1#
objectCoeffs(4) = 1#
objectCoeffs(5) = 1#
objectCoeffs(6) = 1#
objectCoeffs(7) = 1#

lowerBounds(0) = 0
lowerBounds(1) = 0
lowerBounds(2) = 0
lowerBounds(3) = 0
lowerBounds(4) = 0
lowerBounds(5) = 0
lowerBounds(6) = 0
lowerBounds(7) = 0

upperBounds(0) = 1000000
upperBounds(1) = 1000000
upperBounds(2) = 1000000
upperBounds(3) = 1000000
upperBounds(4) = 1000000
upperBounds(5) = 1000000
upperBounds(6) = 1000000
upperBounds(7) = 1000000
    
rowType(0) = Asc("L")
rowType(1) = Asc("L")
rowType(2) = Asc("L")
rowType(3) = Asc("L")
rowType(4) = Asc("L")

rhsValues(0) = 14
rhsValues(1) = 80
rhsValues(2) = 50
rhsValues(3) = 50
rhsValues(4) = 50

matrixBegin(0) = 0
matrixBegin(1) = 2
matrixBegin(2) = 4
matrixBegin(3) = 6
matrixBegin(4) = 8
matrixBegin(5) = 10
matrixBegin(6) = 11
matrixBegin(7) = 12
matrixBegin(8) = 14
    
matrixCount(0) = 2
matrixCount(1) = 2
matrixCount(2) = 2
matrixCount(3) = 2
matrixCount(4) = 2
matrixCount(5) = 1
matrixCount(6) = 1
matrixCount(7) = 2

matrixIndex(0) = 0
matrixIndex(1) = 4
matrixIndex(2) = 0
matrixIndex(3) = 1
matrixIndex(4) = 1
matrixIndex(5) = 2
matrixIndex(6) = 0
matrixIndex(7) = 3
matrixIndex(8) = 0
matrixIndex(9) = 4
matrixIndex(10) = 2
matrixIndex(11) = 3
matrixIndex(12) = 0
matrixIndex(13) = 4

matrixValues(0) = 3
matrixValues(1) = 5.6
matrixValues(2) = 1
matrixValues(3) = 2
matrixValues(4) = 1.1
matrixValues(5) = 1
matrixValues(6) = -2
matrixValues(7) = 2.8
matrixValues(8) = -1
matrixValues(9) = 1
matrixValues(10) = 1
matrixValues(11) = -1.2
matrixValues(12) = -1
matrixValues(13) = 1.9

colNames(0) = "c1"
colNames(1) = "c2"
colNames(2) = "c3"
colNames(3) = "c4"
colNames(4) = "c5"
colNames(5) = "c6"
colNames(6) = "c7"
colNames(7) = "c8"

'colNamesBuf = "c1" & vbNullChar & _
'           "c2" & vbNullChar & _
'           "c3" & vbNullChar & _
'           "c4" & vbNullChar & _
'           "c5" & vbNullChar & _
'           "c6" & vbNullChar & _
'           "c7" & vbNullChar & _
'           "c8" & vbNullChar

rowNames(0) = "r1"
rowNames(1) = "r2"
rowNames(2) = "r3"
rowNames(3) = "r4"
rowNames(4) = "r5"

'rowNamesBuf = "r1" & vbNullChar & _
'           "r2" & vbNullChar & _
'           "r3" & vbNullChar & _
'           "r4" & vbNullChar & _
'           "r5" & vbNullChar
    
colNamesBuf = CoinGenerateNamesBuf(colNames, NUM_COLS)
rowNamesBuf = CoinGenerateNamesBuf(rowNames, NUM_ROWS)

optimalValue = 1428729.2857143

RunTestProblem problemName, optimalValue, colCount, rowCount, _
    nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs(0), _
    lowerBounds(0), upperBounds(0), rowType(0), rhsValues(0), 0&, _
    matrixBegin(0), matrixCount(0), matrixIndex(0), matrixValues(0), _
    colNames, rowNames, objectName, 0&, 0&
    
End Sub




