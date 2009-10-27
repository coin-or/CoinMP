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

        public const int SOLV_CHECK_COLCOUNT = 1;
        public const int SOLV_CHECK_ROWCOUNT = 2;
        public const int SOLV_CHECK_RANGECOUNT = 3;
        public const int SOLV_CHECK_OBJSENSE = 4;
        public const int SOLV_CHECK_ROWTYPE = 5;
        public const int SOLV_CHECK_MATBEGIN = 6;
        public const int SOLV_CHECK_MATCOUNT = 7;
        public const int SOLV_CHECK_MATBEGCNT = 8;
        public const int SOLV_CHECK_MATBEGNZ = 9;
        public const int SOLV_CHECK_MATINDEX = 10;
        public const int SOLV_CHECK_MATINDEXROW = 11;
        public const int SOLV_CHECK_BOUNDS = 12;
        public const int SOLV_CHECK_COLTYPE = 13;
        public const int SOLV_CHECK_COLNAMES = 14;
        public const int SOLV_CHECK_COLNAMESLEN = 15;
        public const int SOLV_CHECK_ROWNAMES = 16;
        public const int SOLV_CHECK_ROWNAMSLEN = 17;

        private static string GenerateNamesBuf(string[] namesList)
        {
            int i;
            StringBuilder namesBuf = new StringBuilder();

            if (namesList.GetLength(0) > 0)
            {
                namesBuf.Append(namesList[0] + "\0");
                for (i = 1; i < namesList.GetLength(0); i++)
                {
                    namesBuf.Append(namesList[i] + "\0");
                }
            }
            return namesBuf.ToString();
        }

        public delegate int MsgLogDelegate(string messageStr);

        public delegate int IterDelegate(int iterCount,
                                         double objectValue,
                                         int isFeasible,
                                         double infeasValue);

        public delegate int MipNodeDelegate(int iterCount,
                                            int mipNodeCount,
                                            double bestBound,
                                            double bestInteger,
                                            int isMipImproved);

        [DllImport("coinmp.dll")] public static extern int CoinInitSolver(string licenseStr);
        [DllImport("coinmp.dll")] public static extern int CoinFreeSolver();

        [DllImport("coinmp.dll")] public static extern string CoinGetSolverName();
        [DllImport("coinmp.dll")] public static extern int CoinGetSolverNameBuf(StringBuilder solverName, int buflen);
        [DllImport("coinmp.dll")] public static extern string CoinGetVersionStr();
        [DllImport("coinmp.dll")] public static extern int CoinGetVersionStrBuf(StringBuilder versionStr, int buflen);
        [DllImport("coinmp.dll")] public static extern double CoinGetVersion();

        [DllImport("coinmp.dll")] public static extern int CoinGetFeatures();
        [DllImport("coinmp.dll")] public static extern int CoinGetMethods();
        [DllImport("coinmp.dll")] public static extern double CoinGetInfinity();

        [DllImport("coinmp.dll")] public static extern IntPtr CoinCreateProblem(string problemName);

        [DllImport("coinmp.dll")] public static extern int CoinLoadProblemBuf(IntPtr hProb, int colCount, int rowCount,
                        int nzCount, int rangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, char[] rowType, double[] rhsValues,
                        double[] rangeValues, int[] matrixBegin, int[] matrixCount, int[] matrixIndex,
                        double[] matrixValues, string colNamesBuf, string rowNamesBuf, string objName);

        public static int CoinLoadProblem(IntPtr hProb, int colCount, int rowCount, int nzCount,
                        int rangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, char[] rowType, double[] rhsValues,
                        double[] rangeValues, int[] matrixBegin, int[] matrixCount, int[] matrixIndex,
                        double[] matrixValues, string[] colNames, string[] rowNames, string objName)
        {
            return CoinLoadProblemBuf(hProb, colCount, rowCount, nzCount, rangeCount, objectSense, 
                        objectConst, objectCoeffs, lowerBounds, upperBounds, rowType, rhsValues, 
                        rangeValues, matrixBegin, matrixCount, matrixIndex, matrixValues,
                        GenerateNamesBuf(colNames), GenerateNamesBuf(rowNames), objName);

        }

        // when there is no rowType argument, CoinLoadProblem switches to rowLower and rowUpper arguments
        public static int CoinLoadProblem(IntPtr hProb, int colCount, int rowCount, int nzCount,
                        int rangeCount, int objectSense, double objectConst, double[] objectCoeffs,
                        double[] lowerBounds, double[] upperBounds, double[] rowLower, double[] rowUpper,
                        int[] matrixBegin, int[] matrixCount, int[] matrixIndex, double[] matrixValues, 
                        string[] colNames, string[] rowNames, string objName)
        {
            return CoinLoadProblemBuf(hProb, colCount, rowCount, nzCount, rangeCount, objectSense,
                        objectConst, objectCoeffs, lowerBounds, upperBounds, null, rowLower,
                        rowUpper, matrixBegin, matrixCount, matrixIndex, matrixValues,
                        GenerateNamesBuf(colNames), GenerateNamesBuf(rowNames), objName);

        }

        [DllImport("coinmp.dll")] public static extern int CoinLoadInitValues(IntPtr hProb, double[] initValues);

        [DllImport("coinmp.dll")] public static extern int CoinLoadInteger(IntPtr hProb, char[] columnType);


        [DllImport("coinmp.dll")] public static extern int CoinLoadPriority(IntPtr hProb, int priorCount,
                        int[] priorIndex, int[] priorValues, int[] branchDir);

        [DllImport("coinmp.dll")] public static extern int CoinLoadSemiCont(IntPtr hProb, int semiCount,
                        int[] semiIndex);

        [DllImport("coinmp.dll")] public static extern int CoinLoadSos(IntPtr hProb, int sosCount,
                        int sosNZCount, int[] sosType, int[] sosPrior,
                        int[] sosBegin, int[] sosIndex, double[] sosRef);

        [DllImport("coinmp.dll")] public static extern int CoinLoadQuadratic(IntPtr hProb, int[] quadBegin,
                        int[] quadCount, int[] quadIndex, double[] quadValues);

        [DllImport("coinmp.dll")] public static extern int CoinLoadNonlinear(IntPtr hProb, int nlpTreeCount,
                        int nlpLineCount, int[] nlpBegin, int[] nlpOper,
                        int[] nlpArg1, int[] nlpArg2, int[] nlpIndex1,
                        int[] nlpIndex2, double[] nlpValue1, double[] nlpValue2);

        [DllImport("coinmp.dll")] public static extern int CoinUnloadProblem(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinCheckProblem(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinSetLoadNamesType(IntPtr hProb, int loadNamesType);

        [DllImport("coinmp.dll")] public static extern string CoinGetProblemName(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetProblemNameBuf(IntPtr hProb,
                        StringBuilder problemName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetColCount(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetRowCount(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern string CoinGetColName(IntPtr hProb, int col);
        [DllImport("coinmp.dll")] public static extern int CoinGetColNameBuf(IntPtr hProb, int col,
                        StringBuilder colName, int buflen);

        [DllImport("coinmp.dll")] public static extern string CoinGetRowName(IntPtr hProb, int row);
        [DllImport("coinmp.dll")] public static extern int CoinGetRowNameBuf(IntPtr hProb, int row,
                        StringBuilder rowName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinSetMsgLogCallback(IntPtr hProb, MsgLogDelegate msgLogDelegate);
        [DllImport("coinmp.dll")] public static extern int CoinSetIterCallback(IntPtr hProb, IterDelegate iterDelegate);
        [DllImport("coinmp.dll")] public static extern int CoinSetMipNodeCallback(IntPtr hProb, MipNodeDelegate mipNodeDelegate);

        [DllImport("coinmp.dll")] public static extern int CoinOptimizeProblem(IntPtr hProb, int method);

        public static int CoinOptimizeProblem(IntPtr hProb)
        {
            return CoinOptimizeProblem(hProb, SOLV_METHOD_DEFAULT);
        }

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionStatus(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern string CoinGetSolutionText(IntPtr hProbs, int solutionStatus);
        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionTextBuf(IntPtr hProbs, int solutionStatus, 
                        StringBuilder solutionText, int buflen);

        [DllImport("coinmp.dll")] public static extern double CoinGetObjectValue(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern double CoinGetMipBestBound(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinGetIterCount(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetMipNodeCount(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionValues(IntPtr hProb, [In, Out] double[] activity,
                        [In, Out] double[] reducedCost, [In, Out] double[] slackValues, [In, Out] double[] shadowPrice);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionRanges(IntPtr hProb, [In, Out] double[] objLoRange,
                        [In, Out] double[] objUpRange, [In, Out] double[] rhsLoRange, [In, Out] double[] rhsUpRange);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionBasis(IntPtr hProb, [In, Out] int[] colStatus,
                        [In, Out] double[] rowStatus);

        [DllImport("coinmp.dll")] public static extern int CoinReadFile(IntPtr hProb, int fileType, string readFilename);
        [DllImport("coinmp.dll")] public static extern int CoinWriteFile(IntPtr hProb, int fileType, string writeFilename);


        [DllImport("coinmp.dll")] public static extern int CoinOpenLogFile(IntPtr hProb, string logFilename);
        [DllImport("coinmp.dll")] public static extern int CoinCloseLogFile(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionCount(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionInfo(IntPtr hProb, int optionNr, 
                        [In, Out] int[] optionID, [In, Out] int[] groupType, [In, Out] int[] optionType);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionName(IntPtr hProb, int optionNr);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionNameBuf(IntPtr hProb, int optionNr,
                        StringBuilder optionName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionShortName(IntPtr hProb, int optionNr);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionShortNameBuf(IntPtr hProb, int optionNr,
                        StringBuilder shortName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetIntOptionMinMax(IntPtr hProb, int optionNr,
                        [In, Out] int[] minValue, [In, Out] int[] maxValue);
        [DllImport("coinmp.dll")] public static extern int CoinGetRealOptionMinMax(IntPtr hProb, int optionNr,
                        [In, Out] double[] minValue, [In, Out] double[] maxValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionChanged(IntPtr hProb, int optionID);

        [DllImport("coinmp.dll")] public static extern int CoinGetIntOption(IntPtr hProb, int optionID);
        [DllImport("coinmp.dll")] public static extern int CoinSetIntOption(IntPtr hProb, int optionID, int intValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetRealOption(IntPtr hProb, int optionID);
        [DllImport("coinmp.dll")] public static extern int CoinSetRealOption(IntPtr hProb, int optionID, double realValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetStringOption(IntPtr hProb, int optionID);
        [DllImport("coinmp.dll")] public static extern int CoinGetStringOptionBuf(IntPtr hProb, int optionID,
                        StringBuilder stringValue, int buflen);
        [DllImport("coinmp.dll")] public static extern int CoinSetStringOption(IntPtr hProb, int optionID,
                        string stringValue);
    }
}
