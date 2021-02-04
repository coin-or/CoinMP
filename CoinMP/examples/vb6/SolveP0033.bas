Attribute VB_Name = "SolveP0033"
Option Explicit


Public Sub SolveProblemP0033()

Const NUM_COLS As Long = 33
Const NUM_ROWS As Long = 15
Const NUM_NZ As Long = 98
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


problemName = "P0033"

colCount = NUM_COLS
rowCount = NUM_ROWS
nonZeroCount = NUM_NZ
rangeCount = NUM_RNG

objectName = "Obj"
objectSense = SOLV_OBJSENS_MIN
objectConst = 0#

objectCoeffs(0) = 171
objectCoeffs(1) = 171
objectCoeffs(2) = 171
objectCoeffs(3) = 171
objectCoeffs(4) = 163
objectCoeffs(5) = 162
objectCoeffs(6) = 163
objectCoeffs(7) = 69
objectCoeffs(8) = 69
objectCoeffs(9) = 183
objectCoeffs(10) = 183
objectCoeffs(11) = 183
objectCoeffs(12) = 183
objectCoeffs(13) = 49
objectCoeffs(14) = 183
objectCoeffs(15) = 258
objectCoeffs(16) = 517
objectCoeffs(17) = 250
objectCoeffs(18) = 500
objectCoeffs(19) = 250
objectCoeffs(20) = 500
objectCoeffs(21) = 159
objectCoeffs(22) = 318
objectCoeffs(23) = 159
objectCoeffs(24) = 318
objectCoeffs(25) = 159
objectCoeffs(26) = 318
objectCoeffs(27) = 159
objectCoeffs(28) = 318
objectCoeffs(29) = 114
objectCoeffs(30) = 228
objectCoeffs(31) = 159
objectCoeffs(32) = 318

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
lowerBounds(32) = 0

upperBounds(0) = 1
upperBounds(1) = 1
upperBounds(2) = 1
upperBounds(3) = 1
upperBounds(4) = 1
upperBounds(5) = 1
upperBounds(6) = 1
upperBounds(7) = 1
upperBounds(8) = 1
upperBounds(9) = 1
upperBounds(10) = 1
upperBounds(11) = 1
upperBounds(12) = 1
upperBounds(13) = 1
upperBounds(14) = 1
upperBounds(15) = 1
upperBounds(16) = 1
upperBounds(17) = 1
upperBounds(18) = 1
upperBounds(19) = 1
upperBounds(20) = 1
upperBounds(21) = 1
upperBounds(22) = 1
upperBounds(23) = 1
upperBounds(24) = 1
upperBounds(25) = 1
upperBounds(26) = 1
upperBounds(27) = 1
upperBounds(28) = 1
upperBounds(29) = 1
upperBounds(30) = 1
upperBounds(31) = 1
upperBounds(32) = 1

rowType(0) = Asc("L")
rowType(1) = Asc("L")
rowType(2) = Asc("L")
rowType(3) = Asc("L")
rowType(4) = Asc("L")
rowType(5) = Asc("L")
rowType(6) = Asc("L")
rowType(7) = Asc("L")
rowType(8) = Asc("L")
rowType(9) = Asc("L")
rowType(10) = Asc("L")
rowType(11) = Asc("L")
rowType(12) = Asc("L")
rowType(13) = Asc("L")
rowType(14) = Asc("L")

rhsValues(0) = 1
rhsValues(1) = 1
rhsValues(2) = 1
rhsValues(3) = 1
rhsValues(4) = -5
rhsValues(5) = 2700
rhsValues(6) = -2600
rhsValues(7) = -100
rhsValues(8) = -900
rhsValues(9) = -1656
rhsValues(10) = -335
rhsValues(11) = -1026
rhsValues(12) = -5
rhsValues(13) = -500
rhsValues(14) = -270

matrixBegin(0) = 0
matrixBegin(1) = 3
matrixBegin(2) = 6
matrixBegin(3) = 10
matrixBegin(4) = 14
matrixBegin(5) = 19
matrixBegin(6) = 24
matrixBegin(7) = 26
matrixBegin(8) = 31
matrixBegin(9) = 36
matrixBegin(10) = 38
matrixBegin(11) = 41
matrixBegin(12) = 45
matrixBegin(13) = 49
matrixBegin(14) = 53
matrixBegin(15) = 54
matrixBegin(16) = 55
matrixBegin(17) = 56
matrixBegin(18) = 58
matrixBegin(19) = 60
matrixBegin(20) = 61
matrixBegin(21) = 62
matrixBegin(22) = 66
matrixBegin(23) = 70
matrixBegin(24) = 73
matrixBegin(25) = 76
matrixBegin(26) = 80
matrixBegin(27) = 84
matrixBegin(28) = 87
matrixBegin(29) = 90
matrixBegin(30) = 93
matrixBegin(31) = 96
matrixBegin(32) = 97
matrixBegin(33) = 98

matrixCount(0) = 3
matrixCount(1) = 3
matrixCount(2) = 4
matrixCount(3) = 4
matrixCount(4) = 5
matrixCount(5) = 5
matrixCount(6) = 2
matrixCount(7) = 5
matrixCount(8) = 5
matrixCount(9) = 2
matrixCount(10) = 3
matrixCount(11) = 4
matrixCount(12) = 4
matrixCount(13) = 4
matrixCount(14) = 1
matrixCount(15) = 1
matrixCount(16) = 1
matrixCount(17) = 2
matrixCount(18) = 2
matrixCount(19) = 1
matrixCount(20) = 1
matrixCount(21) = 4
matrixCount(22) = 4
matrixCount(23) = 3
matrixCount(24) = 3
matrixCount(25) = 4
matrixCount(26) = 4
matrixCount(27) = 3
matrixCount(28) = 3
matrixCount(29) = 3
matrixCount(30) = 3
matrixCount(31) = 1
matrixCount(32) = 1

matrixIndex(0) = 0
matrixIndex(1) = 8
matrixIndex(2) = 9
matrixIndex(3) = 0
matrixIndex(4) = 12
matrixIndex(5) = 13
matrixIndex(6) = 0
matrixIndex(7) = 5
matrixIndex(8) = 6
matrixIndex(9) = 9
matrixIndex(10) = 0
matrixIndex(11) = 5
matrixIndex(12) = 6
matrixIndex(13) = 7
matrixIndex(14) = 1
matrixIndex(15) = 5
matrixIndex(16) = 6
matrixIndex(17) = 10
matrixIndex(18) = 11
matrixIndex(19) = 1

matrixIndex(20) = 5
matrixIndex(21) = 6
matrixIndex(22) = 8
matrixIndex(23) = 9
matrixIndex(24) = 1
matrixIndex(25) = 14
matrixIndex(26) = 2
matrixIndex(27) = 5
matrixIndex(28) = 6
matrixIndex(29) = 10
matrixIndex(30) = 11
matrixIndex(31) = 2
matrixIndex(32) = 5
matrixIndex(33) = 6
matrixIndex(34) = 8
matrixIndex(35) = 9
matrixIndex(36) = 3
matrixIndex(37) = 4
matrixIndex(38) = 3
matrixIndex(39) = 10
matrixIndex(40) = 11
matrixIndex(41) = 3
matrixIndex(42) = 5
matrixIndex(43) = 6

matrixIndex(44) = 11
matrixIndex(45) = 3
matrixIndex(46) = 5
matrixIndex(47) = 6
matrixIndex(48) = 9
matrixIndex(49) = 5
matrixIndex(50) = 6
matrixIndex(51) = 8
matrixIndex(52) = 9
matrixIndex(53) = 3
matrixIndex(54) = 4
matrixIndex(55) = 4
matrixIndex(56) = 12
matrixIndex(57) = 13
matrixIndex(58) = 12
matrixIndex(59) = 13
matrixIndex(60) = 13
matrixIndex(61) = 13
matrixIndex(62) = 5
matrixIndex(63) = 6
matrixIndex(64) = 10
matrixIndex(65) = 11
matrixIndex(66) = 5

matrixIndex(67) = 6
matrixIndex(68) = 10
matrixIndex(69) = 11
matrixIndex(70) = 5
matrixIndex(71) = 6
matrixIndex(72) = 11
matrixIndex(73) = 5
matrixIndex(74) = 6
matrixIndex(75) = 11
matrixIndex(76) = 5
matrixIndex(77) = 6
matrixIndex(78) = 8
matrixIndex(79) = 9
matrixIndex(80) = 5
matrixIndex(81) = 6
matrixIndex(82) = 8
matrixIndex(83) = 9
matrixIndex(84) = 5
matrixIndex(85) = 6
matrixIndex(86) = 9
matrixIndex(87) = 5
matrixIndex(88) = 6
matrixIndex(89) = 9
matrixIndex(90) = 5
matrixIndex(91) = 6

matrixIndex(92) = 7
matrixIndex(93) = 5
matrixIndex(94) = 6
matrixIndex(95) = 7
matrixIndex(96) = 14
matrixIndex(97) = 14

matrixValues(0) = 1
matrixValues(1) = -300
matrixValues(2) = -300
matrixValues(3) = 1
matrixValues(4) = -300
matrixValues(5) = -300
matrixValues(6) = 1
matrixValues(7) = 300
matrixValues(8) = -300
matrixValues(9) = -300
matrixValues(10) = 1
matrixValues(11) = 300

matrixValues(12) = -300
matrixValues(13) = -300
matrixValues(14) = 1
matrixValues(15) = 285
matrixValues(16) = -285
matrixValues(17) = -285
matrixValues(18) = -285
matrixValues(19) = 1
matrixValues(20) = 285
matrixValues(21) = -285
matrixValues(22) = -285
matrixValues(23) = -285
matrixValues(24) = 1
matrixValues(25) = -285
matrixValues(26) = 1

matrixValues(27) = 265
matrixValues(28) = -265
matrixValues(29) = -265
matrixValues(30) = -265
matrixValues(31) = 1
matrixValues(32) = 265
matrixValues(33) = -265
matrixValues(34) = -265
matrixValues(35) = -265
matrixValues(36) = 1
matrixValues(37) = -230
matrixValues(38) = 1
matrixValues(39) = -230
matrixValues(40) = -230
matrixValues(41) = 1

matrixValues(42) = 230
matrixValues(43) = -230
matrixValues(44) = -230
matrixValues(45) = 1
matrixValues(46) = 230
matrixValues(47) = -230
matrixValues(48) = -230
matrixValues(49) = 190
matrixValues(50) = -190
matrixValues(51) = -190
matrixValues(52) = -190
matrixValues(53) = 1
matrixValues(54) = -200
matrixValues(55) = -400

matrixValues(56) = -200
matrixValues(57) = -200
matrixValues(58) = -400
matrixValues(59) = -400
matrixValues(60) = -200
matrixValues(61) = -400
matrixValues(62) = 200
matrixValues(63) = -200
matrixValues(64) = -200
matrixValues(65) = -200
matrixValues(66) = 400
matrixValues(67) = -400
matrixValues(68) = -400

matrixValues(69) = -400
matrixValues(70) = 200
matrixValues(71) = -200
matrixValues(72) = -200
matrixValues(73) = 400
matrixValues(74) = -400
matrixValues(75) = -400
matrixValues(76) = 200
matrixValues(77) = -200
matrixValues(78) = -200
matrixValues(79) = -200
matrixValues(80) = 400
matrixValues(81) = -400

matrixValues(82) = -400
matrixValues(83) = -400
matrixValues(84) = 200
matrixValues(85) = -200
matrixValues(86) = -200
matrixValues(87) = 400
matrixValues(88) = -400
matrixValues(89) = -400
matrixValues(90) = 200
matrixValues(91) = -200
matrixValues(92) = -200
matrixValues(93) = 400
matrixValues(94) = -400

matrixValues(95) = -400
matrixValues(96) = -200
matrixValues(97) = -400

colNames(0) = "c157"
colNames(1) = "c158"
colNames(2) = "c159"
colNames(3) = "c160"
colNames(4) = "c161"
colNames(5) = "c162"
colNames(6) = "c163"
colNames(7) = "c164"
colNames(8) = "c165"
colNames(9) = "c166"
colNames(10) = "c167"
colNames(11) = "c168"
colNames(12) = "c169"
colNames(13) = "c170"
colNames(14) = "c171"
colNames(15) = "c172"
colNames(16) = "c173"
colNames(17) = "c174"
colNames(18) = "c175"
colNames(19) = "c176"
colNames(20) = "c177"
colNames(21) = "c178"
colNames(22) = "c179"
colNames(23) = "c180"
colNames(24) = "c181"
colNames(25) = "c182"
colNames(26) = "c183"
colNames(27) = "c184"
colNames(28) = "c185"
colNames(29) = "c186"
colNames(30) = "c187"
colNames(31) = "c188"
colNames(32) = "c189"

'colNamesBuf = "c157" & vbNullChar & "c158" & vbNullChar & _
'           "c159" & vbNullChar & "c160" & vbNullChar & _
'           "c161" & vbNullChar & "c162" & vbNullChar & _
'           "c163" & vbNullChar & "c164" & vbNullChar & _
'           "c165" & vbNullChar & "c166" & vbNullChar & _
'           "c167" & vbNullChar & "c168" & vbNullChar & _
'           "c169" & vbNullChar & "c170" & vbNullChar & _
'           "c171" & vbNullChar & "c172" & vbNullChar & _
'           "c173" & vbNullChar & "c174" & vbNullChar & _
'           "c175" & vbNullChar & "c176" & vbNullChar & _
'           "c177" & vbNullChar & "c178" & vbNullChar & _
'           "c179" & vbNullChar & "c180" & vbNullChar & _
'           "c181" & vbNullChar & "c182" & vbNullChar & _
'           "c183" & vbNullChar & "c184" & vbNullChar & _
'           "c185" & vbNullChar & "c186" & vbNullChar & _
'           "c187" & vbNullChar & "c188" & vbNullChar & _
'           "c189" & vbNullChar

rowNames(0) = "r114"
rowNames(1) = "r115"
rowNames(2) = "r116"
rowNames(3) = "r117"
rowNames(4) = "r118"
rowNames(5) = "r119"
rowNames(6) = "r120"
rowNames(7) = "r121"
rowNames(8) = "r122"
rowNames(9) = "r123"
rowNames(10) = "r124"
rowNames(11) = "r125"
rowNames(12) = "r126"
rowNames(13) = "r127"
rowNames(14) = "r128"

'rowNamesBuf = "r114" & vbNullChar & "r115" & vbNullChar & _
'           "r116" & vbNullChar & "r117" & vbNullChar & _
'           "r118" & vbNullChar & "r119" & vbNullChar & _
'           "r120" & vbNullChar & "r121" & vbNullChar & _
'           "r122" & vbNullChar & "r123" & vbNullChar & _
'           "r124" & vbNullChar & "r125" & vbNullChar & _
'           "r126" & vbNullChar & "r127" & vbNullChar & _
'           "r128" & vbNullChar

colNamesBuf = CoinGenerateNamesBuf(colNames, NUM_COLS)
rowNamesBuf = CoinGenerateNamesBuf(rowNames, NUM_ROWS)

colType(0) = Asc("B")
colType(1) = Asc("B")
colType(2) = Asc("B")
colType(3) = Asc("B")
colType(4) = Asc("B")
colType(5) = Asc("B")
colType(6) = Asc("B")
colType(7) = Asc("B")
colType(8) = Asc("B")
colType(9) = Asc("B")
colType(10) = Asc("B")
colType(11) = Asc("B")
colType(12) = Asc("B")
colType(13) = Asc("B")
colType(14) = Asc("B")
colType(15) = Asc("B")
colType(16) = Asc("B")
colType(17) = Asc("B")
colType(18) = Asc("B")
colType(19) = Asc("B")
colType(20) = Asc("B")
colType(21) = Asc("B")
colType(22) = Asc("B")
colType(23) = Asc("B")
colType(24) = Asc("B")
colType(25) = Asc("B")
colType(26) = Asc("B")
colType(27) = Asc("B")
colType(28) = Asc("B")
colType(29) = Asc("B")
colType(30) = Asc("B")
colType(31) = Asc("B")
colType(32) = Asc("B")


optimalValue = 3089#

RunTestProblem problemName, optimalValue, colCount, rowCount, _
    nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs(0), _
    lowerBounds(0), upperBounds(0), rowType(0), rhsValues(0), 0&, _
    matrixBegin(0), matrixCount(0), matrixIndex(0), matrixValues(0), _
    colNames, rowNames, objectName, 0&, colType(0)

End Sub
