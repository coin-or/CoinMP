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

        public void RunProblem(string problemName, double optimalValue,
            int colCount, int rowCount, int nonZeroCount, int rangeCount,
            int objectSense, double objectConst, double[] objectCoeffs,
            double[] lowerBounds, double[] upperBounds, char[] rowType,
            double[] rhsValues, double[] rangeValues, int[] matrixBegin,
            int[] matrixCount, int[] matrixIndex, double[] matrixValues,
            string[] colNames, string[] rowNames, string objectName,
            double[] initValues, char[] colType)
        {
            IntPtr hProb;
            int solutionStatus;
            double objectValue;
            int result;
            int length;
            int i;
            StringBuilder solutionText = new StringBuilder(100);
            StringBuilder colName = new StringBuilder(100);

            logTxt.NewLine();
            logTxt.WriteLine("Solve Problem: " + problemName);
            logTxt.WriteLine("---------------------------------------------------------------");

            hProb = CoinMP.CoinCreateProblem(problemName);
            result = CoinMP.CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount,
                            objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds,
                            rowType, rhsValues, rangeValues, matrixBegin, matrixCount,
                            matrixIndex, matrixValues, colNames, rowNames, objectName);
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
            logMsg.WriteLine("---------------------------------------------------------------");

            result = CoinMP.CoinWriteFile(hProb, 1, problemName + ".mps");  // SOLV_FILE_MPS

            solutionStatus = CoinMP.CoinGetSolutionStatus(hProb);
            length = CoinMP.CoinGetSolutionText(hProb, solutionStatus, solutionText, solutionText.Capacity);
            objectValue = CoinMP.CoinGetObjectValue(hProb);

            logTxt.WriteLine("Problem Name:    " + problemName);
            logTxt.WriteLine("Solution Result: " + solutionText);
            logTxt.WriteLine("Solution Status: " + solutionStatus);
            logTxt.WriteLine("Optimal Value:   " + objectValue + " (" + optimalValue + ")");
            logTxt.WriteLine("---------------------------------------------------------------");

            colCount = CoinMP.CoinGetColCount(hProb);
            rowCount = CoinMP.CoinGetRowCount(hProb);

            double[] activity = new double[colCount];
            double[] reduced = new double[colCount];
            double[] slack = new double[rowCount];
            double[] shadow = new double[rowCount];

            result = CoinMP.CoinGetSolutionValues(hProb, activity, reduced, slack, shadow);
            for (i = 0; i < colCount; i++)
            {
                if (activity[i] != 0.0)
                {
                    length = CoinMP.CoinGetColName(hProb, i, colName, colName.Capacity);
                    logTxt.WriteLine(colName + " = " + activity[i]);
                }
            }
            logTxt.WriteLine("---------------------------------------------------------------");
            logTxt.NewLine();

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
    }
}
