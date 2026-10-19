using System;
using System.Text;
using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public class SolveProblem
    {
        LogHandler logTxt = null;
        LogHandler logMsg = null;

        public SolveProblem()
        {
            logTxt = new LogHandler(null);
            logMsg = new LogHandler(null);
        }

        public SolveProblem(TextBox txtLog)
        {
            logTxt = new LogHandler(txtLog);
            logMsg = new LogHandler(null);
        }

        public SolveProblem(TextBox txtLog, TextBox msgLog)
        {
            logTxt = new LogHandler(txtLog);
            logMsg = new LogHandler(msgLog);
        }

        public TextBox txtLog
        {
            get { return logTxt.txtLog; }
            set { logTxt.txtLog = value; }
        }

        public TextBox msgLog
        {
            get { return logMsg.txtLog; }
            set { logMsg.txtLog = value; }
        }

        private int MsgLogCallback(string msg)
        {
            logMsg.WriteLine("*** " + msg);
            return 0;
        }

        public void GetAndCheckSolution(double optimalValue, IntPtr hProb)
        {
            logMsg.WriteLine("---------------------------------------------------------------");

            String problemName = CoinMP.CoinGetProblemName(hProb);
            int solutionStatus = CoinMP.CoinGetSolutionStatus(hProb);
            String solutionText = CoinMP.CoinGetSolutionText(hProb);
            double objectValue = CoinMP.CoinGetObjectValue(hProb);

            logTxt.WriteLine("Problem Name:    " + problemName);
            logTxt.WriteLine("Solution Result: " + solutionText);
            logTxt.WriteLine("Solution Status: " + solutionStatus);
            logTxt.WriteLine("Optimal Value:   " + objectValue + " (" + optimalValue + ")");
            logTxt.WriteLine("---------------------------------------------------------------");

            int colCount = CoinMP.CoinGetColCount(hProb);

            double[] xValues = new double[colCount];

            int result = CoinMP.CoinGetSolutionValues(hProb, xValues, null, null, null);
            for (int i = 0; i < colCount; i++) {
                if (xValues[i] != 0.0) {
                    String colName = CoinMP.CoinGetColName(hProb, i);
                    logTxt.WriteLine(colName + " = " + xValues[i]);
                }
            }
            logTxt.WriteLine("---------------------------------------------------------------");
            logTxt.NewLine();
            if (solutionStatus != 0) {
                MessageBox.Show("status="+solutionStatus, problemName);
            }
            if (optimalValue != 0.0) {
                if (Math.Abs(objectValue - optimalValue) >= 0.001) {
                    MessageBox.Show("Obj=" + objectValue + " <> " + optimalValue, problemName);
                }
            }
        }

        public void RunProblem(string problemName, double optimalValue,
            int colCount, int rowCount, int nonZeroCount, int rangeCount,
            int objectSense, double objectConst, double[] objectCoeffs,
            double[] lowerBounds, double[] upperBounds, char[] rowType,
            double[] rhsValues, double[] rangeValues, int[] matrixBegin,
            int[] matrixCount, int[] matrixIndex, double[] matrixValues,
            string[] colNames, string[] rowNames, string objectName,
            double[] initValues, char[] colType)
        {
            logTxt.NewLine();
            logTxt.WriteLine("Solve Problem: " + problemName + " (obj=" + optimalValue + ")");
            logTxt.WriteLine("---------------------------------------------------------------");

            IntPtr hProb = CoinMP.CoinCreateProblem(problemName);
            int result = CoinMP.CoinLoadMatrix(hProb, colCount, rowCount, nonZeroCount, rangeCount,
                            objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds,
                            rowType, rhsValues, rangeValues, matrixBegin, matrixCount,
                            matrixIndex, matrixValues);
            result = CoinMP.CoinLoadNames(hProb, colNames, rowNames, objectName);
            if (result != 0) {
                logTxt.WriteLine("CoinLoadProblem failed");
            }
            if (colType != null) {
                result = CoinMP.CoinLoadInteger(hProb, colType);
                if (result != 0) {
                    logTxt.WriteLine("CoinLoadInteger failed");
                }
            }
            result = CoinMP.CoinCheckProblem(hProb);
            if (result != 0) {
                logTxt.WriteLine("Check Problem failed (result = " + result + ")");
            }

            CoinMP.MsgLogDelegate MsgLogDelegate = new CoinMP.MsgLogDelegate(MsgLogCallback);

            result = CoinMP.CoinSetMsgLogCallback(hProb, MsgLogDelegate);
            result = CoinMP.CoinOptimizeProblem(hProb);
            result = CoinMP.CoinWriteFile(hProb, CoinMP.SOLV_FILE_MPS, problemName);

            GetAndCheckSolution(optimalValue, hProb);

            result = CoinMP.CoinUnloadProblem(hProb);
        }


        public void RunProblem(string problemName, double optimalValue,
            int colCount, int rowCount, int nonZeroCount, int rangeCount,
            int objectSense, double objectConst, double[] objectCoeffs,
            double[] lowerBounds, double[] upperBounds,
            double[] rowLower, double[] rowUpper, int[] matrixBegin,
            int[] matrixCount, int[] matrixIndex, double[] matrixValues,
            string[] colNames, string[] rowNames, string objectName,
            double[] initValues, char[] colType)
        {
            RunProblem(problemName, optimalValue, colCount, rowCount,
                nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs,
                lowerBounds, upperBounds, null, rowLower, rowUpper, matrixBegin,
                matrixCount, matrixIndex, matrixValues, colNames, rowNames,
                objectName, initValues, colType);
        }


        public void RunProblemMip(string problemName, double optimalValue,
            int colCount, int rowCount, int nonZeroCount, int rangeCount,
            int objectSense, double objectConst, double[] objectCoeffs,
            double[] lowerBounds, double[] upperBounds, char[] rowType,
            double[] rhsValues, double[] rangeValues, int[] matrixBegin,
            int[] matrixCount, int[] matrixIndex, double[] matrixValues,
            string[] colNames, string[] rowNames, string objectName,
            double[] initValues, char[] colType, int sosCount, int sosNZCount,
            int[] sosType, int[] sosPrior, int[] sosBegin, int[] sosIndex,
            double[] sosRef, int semiCount, int[] semiIndex, int priorCount,
            int[] priorIndex, int[] priorValues, int[] priorBranch)
        {
            logTxt.NewLine();
            logTxt.WriteLine("Solve Problem: " + problemName + " (obj=" + optimalValue + ")");
            logTxt.WriteLine("---------------------------------------------------------------");

            IntPtr hProb = CoinMP.CoinCreateProblem(problemName);
            int result = CoinMP.CoinLoadMatrix(hProb, colCount, rowCount, nonZeroCount, rangeCount,
                            objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds,
                            rowType, rhsValues, rangeValues, matrixBegin, matrixCount,
                            matrixIndex, matrixValues);
            result = CoinMP.CoinLoadNames(hProb, colNames, rowNames, objectName);
            if (result != 0) {
                logTxt.WriteLine("CoinLoadProblem failed");
            }
            if (colType != null) {
                result = CoinMP.CoinLoadInteger(hProb, colType);
                if (result != 0) {
                    logTxt.WriteLine("CoinLoadInteger failed");
                }
            }
            if (priorCount > 0) {
                result = CoinMP.CoinLoadPriority(hProb, priorCount, priorIndex, priorValues, priorBranch);
            }
            if (sosCount > 0) {
		        result = CoinMP.CoinLoadSos(hProb, sosCount, sosNZCount, sosType, sosPrior, sosBegin,
                    sosIndex, sosRef);
            }
            if (semiCount > 0) {
		        result = CoinMP.CoinLoadSemiCont(hProb, semiCount, semiIndex);
            }
            result = CoinMP.CoinCheckProblem(hProb);
            if (result != 0) {
                logTxt.WriteLine("Check Problem failed (result = " + result + ")");
            }

            CoinMP.MsgLogDelegate MsgLogDelegate = new CoinMP.MsgLogDelegate(MsgLogCallback);

            result = CoinMP.CoinSetMsgLogCallback(hProb, MsgLogDelegate);
            result = CoinMP.CoinOptimizeProblem(hProb);
            result = CoinMP.CoinWriteFile(hProb, CoinMP.SOLV_FILE_MPS, problemName + ".mps");

            GetAndCheckSolution(optimalValue, hProb);

            result = CoinMP.CoinUnloadProblem(hProb);
        }

    }
}

