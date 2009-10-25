using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace Coin.CoinMP
{
    public class CoinMP
    {
        public const int SOLV_CALL_SUCCESS = 0;
        public const int SOLV_CALL_FAILED = -1;

        public const int SOLV_METHOD_DEFAULT = 0;

        public const int SOLV_METHOD_PRIMAL = 0x1;
        public const int SOLV_METHOD_DUAL = 0x2;
        public const int SOLV_METHOD_NETWORK = 0x4;
        public const int SOLV_METHOD_BARRIER = 0x8;

        public const int SOLV_METHOD_BENDERS = 0x100;
        public const int SOLV_METHOD_DEQ = 0x200;
        public const int SOLV_METHOD_EV = 0x400;


        public const int SOLV_FEATURE_LP = 0x1;
        public const int SOLV_FEATURE_QP = 0x2;
        public const int SOLV_FEATURE_QCP = 0x4;
        public const int SOLV_FEATURE_NLP = 0x8;

        public const int SOLV_FEATURE_MIP = 0x10;
        public const int SOLV_FEATURE_MIQP = 0x20;
        public const int SOLV_FEATURE_MIQCP = 0x40;
        public const int SOLV_FEATURE_MINLP = 0x80;

        public const int SOLV_FEATURE_SP = 0x10000;

        public const int SOLV_LOADNAMES_DEFAULT = 0;
        public const int SOLV_LOADNAMES_LIST = 1;
        public const int SOLV_LOADNAMES_BUF = 2;

        public const int SOLV_OBJSENS_MAX = -1;
        public const int SOLV_OBJSENS_MIN = 1;


        public const int SOLV_FILE_LOG = 0;
        public const int SOLV_FILE_BASIS = 1;
        public const int SOLV_FILE_MIPSTART = 2;
        public const int SOLV_FILE_MPS = 3;
        public const int SOLV_FILE_LP = 4;
        public const int SOLV_FILE_BINARY = 5;
        public const int SOLV_FILE_OUTPUT = 6;
        public const int SOLV_FILE_BINOUT = 7;
        public const int SOLV_FILE_IIS = 8;

        private static string GenerateNamesBuf(string[] NamesList)
        {
            int i;
            StringBuilder namesBuf = new StringBuilder();

            if (NamesList.GetLength(0) > 0)
            {
                namesBuf.Append(NamesList[0] + "\0");
                for (i = 1; i < NamesList.GetLength(0); i++)
                {
                    namesBuf.Append(NamesList[i] + "\0");
                }
            }
            return namesBuf.ToString();
        }

        public delegate int MsgLogDelegate(string MessageStr);

        [DllImport("coinmp.dll")] public static extern int CoinInitSolver(string LicenseStr);
        [DllImport("coinmp.dll")] public static extern int CoinFreeSolver();

        [DllImport("coinmp.dll")] public static extern String CoinGetSolverName();
        [DllImport("coinmp.dll")] public static extern int CoinGetSolverNameBuf(StringBuilder SolverName, int buflen);
        [DllImport("coinmp.dll")] public static extern String CoinGetVersionStr();
        [DllImport("coinmp.dll")] public static extern int CoinGetVersionStrBuf(StringBuilder VersionStr, int buflen);
        [DllImport("coinmp.dll")] public static extern double CoinGetVersion();

        [DllImport("coinmp.dll")] public static extern int CoinGetFeatures();
        [DllImport("coinmp.dll")] public static extern int CoinGetMethods();
        [DllImport("coinmp.dll")] public static extern double CoinGetInfinity();

        [DllImport("coinmp.dll")] public static extern IntPtr CoinCreateProblem(string problemName);

        [DllImport("coinmp.dll")] public static extern int CoinSetLoadNamesType(IntPtr hProb, int loadNamesType);

        [DllImport("coinmp.dll")] public static extern int CoinLoadProblemBuf(IntPtr hProb, int colCount, int rowCount,
                        int NZcount, int RangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, char[] rowType, double[] RHSValues,
                        double[] RangeValues, int[] matrixBegin, int[] matrixCount, int[] matrixIndex,
                        double[] matrixValues, string colNamesBuf, string rowNamesBuf, string objName);

        public static int CoinLoadProblem(IntPtr hProb, int colCount, int rowCount, int NZcount,
                        int RangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, char[] rowType, double[] RHSValues,
                        double[] RangeValues, int[] matrixBegin, int[] matrixCount, int[] matrixIndex,
                        double[] matrixValues, string[] colNames, string[] rowNames, string objName)
        {
            return CoinLoadProblemBuf(hProb, colCount, rowCount, NZcount, RangeCount, objectSense, 
                        objectConst, objectCoeffs, lowerBounds, upperBounds, rowType, RHSValues, 
                        RangeValues, matrixBegin, matrixCount, matrixIndex, matrixValues,
                        GenerateNamesBuf(colNames), GenerateNamesBuf(rowNames), objName);

        }

        // No rowType, switch to rowLower and rowUpper arguments
        public static int CoinLoadProblem(IntPtr hProb, int colCount, int rowCount, int NZcount,
                        int RangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, double[] rowLower, double[] rowUpper,
                        int[] matrixBegin, int[] matrixCount, int[] matrixIndex, double[] matrixValues, 
                        string[] colNames, string[] rowNames, string objName)
        {
            return CoinLoadProblemBuf(hProb, colCount, rowCount, NZcount, RangeCount, objectSense,
                        objectConst, objectCoeffs, lowerBounds, upperBounds, null, rowLower,
                        rowUpper, matrixBegin, matrixCount, matrixIndex, matrixValues,
                        GenerateNamesBuf(colNames), GenerateNamesBuf(rowNames), objName);

        }

        [DllImport("coinmp.dll")] public static extern int CoinUnloadProblem(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinCheckProblem(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinOptimizeProblem(IntPtr hProb, int method);

        public static int CoinOptimizeProblem(IntPtr hProb)
        {
            return CoinOptimizeProblem(hProb, SOLV_METHOD_DEFAULT);
        }

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionStatus(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern double CoinGetObjectValue(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinSetMsgLogCallback(IntPtr hProb, MsgLogDelegate MsgLogDelegate);


        [DllImport("coinmp.dll")] public static extern int CoinLoadInteger(IntPtr hProb, char[] columnType);

        [DllImport("coinmp.dll")] public static extern int CoinWriteFile(IntPtr hProb, int fileType, string writeFilename);

        [DllImport("coinmp.dll")] public static extern String CoinGetSolutionText(IntPtr hProbs, int solutionStatus);
        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionTextBuf(IntPtr hProbs, int solutionStatus, 
                        StringBuilder solutionText, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetColCount(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetRowCount(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionValues(IntPtr hProb, [In, Out] double[] activity,
                [In, Out] double[] reducedCost, [In, Out] double[] slackValues, [In, Out] double[] shadowPrice);

        [DllImport("coinmp.dll")] public static extern String CoinGetColName(IntPtr hProb, int col);
        [DllImport("coinmp.dll")] public static extern int CoinGetColNameBuf(IntPtr hProb, int col, StringBuilder colName, int buflen);
   
    }
}
