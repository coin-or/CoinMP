Attribute VB_Name = "SolveAfiro"
Option Explicit

    

Public Sub SolveProblemAfiro()

Const NUM_COLS As Long = 32
Const NUM_ROWS As Long = 27
Const NUM_NZ As Long = 83
Const NUM_RNG As Long = 0
    
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


problemName = "Afiro"

colCount = NUM_COLS
rowCount = NUM_ROWS
nonZeroCount = NUM_NZ
rangeCount = NUM_RNG

objectName = "Cost"
objectSense = SOLV_OBJSENS_MIN
objectConst = 0#

objectCoeffs(0) = 0
objectCoeffs(1) = -0.4
objectCoeffs(2) = 0
objectCoeffs(3) = 0
objectCoeffs(4) = 0
objectCoeffs(5) = 0
objectCoeffs(6) = 0
objectCoeffs(7) = 0
objectCoeffs(8) = 0
objectCoeffs(9) = 0
objectCoeffs(10) = 0
objectCoeffs(11) = 0
objectCoeffs(12) = -0.32
objectCoeffs(13) = 0
objectCoeffs(14) = 0
objectCoeffs(15) = 0
objectCoeffs(16) = -0.6
objectCoeffs(17) = 0
objectCoeffs(18) = 0
objectCoeffs(19) = 0
objectCoeffs(20) = 0
objectCoeffs(21) = 0
objectCoeffs(22) = 0
objectCoeffs(23) = 0
objectCoeffs(24) = 0
objectCoeffs(25) = 0
objectCoeffs(26) = 0
objectCoeffs(27) = 0
objectCoeffs(28) = -0.48
objectCoeffs(29) = 0
objectCoeffs(30) = 0
objectCoeffs(31) = 10

lowerBounds(0) = 0
lowerBounds(1) = 0
lowerBounds(2) = 0
lowerBounds(3) = 0
lowerBounds(4) = 0
lowerBounds(5) = 0
lowerBounds(6) = 0
lowerBounds(7) = 0
lowerBounds(8) = 0
lowerBounds(9) = 0
lowerBounds(10) = 0
lowerBounds(11) = 0
lowerBounds(12) = 0
lowerBounds(13) = 0
lowerBounds(14) = 0
lowerBounds(15) = 0
lowerBounds(16) = 0
lowerBounds(17) = 0
lowerBounds(18) = 0
lowerBounds(19) = 0
lowerBounds(20) = 0
lowerBounds(21) = 0
lowerBounds(22) = 0
lowerBounds(23) = 0
lowerBounds(24) = 0
lowerBounds(25) = 0
lowerBounds(26) = 0
lowerBounds(27) = 0
lowerBounds(28) = 0
lowerBounds(29) = 0
lowerBounds(30) = 0
lowerBounds(31) = 0

upperBounds(0) = 1E+37
upperBounds(1) = 1E+37
upperBounds(2) = 1E+37
upperBounds(3) = 1E+37
upperBounds(4) = 1E+37
upperBounds(5) = 1E+37
upperBounds(6) = 1E+37
upperBounds(7) = 1E+37
upperBounds(8) = 1E+37
upperBounds(9) = 1E+37
upperBounds(10) = 1E+37
upperBounds(11) = 1E+37
upperBounds(12) = 1E+37
upperBounds(13) = 1E+37
upperBounds(14) = 1E+37
upperBounds(15) = 1E+37
upperBounds(16) = 1E+37
upperBounds(17) = 1E+37
upperBounds(18) = 1E+37
upperBounds(19) = 1E+37
upperBounds(20) = 1E+37
upperBounds(21) = 1E+37
upperBounds(22) = 1E+37
upperBounds(23) = 1E+37
upperBounds(24) = 1E+37
upperBounds(25) = 1E+37
upperBounds(26) = 1E+37
upperBounds(27) = 1E+37
upperBounds(28) = 1E+37
upperBounds(29) = 1E+37
upperBounds(30) = 1E+37
upperBounds(31) = 1E+37
    
rowType(0) = Asc("E")
rowType(1) = Asc("E")
rowType(2) = Asc("L")
rowType(3) = Asc("L")
rowType(4) = Asc("E")
rowType(5) = Asc("E")
rowType(6) = Asc("L")
rowType(7) = Asc("L")
rowType(8) = Asc("L")
rowType(9) = Asc("L")
rowType(10) = Asc("E")
rowType(11) = Asc("E")
rowType(12) = Asc("L")
rowType(13) = Asc("L")
rowType(14) = Asc("E")
rowType(15) = Asc("E")
rowType(16) = Asc("L")
rowType(17) = Asc("L")
rowType(18) = Asc("L")
rowType(19) = Asc("L")
rowType(20) = Asc("L")
rowType(21) = Asc("L")
rowType(22) = Asc("L")
rowType(23) = Asc("L")
rowType(24) = Asc("L")
rowType(25) = Asc("L")
rowType(26) = Asc("L")

rhsValues(0) = 0
rhsValues(1) = 0
rhsValues(2) = 80
rhsValues(3) = 0
rhsValues(4) = 0
rhsValues(5) = 0
rhsValues(6) = 80
rhsValues(7) = 0
rhsValues(8) = 0
rhsValues(9) = 0
rhsValues(10) = 0
rhsValues(11) = 0
rhsValues(12) = 500
rhsValues(13) = 0
rhsValues(14) = 0
rhsValues(15) = 44
rhsValues(16) = 500
rhsValues(17) = 0
rhsValues(18) = 0
rhsValues(19) = 0
rhsValues(20) = 0
rhsValues(21) = 0
rhsValues(22) = 0
rhsValues(23) = 0
rhsValues(24) = 0
rhsValues(25) = 310
rhsValues(26) = 300

matrixBegin(0) = 0
matrixBegin(1) = 4
matrixBegin(2) = 6
matrixBegin(3) = 8
matrixBegin(4) = 10
matrixBegin(5) = 14
matrixBegin(6) = 18
matrixBegin(7) = 22
matrixBegin(8) = 26
matrixBegin(9) = 28
matrixBegin(10) = 30
matrixBegin(11) = 32
matrixBegin(12) = 34
matrixBegin(13) = 36
matrixBegin(14) = 38
matrixBegin(15) = 40
matrixBegin(16) = 44
matrixBegin(17) = 46
matrixBegin(18) = 48
matrixBegin(19) = 50
matrixBegin(20) = 52
matrixBegin(21) = 56
matrixBegin(22) = 60
matrixBegin(23) = 64
matrixBegin(24) = 68
matrixBegin(25) = 70
matrixBegin(26) = 72
matrixBegin(27) = 74
matrixBegin(28) = 76
matrixBegin(29) = 78
matrixBegin(30) = 80
matrixBegin(31) = 82
matrixBegin(32) = 83
    
matrixCount(0) = 4
matrixCount(1) = 2
matrixCount(2) = 2
matrixCount(3) = 2
matrixCount(4) = 4
matrixCount(5) = 4
matrixCount(6) = 4
matrixCount(7) = 4
matrixCount(8) = 2
matrixCount(9) = 2
matrixCount(10) = 2
matrixCount(11) = 2
matrixCount(12) = 2
matrixCount(13) = 2
matrixCount(14) = 2
matrixCount(15) = 4
matrixCount(16) = 2
matrixCount(17) = 2
matrixCount(18) = 2
matrixCount(19) = 2
matrixCount(20) = 4
matrixCount(21) = 4
matrixCount(22) = 4
matrixCount(23) = 4
matrixCount(24) = 2
matrixCount(25) = 2
matrixCount(26) = 2
matrixCount(27) = 2
matrixCount(28) = 2
matrixCount(29) = 2
matrixCount(30) = 2
matrixCount(31) = 1

matrixIndex(0) = 0
matrixIndex(1) = 1
matrixIndex(2) = 2
matrixIndex(3) = 23
matrixIndex(4) = 0
matrixIndex(5) = 3
matrixIndex(6) = 0
matrixIndex(7) = 21
matrixIndex(8) = 1
matrixIndex(9) = 25
matrixIndex(10) = 4
matrixIndex(11) = 5
matrixIndex(12) = 6
matrixIndex(13) = 24
matrixIndex(14) = 4
matrixIndex(15) = 5
matrixIndex(16) = 7
matrixIndex(17) = 24
matrixIndex(18) = 4
matrixIndex(19) = 5
matrixIndex(20) = 8
matrixIndex(21) = 24
matrixIndex(22) = 4
matrixIndex(23) = 5
matrixIndex(24) = 9
matrixIndex(25) = 24
matrixIndex(26) = 6
matrixIndex(27) = 20
matrixIndex(28) = 7
matrixIndex(29) = 20
matrixIndex(30) = 8
matrixIndex(31) = 20
matrixIndex(32) = 9
matrixIndex(33) = 20
matrixIndex(34) = 3
matrixIndex(35) = 4
matrixIndex(36) = 4
matrixIndex(37) = 22
matrixIndex(38) = 5
matrixIndex(39) = 26
matrixIndex(40) = 10
matrixIndex(41) = 11
matrixIndex(42) = 12
matrixIndex(43) = 21
matrixIndex(44) = 10
matrixIndex(45) = 13
matrixIndex(46) = 10
matrixIndex(47) = 23
matrixIndex(48) = 10
matrixIndex(49) = 20
matrixIndex(50) = 11
matrixIndex(51) = 25
matrixIndex(52) = 14
matrixIndex(53) = 15
matrixIndex(54) = 16
matrixIndex(55) = 22
matrixIndex(56) = 14
matrixIndex(57) = 15
matrixIndex(58) = 17
matrixIndex(59) = 22
matrixIndex(60) = 14
matrixIndex(61) = 15
matrixIndex(62) = 18
matrixIndex(63) = 22
matrixIndex(64) = 14
matrixIndex(65) = 15
matrixIndex(66) = 19
matrixIndex(67) = 22
matrixIndex(68) = 16
matrixIndex(69) = 20
matrixIndex(70) = 17
matrixIndex(71) = 20
matrixIndex(72) = 18
matrixIndex(73) = 20
matrixIndex(74) = 19
matrixIndex(75) = 20
matrixIndex(76) = 13
matrixIndex(77) = 15
matrixIndex(78) = 15
matrixIndex(79) = 24
matrixIndex(80) = 14
matrixIndex(81) = 26
matrixIndex(82) = 15

matrixValues(0) = -1
matrixValues(1) = -1.06
matrixValues(2) = 1
matrixValues(3) = 0.301
matrixValues(4) = 1
matrixValues(5) = -1
matrixValues(6) = 1
matrixValues(7) = -1
matrixValues(8) = 1
matrixValues(9) = 1
matrixValues(10) = -1
matrixValues(11) = -1.06
matrixValues(12) = 1
matrixValues(13) = 0.301
matrixValues(14) = -1
matrixValues(15) = -1.06
matrixValues(16) = 1
matrixValues(17) = 0.313
matrixValues(18) = -1
matrixValues(19) = -0.96
matrixValues(20) = 1
matrixValues(21) = 0.313
matrixValues(22) = -1
matrixValues(23) = -0.86
matrixValues(24) = 1
matrixValues(25) = 0.326
matrixValues(26) = -1
matrixValues(27) = 2.364
matrixValues(28) = -1
matrixValues(29) = 2.386
matrixValues(30) = -1
matrixValues(31) = 2.408
matrixValues(32) = -1
matrixValues(33) = 2.429
matrixValues(34) = 1.4
matrixValues(35) = 1
matrixValues(36) = 1
matrixValues(37) = -1
matrixValues(38) = 1
matrixValues(39) = 1
matrixValues(40) = -1
matrixValues(41) = -0.43
matrixValues(42) = 1
matrixValues(43) = 0.109
matrixValues(44) = 1
matrixValues(45) = -1
matrixValues(46) = 1
matrixValues(47) = -1
matrixValues(48) = 1
matrixValues(49) = -1
matrixValues(50) = 1
matrixValues(51) = 1
matrixValues(52) = -0.43
matrixValues(53) = 1
matrixValues(54) = 1
matrixValues(55) = 0.109
matrixValues(56) = -0.43
matrixValues(57) = 1
matrixValues(58) = 1
matrixValues(59) = 0.108
matrixValues(60) = -0.39
matrixValues(61) = 1
matrixValues(62) = 1
matrixValues(63) = 0.108
matrixValues(64) = -0.37
matrixValues(65) = 1
matrixValues(66) = 1
matrixValues(67) = 0.107
matrixValues(68) = -1
matrixValues(69) = 2.191
matrixValues(70) = -1
matrixValues(71) = 2.219
matrixValues(72) = -1
matrixValues(73) = 2.249
matrixValues(74) = -1
matrixValues(75) = 2.279
matrixValues(76) = 1.4
matrixValues(77) = -1
matrixValues(78) = 1
matrixValues(79) = -1
matrixValues(80) = 1
matrixValues(81) = 1
matrixValues(82) = 1

colNames(0) = "x01"
colNames(1) = "x02"
colNames(2) = "x03"
colNames(3) = "x04"
colNames(4) = "x06"
colNames(5) = "x07"
colNames(6) = "x08"
colNames(7) = "x09"
colNames(8) = "x10"
colNames(9) = "x11"
colNames(10) = "x12"
colNames(11) = "x13"
colNames(12) = "x14"
colNames(13) = "x15"
colNames(14) = "x16"
colNames(15) = "x22"
colNames(16) = "x23"
colNames(17) = "x24"
colNames(18) = "x25"
colNames(19) = "x26"
colNames(20) = "x28"
colNames(21) = "x29"
colNames(22) = "x30"
colNames(23) = "x31"
colNames(24) = "x32"
colNames(25) = "x33"
colNames(26) = "x34"
colNames(27) = "x35"
colNames(28) = "x36"
colNames(29) = "x37"
colNames(30) = "x38"
colNames(31) = "x39"

'colNamesBuf = "x01" & vbNullChar & "x02" & vbNullChar & _
'           "x03" & vbNullChar & "x04" & vbNullChar & _
'           "x06" & vbNullChar & "x07" & vbNullChar & _
'           "x08" & vbNullChar & "x09" & vbNullChar & _
'           "x10" & vbNullChar & "x11" & vbNullChar & _
'           "x12" & vbNullChar & "x13" & vbNullChar & _
'           "x14" & vbNullChar & "x15" & vbNullChar & _
'           "x16" & vbNullChar & _
'           "x22" & vbNullChar & "x23" & vbNullChar & _
'           "x24" & vbNullChar & "x25" & vbNullChar & _
'           "x26" & vbNullChar & _
'           "x28" & vbNullChar & "x29" & vbNullChar & _
'           "x30" & vbNullChar & "x31" & vbNullChar & _
'           "x32" & vbNullChar & "x33" & vbNullChar & _
'           "x34" & vbNullChar & "x35" & vbNullChar & _
'           "x36" & vbNullChar & "x37" & vbNullChar & _
'           "x38" & vbNullChar & "x39" & vbNullChar

rowNames(0) = "r09"
rowNames(1) = "r10"
rowNames(2) = "x05"
rowNames(3) = "x21"
rowNames(4) = "r12"
rowNames(5) = "r13"
rowNames(6) = "x17"
rowNames(7) = "x18"
rowNames(8) = "x19"
rowNames(9) = "x20"
rowNames(10) = "r19"
rowNames(11) = "r20"
rowNames(12) = "x27"
rowNames(13) = "x44"
rowNames(14) = "r22"
rowNames(15) = "r23"
rowNames(16) = "x40"
rowNames(17) = "x41"
rowNames(18) = "x42"
rowNames(19) = "x43"
rowNames(20) = "x45"
rowNames(21) = "x46"
rowNames(22) = "x47"
rowNames(23) = "x48"
rowNames(24) = "x49"
rowNames(25) = "x50"
rowNames(26) = "x51"

'rowNamesBuf = "r09" & vbNullChar & "r10" & vbNullChar & _
'           "x05" & vbNullChar & "x21" & vbNullChar & _
'           "r12" & vbNullChar & "r13" & vbNullChar & _
'           "x17" & vbNullChar & "x18" & vbNullChar & _
'           "x19" & vbNullChar & "x20" & vbNullChar & _
'           "r19" & vbNullChar & "r20" & vbNullChar & _
'           "x27" & vbNullChar & "x44" & vbNullChar & _
'           "r22" & vbNullChar & "r23" & vbNullChar & _
'           "x40" & vbNullChar & "x41" & vbNullChar & _
'           "x42" & vbNullChar & "x43" & vbNullChar & _
'           "x45" & vbNullChar & "x46" & vbNullChar & _
'           "x47" & vbNullChar & "x48" & vbNullChar & _
'           "x49" & vbNullChar & "x50" & vbNullChar & _
'           "x51" & vbNullChar

colNamesBuf = CoinGenerateNamesBuf(colNames, NUM_COLS)
rowNamesBuf = CoinGenerateNamesBuf(rowNames, NUM_ROWS)

optimalValue = -464.753142857

RunTestProblem problemName, optimalValue, colCount, rowCount, _
    nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs(0), _
    lowerBounds(0), upperBounds(0), rowType(0), rhsValues(0), 0&, _
    matrixBegin(0), matrixCount(0), matrixIndex(0), matrixValues(0), _
    colNames, rowNames, objectName, 0&, 0&
    
End Sub








