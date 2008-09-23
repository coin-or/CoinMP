/************************************************************************/
/*                                                                      */
/*  COINMP.DLL                                        Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.h'                                          */
/*                                                                      */
/*  Version      :  1.2                                                 */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson, Maximal Software               */
/*                                                                      */
/*  Copyright (c) 2002-2008                     Bjarni Kristjansson     */
/*                                                                      */
/************************************************************************/


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



#define SOLV_CALL_SUCCESS   1
#define SOLV_CALL_FAILED    0

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



typedef void *HPROB;

//#ifdef __cplusplus
extern "C" {
//#endif

typedef int (*MSGLOGCALLBACK)(char *MessageStr);

typedef int (*ITERCALLBACK)(int    IterCount, 
							double ObjectValue,
							int    IsFeasible, 
							double InfeasValue);

typedef int (*MIPNODECALLBACK)(int    IterCount, 
							   int	  MipNodeCount,
							   double BestBound,
							   double BestInteger,
							   int    IsMipImproved);



#ifdef SOLV_LINK_LIB

SOLVAPI int    SOLVCALL CoinInitSolver(char* LicenseStr);
SOLVAPI int    SOLVCALL CoinFreeSolver(void);

SOLVAPI int    SOLVCALL CoinGetSolverName(char* SolverName);
SOLVAPI int    SOLVCALL CoinGetVersionStr(char* VersionStr);
SOLVAPI double SOLVCALL CoinGetVersion(void);
SOLVAPI int    SOLVCALL CoinGetFeatures(void);
SOLVAPI int    SOLVCALL CoinGetMethods(void);
SOLVAPI double SOLVCALL CoinGetInfinity(void);

SOLVAPI HPROB  SOLVCALL CoinCreateProblem(char *ProblemName);

SOLVAPI int    SOLVCALL CoinLoadProblem(HPROB hProb, 
							   int ColCount, int RowCount, int NonZeroCount, int RangeCount, 
							   int ObjectSense, double* ObjectCoeffs, double ObjectConst,
							   double* RHSValues,  double* RangeValues, char* RowType, 
							   int* MatrixBegin, int* MatrixCount,   int* MatrixIndex,    
							   double* MatrixValues, double* LowerBounds, double* UpperBounds, 
							   double* InitValues, char* ColNames, char* RowNames, char *objName);

SOLVAPI int    SOLVCALL CoinLoadInteger(HPROB hProb, char* ColumnType);


SOLVAPI int    SOLVCALL CoinLoadPriority(HPROB hProb, int PriorCount, int *PriorIndex, 
										  int *PriorValues, int *BranchDir);

SOLVAPI int    SOLVCALL CoinLoadSos(HPROB hProb, int SosCount, int SosNZCount, 
						   char *SosType, int *SosPrior, int *SosBegin,   
						   int *SosIndex, double *SosRef);

SOLVAPI int    SOLVCALL CoinLoadQuadratic(HPROB hProb, int *QuadBegin, int *QuadCount, 
								 int *QuadIndex, double *QuadValues);

SOLVAPI int    SOLVCALL CoinLoadNonlinear(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
								 int *NlpBegin, int *NlpOper, int *NlpArg1, 
								 int *NlpArg2, int *NlpIndex1, int *NlpIndex2, 
								 double *NlpValue1, double *NlpValue2);

SOLVAPI int    SOLVCALL CoinUnloadProblem(HPROB hProb);

SOLVAPI int    SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback);
SOLVAPI int    SOLVCALL CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback);
SOLVAPI int    SOLVCALL CoinSetMipNodeCallback(HPROB hProb, MIPNODECALLBACK MipNodeCallback);

SOLVAPI int    SOLVCALL CoinOptimizeProblem(HPROB hProb, int Method);

SOLVAPI int    SOLVCALL CoinGetSolutionStatus(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetSolutionText(HPROB hProb, int SolutionStatus, char* SolutionText);
SOLVAPI double SOLVCALL CoinGetObjectValue(HPROB hProb);
SOLVAPI double SOLVCALL CoinGetMipBestBound(HPROB hProb);

SOLVAPI int    SOLVCALL CoinGetIterCount(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetMipNodeCount(HPROB hProb);

SOLVAPI int    SOLVCALL CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
									 double* SlackValues, double* ShadowPrice);
SOLVAPI int    SOLVCALL CoinGetSolutionRanges(HPROB hProb, double *ObjLoRange, double *ObjUpRange,
									 double *RhsLoRange, double *RhsUpRange);
SOLVAPI int    SOLVCALL CoinGetSolutionBasis(HPROB hProb, int *ColStatus, double *RowStatus);

SOLVAPI int    SOLVCALL CoinReadFile(HPROB hProb, int FileType, char *ReadFilename);
SOLVAPI int    SOLVCALL CoinWriteFile(HPROB hProb, int FileType, char *WriteFilename);

SOLVAPI int    SOLVCALL CoinOpenLogFile(HPROB hProb, char *LogFilename);
SOLVAPI int    SOLVCALL CoinCloseLogFile(HPROB hProb);

SOLVAPI int    SOLVCALL CoinGetOptionCount(HPROB hProb);
SOLVAPI int    SOLVCALL CoinGetOptionInfo(HPROB hProb, int OptionNr, int *OptionID, int *GroupType,
								 int *OptionType, char *OptionName, char *ShortName);

SOLVAPI int    SOLVCALL CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int *MinValue, int *MaxValue);
SOLVAPI int    SOLVCALL CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double *MinValue, double *MaxValue);

SOLVAPI int    SOLVCALL CoinGetOptionChanged(HPROB hProb, int OptionID);

SOLVAPI int    SOLVCALL CoinGetIntOption(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinSetIntOption(HPROB hProb, int OptionID, int IntValue);

SOLVAPI double SOLVCALL CoinGetRealOption(HPROB hProb, int OptionID);
SOLVAPI int    SOLVCALL CoinSetRealOption(HPROB hProb, int OptionID, double RealValue);

SOLVAPI int    SOLVCALL CoinGetStringOption(HPROB hProb, int OptionID, char* StringValue);
SOLVAPI int    SOLVCALL CoinSetStringOption(HPROB hProb, int OptionID, char* StringValue);


#endif


#ifdef SOLV_LINK_DLL

int    (SOLVCALL *CoinInitSolver)(char *LicenseStr);
int    (SOLVCALL *CoinFreeSolver)(void);

int    (SOLVCALL *CoinGetSolverName)(char *SolverName);
int    (SOLVCALL *CoinGetVersionStr)(char *VersionStr);
double (SOLVCALL *CoinGetVersion)(void);
int    (SOLVCALL *CoinGetFeatures)(void);
int    (SOLVCALL *CoinGetMethods)(void);
double (SOLVCALL *CoinGetInfinity)(void);

HPROB  (SOLVCALL *CoinCreateProblem)(char *ProblemName);

int    (SOLVCALL *CoinLoadProblem)(HPROB hProb, 
								   int ColCount, int RowCount, int NonZeroCount, int RangeCount, 
								   int ObjectSense, double* ObjectCoeffs, double ObjectConst,
								   double* RHSValues, double* RangeValues, char* RowType, 
								   int* MatrixBegin, int* MatrixCount, int* MatrixIndex,    
								   double* MatrixValues, double* LowerBounds, double* UpperBounds, 
								   double* InitValues, char* ColNames, char* RowNames, char *objName);

int    (SOLVCALL *CoinLoadInteger)(HPROB hProb, char* ColumnType);

int    (SOLVCALL *CoinLoadPriority)(HPROB hProb, int PriorCount, int *PriorIndex, 
												int *PriorValues, int *BranchDir);

int    (SOLVCALL *CoinLoadSos)(HPROB hProb, int SosCount, int SosNZCount, 
										 char *SosType, int *SosPrior, int *SosBegin,   
										 int *SosIndex, double *SosRef);

int    (SOLVCALL *CoinLoadQuadratic)(HPROB hProb, int *QuadBegin, int *QuadCount, 
												 int *QuadIndex, double *QuadValues);

int    (SOLVCALL *CoinLoadNonlinear)(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
												 int *NlpBegin, int *NlpOper, int *NlpArg1, 
												 int *NlpArg2, int *NlpIndex1, int *NlpIndex2, 
												 double *NlpValue1, double *NlpValue2);

int    (SOLVCALL *CoinUnloadProblem)(HPROB hProb);

void   (SOLVCALL *CoinSetMsgLogCallback)(HPROB hProb, MSGLOGCALLBACK MsgLogCallback);
void   (SOLVCALL *CoinSetIterCallback)(HPROB hProb, ITERCALLBACK IterCallback);
void   (SOLVCALL *CoinSetMipNodeCallback)(HPROB hProb, MIPNODECALLBACK MipNodeCallback);

int    (SOLVCALL *CoinOptimizeProblem)(HPROB hProb, int Method);

int    (SOLVCALL *CoinGetSolutionStatus)(HPROB hProb);
int    (SOLVCALL *CoinGetSolutionText)(HPROB hProb, int SolutionStatus, char* SolutionText);
double (SOLVCALL *CoinGetObjectValue)(HPROB hProb);
double (SOLVCALL *CoinGetMipBestBound)(HPROB hProb);

int    (SOLVCALL *CoinGetIterCount)(HPROB hProb);
int    (SOLVCALL *CoinGetMipNodeCount)(HPROB hProb);

int    (SOLVCALL *CoinGetSolutionValues)(HPROB hProb, double* Activity, double* ReducedCost, 
													  double* SlackValues, double* ShadowPrice);
int    (SOLVCALL *CoinGetSolutionRanges)(HPROB hProb, double *ObjLoRange, double *ObjUpRange,
										           double *RhsLoRange, double *RhsUpRange);
int    (SOLVCALL *CoinGetSolutionBasis)(HPROB hProb, int *ColStatus, double *RowStatus);

void   (SOLVCALL *CoinReadFile)(HPROB hProb, int FileType, char* ReadFilename);
void   (SOLVCALL *CoinWriteFile)(HPROB hProb, int FileType, char* WriteFilename);

void   (SOLVCALL *CoinOpenLogFile)(HPROB hProb, char *LogFilename);
void   (SOLVCALL *CoinCloseLogFile)(HPROB hProb);

int    (SOLVCALL *CoinGetOptionCount)(HPROB hProb);
int    (SOLVCALL *CoinGetOptionInfo)(HPROB hProb, int OptionNr, int *OptionID, 
									int *OptionType, char *OptionName, char *ShortName);

int    (SOLVCALL *CoinGetIntOptionMinMax)(HPROB hProb, int OptionNr, int *MinValue, int *MaxValue);
int    (SOLVCALL *CoinGetRealOptionMinMax)(HPROB hProb, int OptionNr, double *MinValue, double *MaxValue);

int    (SOLVCALL *CoinGetOptionChanged)(HPROB hProb, int OptionID);

int    (SOLVCALL *CoinGetIntOption)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinSetIntOption)(HPROB hProb, int OptionID, int IntValue);

double (SOLVCALL *CoinGetRealOption)(HPROB hProb, int OptionID);
int    (SOLVCALL *CoinSetRealOption)(HPROB hProb, int OptionID, double RealValue);

int    (SOLVCALL *CoinGetStringOption)(HPROB hProb, int OptionID, char* StringValue);
int    (SOLVCALL *CoinSetStringOption)(HPROB hProb, int OptionID, char *StringValue);

#endif

//#ifdef __cplusplus
}
//#endif



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
