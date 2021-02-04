Attribute VB_Name = "SolveBakery"
Option Explicit


Public Sub SolveProblemBakery()

Const NUMCOLS As Long = 2
Const NUMROWS As Long = 3
Const NUMNZ As Long = 4

Dim problemName As String
Dim colCount As Long
Dim rowCount As Long
Dim nonZeroCount As Long
Dim rangeCount As Long

Dim objectName As String
Dim objectSense As Long
Dim objectConst As Double
Dim objectCoeffs(NUMCOLS) As Double

Dim lowerBounds(NUMCOLS) As Double
Dim upperBounds(NUMCOLS) As Double

Dim rowType(NUMROWS) As Byte
Dim rhsValues(NUMROWS) As Double
Dim rangeValues(NUMROWS) As Double

Dim matrixBegin(NUMCOLS + 1) As Long
Dim matrixCount(NUMCOLS) As Long
Dim matrixIndex(NUMNZ) As Long
Dim matrixValues(NUMNZ) As Double

Dim colNames(NUMCOLS) As String
Dim rowNames(NUMROWS) As String

Dim colNamesBuf As String
Dim rowNamesBuf As String

Dim colNamesBuf2 As String
Dim rowNamesBuf2 As String

Dim InitValues(NUMCOLS) As Double

Dim colType(NUMCOLS) As Byte

Dim optimalValue As Double


problemName = "Bakery"

colCount = NUMCOLS
rowCount = NUMROWS
nonZeroCount = NUMNZ
rangeCount = 0

objectName = "Profit"
objectSense = SOLV_OBJSENS_MAX
objectConst = -4000# / 30#

objectCoeffs(0) = 0.05
objectCoeffs(1) = 0.08

lowerBounds(0) = 0
lowerBounds(1) = 0

upperBounds(0) = 1000000
upperBounds(1) = 1000000

rowType(0) = Asc("L")
rowType(1) = Asc("L")
rowType(2) = Asc("L")

rhsValues(0) = 1400
rhsValues(1) = 8000
rhsValues(2) = 5000

matrixBegin(0) = 0
matrixBegin(1) = 2
matrixBegin(2) = 4

matrixCount(0) = 2
matrixCount(1) = 2

matrixIndex(0) = 0
matrixIndex(1) = 1
matrixIndex(2) = 0
matrixIndex(3) = 2

matrixValues(0) = 0.1
matrixValues(1) = 1
matrixValues(2) = 0.2
matrixValues(3) = 1

colNames(0) = "Sun"
colNames(1) = "Moon"

'colNamesBuf = "Sun" & vbNullChar & _
'              "Moon" & vbNullChar

rowNames(0) = "c1"
rowNames(1) = "c2"
rowNames(2) = "c3"

'rowNamesBuf = "c1" & vbNullChar & _
'              "c2" & vbNullChar & _
'              "c3" & vbNullChar

colNamesBuf = CoinGenerateNamesBuf(colNames, NUMCOLS)
rowNamesBuf = CoinGenerateNamesBuf(rowNames, NUMROWS)

optimalValue = 506.66666667

RunTestProblem problemName, optimalValue, colCount, rowCount, _
    nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs(0), _
    lowerBounds(0), upperBounds(0), rowType(0), rhsValues(0), 0&, _
    matrixBegin(0), matrixCount(0), matrixIndex(0), matrixValues(0), _
    colNames, rowNames, objectName, 0&, 0&

End Sub
