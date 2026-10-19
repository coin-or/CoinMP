Attribute VB_Name = "SolveExMip1"
Option Explicit



Public Sub SolveProblemExMip1()

Const NUM_COLS As Long = 8
Const NUM_ROWS As Long = 5
Const NUM_NZ As Long = 14
Const NUM_RNG As Long = 2

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


problemName = "ExMip1"

colCount = NUM_COLS
rowCount = NUM_ROWS
nonZeroCount = NUM_NZ
rangeCount = NUM_RNG

objectName = "z"
objectSense = SOLV_OBJSENS_MIN
objectConst = 0#

objectCoeffs(0) = 1#
objectCoeffs(1) = 0
objectCoeffs(2) = 0
objectCoeffs(3) = 0
objectCoeffs(4) = 2
objectCoeffs(5) = 0
objectCoeffs(6) = 0
objectCoeffs(7) = -1#

lowerBounds(0) = 2.5
lowerBounds(1) = 0
lowerBounds(2) = 0
lowerBounds(3) = 0
lowerBounds(4) = 0.5
lowerBounds(5) = 0
lowerBounds(6) = 0
lowerBounds(7) = 0

upperBounds(0) = 1E+37
upperBounds(1) = 4.1
upperBounds(2) = 1E+37
upperBounds(3) = 1E+37
upperBounds(4) = 4
upperBounds(5) = 1E+37
upperBounds(6) = 1E+37
upperBounds(7) = 4.3

rowType(0) = Asc("G")
rowType(1) = Asc("L")
rowType(2) = Asc("E")
rowType(3) = Asc("G")
rowType(4) = Asc("L")

rhsValues(0) = 2.5
rhsValues(1) = 2.1
rhsValues(2) = 4
rhsValues(3) = 1.8
rhsValues(4) = 15

rangeValues(0) = 0
rangeValues(1) = 0
rangeValues(2) = 0
rangeValues(3) = -3.2
rangeValues(4) = 12

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

colNames(0) = "col01"
colNames(1) = "col02"
colNames(2) = "col03"
colNames(3) = "col04"
colNames(4) = "col05"
colNames(5) = "col06"
colNames(6) = "col07"
colNames(7) = "col08"

'colNamesBuf = "col01" & vbNullChar & _
'           "col02" & vbNullChar & _
'           "col03" & vbNullChar & _
'           "col04" & vbNullChar & _
'           "col05" & vbNullChar & _
'           "col06" & vbNullChar & _
'           "col07" & vbNullChar & _
'           "col08" & vbNullChar

rowNames(0) = "row01"
rowNames(1) = "row02"
rowNames(2) = "row03"
rowNames(3) = "row04"
rowNames(4) = "row05"

'rowNamesBuf = "row01" & vbNullChar & _
'           "row02" & vbNullChar & _
'           "row03" & vbNullChar & _
'           "row04" & vbNullChar & _
'           "row05" & vbNullChar

colNamesBuf = CoinGenerateNamesBuf(colNames, NUM_COLS)
rowNamesBuf = CoinGenerateNamesBuf(rowNames, NUM_ROWS)

colType(0) = Asc("C")
colType(1) = Asc("C")
colType(2) = Asc("B")
colType(3) = Asc("B")
colType(4) = Asc("C")
colType(5) = Asc("C")
colType(6) = Asc("C")
colType(7) = Asc("C")

optimalValue = 3.23684210526

RunTestProblem problemName, optimalValue, colCount, rowCount, _
    nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs(0), _
    lowerBounds(0), upperBounds(0), rowType(0), rhsValues(0), rangeValues(0), _
    matrixBegin(0), matrixCount(0), matrixIndex(0), matrixValues(0), _
    colNames, rowNames, objectName, 0&, colType(0)

End Sub
