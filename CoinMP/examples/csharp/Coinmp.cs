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

        public delegate int IterDelegate(int    IterCount, 
							             double ObjectValue,
							             int    IsFeasible, 
							             double InfeasValue);

        public delegate int MipNodeDelegate(int   IterCount, 
							                int	  MipNodeCount,
							                double BestBound,
							                double BestInteger,
							                int    IsMipImproved);

        [DllImport("coinmp.dll")] public static extern int CoinInitSolver(string LicenseStr);
        [DllImport("coinmp.dll")] public static extern int CoinFreeSolver();

        [DllImport("coinmp.dll")] public static extern string CoinGetSolverName();
        [DllImport("coinmp.dll")] public static extern int CoinGetSolverNameBuf(StringBuilder SolverName, int buflen);

        [DllImport("coinmp.dll")] public static extern string CoinGetVersionStr();
        [DllImport("coinmp.dll")] public static extern int CoinGetVersionStrBuf(StringBuilder VersionStr, int buflen);
        [DllImport("coinmp.dll")] public static extern double CoinGetVersion();

        [DllImport("coinmp.dll")] public static extern int CoinGetFeatures();
        [DllImport("coinmp.dll")] public static extern int CoinGetMethods();
        [DllImport("coinmp.dll")] public static extern double CoinGetInfinity();

        [DllImport("coinmp.dll")] public static extern IntPtr CoinCreateProblem(string problemName);

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

        // when there is no rowType argument, CoinLoadProblem switches to rowLower and rowUpper arguments
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

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionValues(IntPtr hProb, double[] activity, 
                                                    double[] reducedCost, double[] slackValues, double[] shadowPrice);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionRanges(IntPtr hProb, double[] ObjLoRange, 
									                double[] ObjUpRange, double[] RhsLoRange, double[] RhsUpRange);

        [DllImport("coinmp.dll")] public static extern int CoinGetSolutionBasis(IntPtr hProb, int[] ColStatus, 
                                                    double[] RowStatus);

        [DllImport("coinmp.dll")] public static extern int CoinReadFile(IntPtr hProb, int FileType, string ReadFilename);
        [DllImport("coinmp.dll")] public static extern int CoinWriteFile(IntPtr hProb, int FileType, string WriteFilename);


        [DllImport("coinmp.dll")] public static extern int CoinOpenLogFile(IntPtr hProb, string LogFilename);
        [DllImport("coinmp.dll")] public static extern int CoinCloseLogFile(IntPtr hProb);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionCount(IntPtr hProb);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionInfo(IntPtr hProb, int OptionNr, int[] OptionID, 
                                                    int[] GroupType, int[] OptionType);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionName(IntPtr hProb, int OptionNr);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionNameBuf(IntPtr hProb, int OptionNr, 
                                                    StringBuilder OptionName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionShortName(IntPtr hProb, int OptionNr);
        [DllImport("coinmp.dll")] public static extern int CoinGetOptionShortNameBuf(IntPtr hProb, int OptionNr, 
                                                    StringBuilder ShortName, int buflen);

        [DllImport("coinmp.dll")] public static extern int CoinGetIntOptionMinMax(IntPtr hProb, int OptionNr, 
                                                    int[] MinValue, int[] MaxValue);
        [DllImport("coinmp.dll")] public static extern int CoinGetRealOptionMinMax(IntPtr hProb, int OptionNr, 
                                                    double[] MinValue, double[] MaxValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetOptionChanged(IntPtr hProb, int OptionID);

        [DllImport("coinmp.dll")] public static extern int CoinGetIntOption(IntPtr hProb, int OptionID);
        [DllImport("coinmp.dll")] public static extern int CoinSetIntOption(IntPtr hProb, int OptionID, int IntValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetRealOption(IntPtr hProb, int OptionID);
        [DllImport("coinmp.dll")] public static extern int CoinSetRealOption(IntPtr hProb, int OptionID, double RealValue);

        [DllImport("coinmp.dll")] public static extern int CoinGetStringOption(IntPtr hProb, int OptionID);
        [DllImport("coinmp.dll")] public static extern int CoinGetStringOptionBuf(IntPtr hProb, int OptionID, 
                                                    StringBuilder StringValue, int buflen);
        [DllImport("coinmp.dll")] public static extern int CoinSetStringOption(IntPtr hProb, int OptionID, 
                                                    string StringValue);
    }
}
