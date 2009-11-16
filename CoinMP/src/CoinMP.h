/************************************************************************/
/*                                                                      */
/*  CoinMP Solver                                     Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.h'                                          */
/*                                                                      */
/*  Version      :  1.5                                                 */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson, Maximal Software               */
/*                                                                      */
/*  Copyright (c) 2002-2009                     Bjarni Kristjansson     */
/*                                                                      */
/************************************************************************/

// $Id$

#ifndef _COINMP_H_
#define _COINMP_H_


#if defined(_MSC_VER) && !defined(HAVE_CONFIG_H)

#define SOLVCALL   __stdcall
#ifdef SOLVER_EXPORT
#define SOLVAPI  __declspec(dllexport)
#else
#define SOLVAPI __declspec(dllimport)
#endif

#else

#define SOLVAPI 
#define SOLVCALL

#endif


#ifndef SOLV_LINK_LIB
#ifndef SOLV_LINK_DLL
#define SOLV_LINK_LIB
#endif
#endif



#define SOLV_CALL_SUCCESS   0
#define SOLV_CALL_FAILED    -1

#define SOLV_METHOD_DEFAULT       0x00000000L

#define SOLV_METHOD_PRIMAL        0x00000001L
#define SOLV_METHOD_DUAL          0x00000002L
#define SOLV_METHOD_NETWORK       0x00000004L
#define SOLV_METHOD_BARRIER       0x00000008L

#define SOLV_METHOD_BENDERS       0x00000100L
#define SOLV_METHOD_DEQ           0x00000200L
#define SOLV_METHOD_EV            0x00000400L


#define SOLV_FEATURE_LP            0x00000001L
#define SOLV_FEATURE_QP            0x00000002L
#define SOLV_FEATURE_QCP           0x00000004L
#define SOLV_FEATURE_NLP           0x00000008L

#define SOLV_FEATURE_MIP           0x00000010L
#define SOLV_FEATURE_MIQP          0x00000020L
#define SOLV_FEATURE_MIQCP         0x00000040L
#define SOLV_FEATURE_MINLP         0x00000080L

#define SOLV_FEATURE_SP            0x00010000L


#define SOLV_OBJSENS_MAX   -1
#define SOLV_OBJSENS_MIN    1


#define SOLV_FILE_LOG      0
#define SOLV_FILE_BASIS    1
#define SOLV_FILE_MIPSTART 2
#define SOLV_FILE_MPS      3
#define SOLV_FILE_LP       4
#define SOLV_FILE_BINARY   5
#define SOLV_FILE_OUTPUT   6
#define SOLV_FILE_BINOUT   7
#define SOLV_FILE_IIS      8

#define SOLV_CHECK_COLCOUNT     1
#define SOLV_CHECK_ROWCOUNT     2
#define SOLV_CHECK_RANGECOUNT   3
#define SOLV_CHECK_OBJSENSE     4
#define SOLV_CHECK_ROWTYPE      5
#define SOLV_CHECK_MATBEGIN     6 
#define SOLV_CHECK_MATCOUNT     7 
#define SOLV_CHECK_MATBEGCNT    8
#define SOLV_CHECK_MATBEGNZ     9
#define SOLV_CHECK_MATINDEX    10
#define SOLV_CHECK_MATINDEXROW 11
#define SOLV_CHECK_BOUNDS      12
#define SOLV_CHECK_COLTYPE     13 
#define SOLV_CHECK_COLNAMES    14
#define SOLV_CHECK_COLNAMESLEN 15
#define SOLV_CHECK_ROWNAMES    16
#define SOLV_CHECK_ROWNAMSLEN  17


typedef void *HPROB;

#ifdef __cplusplus
extern "C" {
#endif

typedef int (SOLVCALL * MSGLOGCALLBACK)(const char* MessageStr);

typedef int (SOLVCALL *ITERCALLBACK)(int    IterCount, 
							double ObjectValue,
							int    IsFeasible, 
							double InfeasValue);

typedef int (SOLVCALL *MIPNODECALLBACK)(int    IterCount, 
							   int	  MipNodeCount,
							   double BestBound,
							   double BestInteger,
							   int    IsMipImproved);



#ifdef SOLV_LINK_LIB

SOLVAPI int    SOLVCALL CoinInitSolver(const char* LicenseStr);
SOLVAPI int    SOLVCALL CoinFreeSolver(void);

SOLVAPI const char*  SOLVCALL CoinGetSolverName(void);
SOLVAPI int    SOLVCALL CoinGetSolverNameBuf(char* SolverName, int buflen);

SOLVAPI const char*  SOLVCALL CoinGetVersionStr(void);
SOLVAPI int    SOLVCALL CoinGetVersionStrBuf(char* VersionStr, int buflen);
SOLVAPI double SOLVCALL CoinGetVersion(void);

SOLVAPI int    SOLVCALL CoinGetFeatures(void);
SOLVAPI int    SOLVCALL CoinGetMethods(void);
SOLVAPI double SOLVCALL CoinGetInfinity(void);

SOLVAPI HPROB  SOLVCALL CoinCreateProblem(const char* ProblemName);

SOLVAPI int SOLVCALL CoinLoadMatrix(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues);

SOLVAPI int SOLVCALL CoinLoadNames(HPROB hProb, char** ColNamesList, 
				char** RowNamesList, char* ObjectName);

SOLVAPI int SOLVCALL CoinLoadNamesBuf(HPROB hProb, char* ColNamesBuf, 
				char* RowNamesBuf, char* ObjectName);

SOLVAPI int    SOLVCALL CoinLoadProblem(HPROB hProb, 
					int ColCount, int RowCount, int NZCount, int RangeCount, 
					int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
					double* LowerBounds, double* UpperBounds, char* RowType, 
					double* RHSValues, double* RangeValues, int* MatrixBegin,  
					int* MatrixCount, int* MatrixIndex, double* MatrixValues,   
					char** ColNamesList, char** RowNamesList, char* objName);

SOLVAPI int    SOLVCALL CoinLoadProblemBuf(HPROB hProb, 
					int ColCount, int RowCount, int NZCount, int RangeCount, 
					int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
					double* LowerBounds, double* UpperBounds, char* RowType, 
					double* RHSValues, double* RangeValues, int* MatrixBegin,  
					int* MatrixCount, int* MatrixIndex, double* MatrixValues,   
					char* ColNamesBuf, char* RowNamesBuf, char* objName);

SOLVAPI int    SOLVCALL CoinLoadInitValues(HPROB hProb, double* InitValues);

SOLVAPI int    SOLVCALL CoinLoadInteger(HPROB hProb, char* ColumnType);


SOLVAPI int    SOLVCALL CoinLoadPriority(HPROB hProb, int PriorCount, int* PriorIndex, 
										  int* PriorValues, int* PriorBranch);

SOLVAPI int    SOLVCALL CoinLoadSos(HPROB hProb, int SosCount, int SosNZCount, 
						   int* SosType, int* SosPrior, int* SosBegin,   
						   int* SosIndex, double* SosRef);

SOLVAPI int    SOLVCALL CoinLoadSemiCont(HPROB hProb, int SemiCount, int* SemiIndex);

SOLVAPI int    SOLVCALL CoinLoadQuadratic(HPROB hProb, int* QuadBegin, int* QuadCount, 
								 int* QuadIndex, double* QuadValues);

SOLVAPI int    SOLVCALL CoinLoadNonlinear(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
								 int* NlpBegin, int* NlpOper, int* NlpArg1, 
								 int* NlpArg2, int* NlpIndex1, int* NlpIndex2, 
								 double* NlpValue1, double* NlpValue2);

SOLVAPI int    SOLVCALL CoinUnloadProblem(HPROB hProb);

SOLVAPI int    SOLVCALL CoinCheckProblem(HPROB hProb);

// Depreciated, use CoinLoadNamesBuf instead
//SOLVAPI int    SOLVCALL CoinSetLoadNamesType(HPROB hProb, int LoadNamesType);

SOLVAPI const char*  SOLVCALL CoinGetProblemName(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetProblemNameBuf(HPROB hProb, char* ProblemName, int buflen);

SOLVAPI int    SOLVCALL CoinGetColCount(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetRowCount(HPROB hProb);

SOLVAPI const char*  SOLVCALL CoinGetColName(HPROB hProb, int col);
SOLVAPI int    SOLVCALL CoinGetColNameBuf(HPROB hProb, int col, char* ColName, int buflen);

SOLVAPI const char*  SOLVCALL CoinGetRowName(HPROB hProb, int row);
SOLVAPI int    SOLVCALL CoinGetRowNameBuf(HPROB hProb, int row, char* RowName, int buflen);

SOLVAPI int    SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback);
SOLVAPI int    SOLVCALL CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback);
SOLVAPI int    SOLVCALL CoinSetMipNodeCallback(HPROB hProb, MIPNODECALLBACK MipNodeCallback);

SOLVAPI int    SOLVCALL CoinOptimizeProblem(HPROB hProb, int Method);

SOLVAPI int    SOLVCALL CoinGetSolutionStatus(HPROB hProb);
SOLVAPI const char*  SOLVCALL CoinGetSolutionText(HPROB hProb, int SolutionStatus);
SOLVAPI int    SOLVCALL CoinGetSolutionTextBuf(HPROB hProb, int SolutionStatus, char* SolutionText, int buflen);

SOLVAPI double SOLVCALL CoinGetObjectValue(HPROB hProb);
SOLVAPI double SOLVCALL CoinGetMipBestBound(HPROB hProb);

SOLVAPI int    SOLVCALL CoinGetIterCount(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetMipNodeCount(HPROB hProb);

SOLVAPI int    SOLVCALL CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
									 double* SlackValues, double* ShadowPrice);
SOLVAPI int    SOLVCALL CoinGetSolutionRanges(HPROB hProb, double* ObjLoRange, double* ObjUpRange,
									 double* RhsLoRange, double* RhsUpRange);
SOLVAPI int    SOLVCALL CoinGetSolutionBasis(HPROB hProb, int* ColStatus, double* RowStatus);



SOLVAPI int    SOLVCALL CoinReadFile(HPROB hProb, int FileType, const char* ReadFilename);
SOLVAPI int    SOLVCALL CoinWriteFile(HPROB hProb, int FileType, const char* WriteFilename);

SOLVAPI int    SOLVCALL CoinOpenLogFile(HPROB hProb, const char* LogFilename);
SOLVAPI int    SOLVCALL CoinCloseLogFile(HPROB hProb);



SOLVAPI int    SOLVCALL CoinGetOptionCount(HPROB hProb);
SOLVAPI int    SOLVCALL CoinLocateOptionID(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinLocateOptionName(HPROB hProb, const char* OptionName);

SOLVAPI int	   SOLVCALL CoinGetOptionID(HPROB hProb, int OptionNr);
SOLVAPI int    SOLVCALL CoinGetOptionInfo(HPROB hProb, int OptionNr, int* OptionID, int* GroupType, int* OptionType);
SOLVAPI int    SOLVCALL CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int* MinValue, int* MaxValue);
SOLVAPI int    SOLVCALL CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double* MinValue, double* MaxValue);
SOLVAPI int    SOLVCALL CoinGetOptionNamesBuf(HPROB hProb, int OptionNr, char* OptionName, char* ShortName, int buflen);


SOLVAPI int	   SOLVCALL CoinGetOptionGroup(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetOptionType(HPROB hProb, int OptionID);

SOLVAPI int    SOLVCALL CoinGetIntOptionDefaultValue(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetIntOptionMinValue(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetIntOptionMaxValue(HPROB hProb, int OptionID);

SOLVAPI double SOLVCALL CoinGetRealOptionDefaultValue(HPROB hProb, int OptionID);
SOLVAPI double SOLVCALL CoinGetRealOptionMinValue(HPROB hProb, int OptionID);
SOLVAPI double SOLVCALL CoinGetRealOptionMaxValue(HPROB hProb, int OptionID);

SOLVAPI const char*  SOLVCALL CoinGetOptionName(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetOptionNameBuf(HPROB hProb, int OptionID, char* OptionName, int buflen);
SOLVAPI const char*  SOLVCALL CoinGetOptionShortName(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetOptionShortNameBuf(HPROB hProb, int OptionID, char* ShortName, int buflen);


SOLVAPI int    SOLVCALL CoinGetOptionChanged(HPROB hProb, int OptionID);

SOLVAPI int    SOLVCALL CoinGetIntOption(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinSetIntOption(HPROB hProb, int OptionID, int IntValue);

SOLVAPI double SOLVCALL CoinGetRealOption(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinSetRealOption(HPROB hProb, int OptionID, double RealValue);

SOLVAPI const char*  SOLVCALL CoinGetStringOption(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinGetStringOptionBuf(HPROB hProb, int OptionID, char* StringValue, int buflen);
SOLVAPI int    SOLVCALL CoinSetStringOption(HPROB hProb, int OptionID, const char* StringValue);


#endif


#ifdef SOLV_LINK_DLL

int    (SOLVCALL *CoinInitSolver)(const char* LicenseStr);
int    (SOLVCALL *CoinFreeSolver)(void);

const char*  (SOLVCALL *CoinGetSolverName)(void);
int    (SOLVCALL *CoinGetSolverNameBuf)(char* SolverName, int buflen);
const char*  (SOLVCALL *CoinGetVersionStr)(void);
int    (SOLVCALL *CoinGetVersionStrBuf)(char* VersionStr, int buflen);
double (SOLVCALL *CoinGetVersion)(void);
int    (SOLVCALL *CoinGetFeatures)(void);
int    (SOLVCALL *CoinGetMethods)(void);
double (SOLVCALL *CoinGetInfinity)(void);

HPROB  (SOLVCALL *CoinCreateProblem)(const char* ProblemName);

int    (SOLVCALL *CoinLoadMatrix)(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues);

int    (SOLVCALL *CoinLoadNames)(HPROB hProb, char** ColNamesList, 
				char** RowNamesList, char* ObjectName);

int    (SOLVCALL *CoinLoadNamesBuf)(HPROB hProb, char* ColNamesBuf, 
				char* RowNamesBuf, char* ObjectName);

int    (SOLVCALL *CoinLoadProblem)(HPROB hProb, 
								   int ColCount, int RowCount, int NonZeroCount, int RangeCount, 
								   int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
								   double* LowerBounds, double* UpperBounds, char* RowType, 
								   double* RHSValues, double* RangeValues, int* MatrixBegin, 
								   int* MatrixCount, int* MatrixIndex, double* MatrixValues,   
								   char** ColNamesList, char** RowNamesList, char* objName);

int    (SOLVCALL *CoinLoadProblemBuf)(HPROB hProb, 
								   int ColCount, int RowCount, int NonZeroCount, int RangeCount, 
								   int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
								   double* LowerBounds, double* UpperBounds, char* RowType, 
								   double* RHSValues, double* RangeValues, int* MatrixBegin, 
								   int* MatrixCount, int* MatrixIndex, double* MatrixValues,   
								   char* ColNamesBuf, char* RowNamesBuf, char* objName);

int    (SOLVCALL *CoinLoadInitValues)(HPROB hProb, double* InitValues);

int    (SOLVCALL *CoinLoadInteger)(HPROB hProb, char* ColumnType);

int    (SOLVCALL *CoinLoadPriority)(HPROB hProb, int PriorCount, int* PriorIndex, 
												int* PriorValues, int* PriorBranch);

int    (SOLVCALL *CoinLoadSos)(HPROB hProb, int SosCount, int SosNZCount, 
										 int* SosType, int* SosPrior, int* SosBegin,   
										 int* SosIndex, double* SosRef);

int    (SOLVCALL *CoinLoadSemiCont)(HPROB hProb, int SemiCount, int* SemiIndex);

int    (SOLVCALL *CoinLoadQuadratic)(HPROB hProb, int* QuadBegin, int* QuadCount, 
												 int* QuadIndex, double* QuadValues);

int    (SOLVCALL *CoinLoadNonlinear)(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
												 int* NlpBegin, int* NlpOper, int* NlpArg1, 
												 int* NlpArg2, int* NlpIndex1, int* NlpIndex2, 
												 double* NlpValue1, double* NlpValue2);

int    (SOLVCALL *CoinUnloadProblem)(HPROB hProb);

int    (SOLVCALL *CoinCheckProblem)(HPROB hProb);

//int    (SOLVCALL *CoinSetLoadNamesType)(HPROB hProb, int LoadNamesType);

const char*  (SOLVCALL *CoinGetProblemName)(HPROB hProb);
int    (SOLVCALL *CoinGetProblemNameBuf)(HPROB hProb, char* ProblemName, int buflen);

int    (SOLVCALL *CoinGetColCount)(HPROB hProb);
int    (SOLVCALL *CoinGetRowCount)(HPROB hProb);

const char*  (SOLVCALL *CoinGetColName)(HPROB hProb, int col);
int    (SOLVCALL *CoinGetColNameBuf)(HPROB hProb, int col, char* ColName, int buflen);
const char*  (SOLVCALL *CoinGetRowName)(HPROB hProb, int row);
int    (SOLVCALL *CoinGetRowNameBuf)(HPROB hProb, int row, char* RowName, int buflen);

void   (SOLVCALL *CoinSetMsgLogCallback)(HPROB hProb, MSGLOGCALLBACK MsgLogCallback);
void   (SOLVCALL *CoinSetIterCallback)(HPROB hProb, ITERCALLBACK IterCallback);
void   (SOLVCALL *CoinSetMipNodeCallback)(HPROB hProb, MIPNODECALLBACK MipNodeCallback);

int    (SOLVCALL *CoinOptimizeProblem)(HPROB hProb, int Method);

int    (SOLVCALL *CoinGetSolutionStatus)(HPROB hProb);
const char* (SOLVCALL *CoinGetSolutionText)(HPROB hProb, int SolutionStatus);
int    (SOLVCALL *CoinGetSolutionTextBuf)(HPROB hProb, int SolutionStatus, char* SolutionText, int buflen);

double (SOLVCALL *CoinGetObjectValue)(HPROB hProb);
double (SOLVCALL *CoinGetMipBestBound)(HPROB hProb);

int    (SOLVCALL *CoinGetIterCount)(HPROB hProb);
int    (SOLVCALL *CoinGetMipNodeCount)(HPROB hProb);

int    (SOLVCALL *CoinGetSolutionValues)(HPROB hProb, double* Activity, double* ReducedCost, 
													  double* SlackValues, double* ShadowPrice);
int    (SOLVCALL *CoinGetSolutionRanges)(HPROB hProb, double* ObjLoRange, double* ObjUpRange,
										           double* RhsLoRange, double* RhsUpRange);
int    (SOLVCALL *CoinGetSolutionBasis)(HPROB hProb, int* ColStatus, double* RowStatus);


void   (SOLVCALL *CoinReadFile)(HPROB hProb, int FileType, const char* ReadFilename);
void   (SOLVCALL *CoinWriteFile)(HPROB hProb, int FileType, const char* WriteFilename);

void   (SOLVCALL *CoinOpenLogFile)(HPROB hProb, const char* LogFilename);
void   (SOLVCALL *CoinCloseLogFile)(HPROB hProb);


int    (SOLVCALL *CoinGetOptionCount)(HPROB hProb);
int    (SOLVCALL *CoinLocateOptionID)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinLocateOptionName)(HPROB hProb, const char* OptionName);

int	   (SOLVCALL *CoinGetOptionID)(HPROB hProb, int OptionNr);
int    (SOLVCALL *CoinGetOptionInfo)(HPROB hProb, int OptionNr, int* OptionID, int* GroupType, int* OptionType);
int    (SOLVCALL *CoinGetIntOptionMinMax)(HPROB hProb, int OptionNr, int* MinValue, int* MaxValue);
int    (SOLVCALL *CoinGetRealOptionMinMax)(HPROB hProb, int OptionNr, double* MinValue, double* MaxValue);
int    (SOLVCALL *CoinGetOptionNames)(HPROB hProb, int OptionNr, char* OptionName, char* ShortName, int buflen);


int    (SOLVCALL *CoinGetOptionGroup)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetOptionType)(HPROB hProb, int OptionID);

int    (SOLVCALL *CoinGetIntOptionDefaultValue)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetIntOptionMinValue)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetIntOptionMaxValue)(HPROB hProb, int OptionID);

double (SOLVCALL *CoinGetRealOptionDefaultValue)(HPROB hProb, int OptionID);
double (SOLVCALL *CoinGetRealOptionMinValue)(HPROB hProb, int OptionID);
double (SOLVCALL *CoinGetRealOptionMaxValue)(HPROB hProb, int OptionID);

const char*  (SOLVCALL *CoinGetOptionName)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetOptionNameBuf)(HPROB hProb, int OptionID, char* OptionName, int buflen);
const char*  (SOLVCALL *CoinGetOptionShortName)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetOptionShortNameBuf)(HPROB hProb, int OptionID, char* ShortName, int buflen);


int    (SOLVCALL *CoinGetOptionChanged)(HPROB hProb, int OptionID);

int    (SOLVCALL *CoinGetIntOption)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinSetIntOption)(HPROB hProb, int OptionID, int IntValue);

double (SOLVCALL *CoinGetRealOption)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinSetRealOption)(HPROB hProb, int OptionID, double RealValue);

const char*  (SOLVCALL *CoinGetStringOption)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinGetStringOptionBuf)(HPROB hProb, int OptionID, char* StringValue, int buflen);
int    (SOLVCALL *CoinSetStringOption)(HPROB hProb, int OptionID, const char* StringValue);

#endif

#ifdef __cplusplus
}
#endif



#define COIN_INT_SOLVEMETHOD     1
#define COIN_INT_PRESOLVETYPE    2
#define COIN_INT_SCALING         3
#define COIN_INT_PERTURBATION    4
#define COIN_INT_PRIMALPIVOTALG  5
#define COIN_INT_DUALPIVOTALG    6
#define COIN_INT_LOGLEVEL        7
#define COIN_INT_MAXITER         8
#define COIN_INT_CRASHIND        9     
#define COIN_INT_CRASHPIVOT     10
#define COIN_REAL_CRASHGAP      11
#define COIN_REAL_PRIMALOBJLIM  12
#define COIN_REAL_DUALOBJLIM    13
#define COIN_REAL_PRIMALOBJTOL  14
#define COIN_REAL_DUALOBJTOL    15
#define COIN_REAL_MAXSECONDS    16

#define COIN_INT_MIPMAXNODES    17
#define COIN_INT_MIPMAXSOL      18
#define COIN_REAL_MIPMAXSEC     19

#define COIN_INT_MIPFATHOMDISC     20
#define COIN_INT_MIPHOTSTART       21
//#define COIN_INT_MIPFORCEPRIOR     21
#define COIN_INT_MIPMINIMUMDROP    22
#define COIN_INT_MIPMAXCUTPASS     23
#define COIN_INT_MIPMAXPASSROOT    24
#define COIN_INT_MIPSTRONGBRANCH   25
#define COIN_INT_MIPSCANGLOBCUTS   26

#define COIN_REAL_MIPINTTOL        30
#define COIN_REAL_MIPINFWEIGHT     31
#define COIN_REAL_MIPCUTOFF        32
#define COIN_REAL_MIPABSGAP        33
#define COIN_REAL_MIPFRACGAP       34

#define COIN_INT_MIPCUT_PROBING          110
#define COIN_INT_MIPPROBE_FREQ           111
#define COIN_INT_MIPPROBE_MODE           112
#define COIN_INT_MIPPROBE_USEOBJ         113
#define COIN_INT_MIPPROBE_MAXPASS        114
#define COIN_INT_MIPPROBE_MAXPROBE       115
#define COIN_INT_MIPPROBE_MAXLOOK        116
#define COIN_INT_MIPPROBE_ROWCUTS        117

#define COIN_INT_MIPCUT_GOMORY           120
#define COIN_INT_MIPGOMORY_FREQ          121
#define COIN_INT_MIPGOMORY_LIMIT         122
#define COIN_REAL_MIPGOMORY_AWAY         123


#define COIN_INT_MIPCUT_KNAPSACK         130
#define COIN_INT_MIPKNAPSACK_FREQ        131
#define COIN_INT_MIPKNAPSACK_MAXIN       132

#define COIN_INT_MIPCUT_ODDHOLE          140
#define COIN_INT_MIPODDHOLE_FREQ         141
#define COIN_REAL_MIPODDHOLE_MINVIOL     142
#define COIN_REAL_MIPODDHOLE_MINVIOLPER  143
#define COIN_INT_MIPODDHOLE_MAXENTRIES   144

#define COIN_INT_MIPCUT_CLIQUE           150
#define COIN_INT_MIPCLIQUE_FREQ          151
#define COIN_INT_MIPCLIQUE_PACKING       152
#define COIN_INT_MIPCLIQUE_STAR          153
#define COIN_INT_MIPCLIQUE_STARMETHOD    154
#define COIN_INT_MIPCLIQUE_STARMAXLEN    155
#define COIN_INT_MIPCLIQUE_STARREPORT    156
#define COIN_INT_MIPCLIQUE_ROW           157
#define COIN_INT_MIPCLIQUE_ROWMAXLEN     158
#define COIN_INT_MIPCLIQUE_ROWREPORT     159
#define COIN_REAL_MIPCLIQUE_MINVIOL      160

#define COIN_INT_MIPCUT_LIFTPROJECT      170
#define COIN_INT_MIPLIFTPRO_FREQ         171
#define COIN_INT_MIPLIFTPRO_BETAONE      172

#define COIN_INT_MIPCUT_SIMPROUND        180
#define COIN_INT_MIPSIMPROUND_FREQ       181   

#define COIN_INT_MIPUSECBCMAIN           200   


#endif  /* _COINMP_H_ */
