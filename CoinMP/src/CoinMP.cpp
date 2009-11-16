/************************************************************************/
/*                                                                      */
/*  CoinMP Solver                                     Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.cpp'                                        */
/*                                                                      */
/*  Version      :  1.5                                                 */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson, Maximal Software               */
/*                                                                      */
/*  Copyright (c) 2002-2009                     Bjarni Kristjansson     */
/*                                                                      */
/************************************************************************/

// $Id$

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

#include "CoinProblem.h"
#include "CoinResult.h"
#include "CoinSolver.h"

#include "CoinCbc.h"


#ifndef COIN_DBL_MAX
#define COIN_DBL_MAX DBL_MAX
#endif


#define SOLVER_EXPORT
#define SOLV_LINK_LIB
#include "CoinMP.h"


#define NEW_STYLE_CBCMAIN


const double COINMP_VERSION    = 1.5;
const char*  COINMP_VERSIONSTR = "1.5";
const char*  COINMP_NAME       = "CoinMP";


/************************************************************************/
/*  Solver Initialization                                               */
/************************************************************************/


SOLVAPI int SOLVCALL CoinInitSolver(const char* LicenseStr)
{
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinFreeSolver(void)
{
	return SOLV_CALL_SUCCESS;
}

/************************************************************************/
/*  Solver Queries                                                      */
/************************************************************************/


SOLVAPI const char* SOLVCALL CoinGetSolverName(void)
{
	return COINMP_NAME;
}


SOLVAPI int  SOLVCALL CoinGetSolverNameBuf(char* SolverName, int buflen)
{
	strncpy(SolverName, CoinGetSolverName(), buflen-1);
	SolverName[buflen-1] = '\0';
	return (int)strlen(SolverName);
}


SOLVAPI const char* SOLVCALL CoinGetVersionStr(void)
{
	return COINMP_VERSIONSTR;
}


SOLVAPI int  SOLVCALL CoinGetVersionStrBuf(char* VersionStr, int buflen)
{
	strncpy(VersionStr, CoinGetVersionStr(), buflen-1);
	VersionStr[buflen-1] = '\0';
	return (int)strlen(VersionStr);
}


SOLVAPI double SOLVCALL CoinGetVersion(void)
{
	return COINMP_VERSION;
}


SOLVAPI int SOLVCALL CoinGetFeatures(void)
{
	return SOLV_FEATURE_LP |
			SOLV_FEATURE_MIP;
}


SOLVAPI int SOLVCALL CoinGetMethods(void)
{
	return SOLV_METHOD_PRIMAL |
			SOLV_METHOD_DUAL | 
			SOLV_METHOD_BARRIER;
}



SOLVAPI double SOLVCALL CoinGetInfinity(void)
{
	return COIN_DBL_MAX;
}


/************************************************************************/
/*  Coin Problem Info Structure                                         */
/************************************************************************/


typedef struct {
				PPROBLEM  pProblem;
				PRESULT   pResult;
				PSOLVER   pSolver;

				} COININFO, *PCOIN;
 


//PCOIN global_pCoin;



/************************************************************************/
/*  Create/Load Problem                                                 */
/************************************************************************/


SOLVAPI HPROB SOLVCALL CoinCreateProblem(const char* ProblemName)
{
	PCOIN pCoin;

	pCoin = (PCOIN) malloc(sizeof(COININFO));

	pCoin->pProblem = coinCreateProblemObject();
	pCoin->pResult = coinCreateResultObject();
	pCoin->pSolver = coinCreateSolverObject();

	coinSetProblemName(pCoin->pProblem, ProblemName);
	coinCopyOptionTable(pCoin->pSolver, CbcGetOptionTable(), CbcGetOptionCount());

	return (HPROB)pCoin;
}


SOLVAPI int SOLVCALL CoinUnloadProblem(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;
	
	if (pCoin) {
		coinClearProblemObject(pCoin->pProblem);
		coinClearResultObject(pCoin->pResult);
		coinClearSolverObject(pCoin->pSolver);
	}
	free(pCoin);
	pCoin = NULL;
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadMatrix(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!coinStoreMatrix(pProblem, ColCount, RowCount, NZCount, RangeCount, ObjectSense,
						 ObjectConst, ObjectCoeffs, LowerBounds, UpperBounds, RowType,
						 RHSValues, RangeValues, MatrixBegin, MatrixCount, MatrixIndex,
						 MatrixValues)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadNames(HPROB hProb, char** ColNamesList, char** RowNamesList, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStoreNamesList(pCoin->pProblem, ColNamesList, RowNamesList, ObjectName)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadNamesBuf(HPROB hProb, char* ColNamesBuf, char* RowNamesBuf, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStoreNamesBuf(pCoin->pProblem, ColNamesBuf, RowNamesBuf, ObjectName)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadProblem(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char** ColNamesList, char** RowNamesList, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!coinStoreMatrix(pProblem, ColCount, RowCount, NZCount, RangeCount, ObjectSense,
						 ObjectConst, ObjectCoeffs, LowerBounds, UpperBounds, RowType,
						 RHSValues, RangeValues, MatrixBegin, MatrixCount, MatrixIndex,
						 MatrixValues)) {
		return SOLV_CALL_FAILED;
	}
	if (!coinStoreNamesList(pProblem, ColNamesList, RowNamesList, ObjectName)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}

// CoinLoadProblemBuf accepts ColNamesBuf/RowNamesBuf as character buffer with \0 after each name

SOLVAPI int SOLVCALL CoinLoadProblemBuf(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char* ColNamesBuf, char* RowNamesBuf, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!coinStoreMatrix(pProblem, ColCount, RowCount, NZCount, RangeCount, ObjectSense,
						 ObjectConst, ObjectCoeffs, LowerBounds, UpperBounds, RowType,
						 RHSValues, RangeValues, MatrixBegin, MatrixCount, MatrixIndex,
						 MatrixValues)) {
		return SOLV_CALL_FAILED;
	}
	if (!coinStoreNamesBuf(pProblem, ColNamesBuf, RowNamesBuf, ObjectName)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}




SOLVAPI int SOLVCALL CoinLoadInitValues(HPROB hProb, double* InitValues)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStoreInitValues(pCoin->pProblem, InitValues)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadInteger(HPROB hProb, char* ColType)
{   
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!coinStoreInteger(pProblem, ColType)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadPriority(HPROB hProb, int PriorCount, int* PriorIndex, 
									  int* PriorValues, int* PriorBranch)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStorePriority(pCoin->pProblem, PriorCount, PriorIndex, PriorValues, PriorBranch)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadSos(HPROB hProb, int SosCount, int SosNZCount, 
								int* SosType, int* SosPrior, int* SosBegin, 
								int* SosIndex, double* SosRef)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStoreSos(pCoin->pProblem, SosCount, SosNZCount, SosType, SosPrior, 
		SosBegin, SosIndex, SosRef)) {
			return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadSemiCont(HPROB hProb, int SemiCount, int* SemiIndex)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!coinStoreSemiCont(pCoin->pProblem, SemiCount, SemiIndex)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadQuadratic(HPROB hProb, int* QuadBegin, int* QuadCount, 
										int* QuadIndex, double* QuadValues)
{
	return SOLV_CALL_FAILED;
}


SOLVAPI int SOLVCALL CoinLoadNonlinear(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
										int* NlpBegin, int* NlpOper, int* NlpArg1, 
										int* NlpArg2, int* NlpIndex1, int* NlpIndex2, 
										double* NlpValue1, double* NlpValue2)
{
	return SOLV_CALL_FAILED;
}




#define CHECK_MAXNAMELEN   100


SOLVAPI int SOLVCALL CoinCheckProblem(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	int i;

	if (pProblem->ColCount == 0) {
		return SOLV_CHECK_COLCOUNT;
	}
	if ((pProblem->RowCount < 0) ||
		(pProblem->NZCount < 0) ||
		(pProblem->RangeCount < 0)) {
		return SOLV_CHECK_ROWCOUNT;
	}
	if ((pProblem->RangeCount > pProblem->RowCount)) {
		return SOLV_CHECK_RANGECOUNT;
	}
	if ((pProblem->ObjectSense < -1) || 
		(pProblem->ObjectSense > 1)) {
		return SOLV_CHECK_OBJSENSE;
	}
	if (pProblem->RowType && (pProblem->RowCount > 0)) {
		for (i = 0; i < pProblem->RowCount; i++) {
			if ((pProblem->RowType[i] != 'L') &&
				(pProblem->RowType[i] != 'E') &&
				(pProblem->RowType[i] != 'G') &&
				(pProblem->RowType[i] != 'R') &&
				(pProblem->RowType[i] != 'N')) {
				return SOLV_CHECK_ROWTYPE;
			}
		}
	}
	if (pProblem->NZCount > 0) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if (pProblem->MatrixBegin[i] < 0) {
				return SOLV_CHECK_MATBEGIN;
			}
			if (pProblem->MatrixCount[i] < 0) {
				return SOLV_CHECK_MATCOUNT;
			}
			if (pProblem->MatrixBegin[i+1] - pProblem->MatrixBegin[i] != pProblem->MatrixCount[i]) {
				return SOLV_CHECK_MATBEGCNT;
			}
		}
		if (pProblem->MatrixBegin[pProblem->ColCount] != pProblem->NZCount) {
			return 100 + pProblem->MatrixBegin[pProblem->ColCount];
		}
		for (i = 0; i < pProblem->NZCount; i++) {
			if (pProblem->MatrixIndex[i] < 0) {
				return SOLV_CHECK_MATINDEX;
			}
			if (pProblem->MatrixIndex[i] >= pProblem->RowCount) {
				return SOLV_CHECK_MATINDEXROW;
			}
		}
	}
	if (pProblem->LowerBounds && pProblem->UpperBounds) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if (pProblem->LowerBounds[i] > pProblem->UpperBounds[i]) {
				return SOLV_CHECK_BOUNDS;
			}
		}
	}
	if (pProblem->ColType) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if ((pProblem->ColType[i] != 'C') &&
				(pProblem->ColType[i] != 'B') &&
				(pProblem->ColType[i] != 'I')) {
				return SOLV_CHECK_COLTYPE;
			}
		}
	}
	if (pProblem->ColNamesBuf) {
		if (pProblem->lenColNamesBuf <= 0) {
			return SOLV_CHECK_COLNAMES;
		}
		if (pProblem->lenColNamesBuf > pProblem->ColCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_COLNAMESLEN;
		}
	}
	if (pProblem->RowNamesBuf) {
		if (pProblem->lenRowNamesBuf <= 0) {
			return SOLV_CHECK_ROWNAMES;
		}
		if (pProblem->lenRowNamesBuf > pProblem->RowCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_ROWNAMSLEN;
		}
	}
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Problem Queries                                                     */
/************************************************************************/


SOLVAPI const char* SOLVCALL CoinGetProblemName(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->ProblemName;
}


SOLVAPI int SOLVCALL CoinGetProblemNameBuf(HPROB hProb, char* ProbName, int buflen)
{
	strncpy(ProbName, CoinGetProblemName(hProb), buflen-1);
	ProbName[buflen-1] = '\0';
	return (int)strlen(ProbName);
}



SOLVAPI int SOLVCALL CoinGetColCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->ColCount;
}


SOLVAPI int SOLVCALL CoinGetRowCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->RowCount;
}



SOLVAPI const char* SOLVCALL CoinGetColName(HPROB hProb, int col)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->ColNamesList[col];
}


SOLVAPI int SOLVCALL CoinGetColNameBuf(HPROB hProb, int col, char* ColName, int buflen)
{
	strncpy(ColName, CoinGetColName(hProb, col), buflen-1);
	ColName[buflen-1] = '\0';
	return (int)strlen(ColName);
}


SOLVAPI const char* SOLVCALL CoinGetRowName(HPROB hProb, int row)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->RowNamesList[row];
}


SOLVAPI int SOLVCALL CoinGetRowNameBuf(HPROB hProb, int row, char* RowName, int buflen)
{
	strncpy(RowName, CoinGetRowName(hProb, row), buflen-1);
	RowName[buflen-1] = '\0';
	return (int)strlen(RowName);
}



SOLVAPI int SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MsgLogCallback = MsgLogCallback;
	return SOLV_CALL_SUCCESS;
}

SOLVAPI int SOLVCALL CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->IterCallback = IterCallback;
	return SOLV_CALL_SUCCESS;
}

SOLVAPI int SOLVCALL CoinSetMipNodeCallback(HPROB hProb, MIPNODECALLBACK MipNodeCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MipNodeCallback = MipNodeCallback;
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Optimization                                                        */
/************************************************************************/


SOLVAPI int SOLVCALL CoinOptimizeProblem(HPROB hProb, int Method)
{		
	PCOIN pCoin = (PCOIN)hProb;

	return CbcOptimizeProblem(pCoin->pProblem, pCoin->pResult, pCoin->pSolver, Method);
}



/************************************************************************/
/*  Solution status                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionStatus(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionStatus;
}


  

SOLVAPI const char* SOLVCALL CoinGetSolutionText(HPROB hProb, int SolutionStatus)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionText;
}


SOLVAPI int SOLVCALL CoinGetSolutionTextBuf(HPROB hProb, int SolutionStatus, char* SolutionText, int buflen)
{
	strncpy(SolutionText, CoinGetSolutionText(hProb, SolutionStatus), buflen-1);
	SolutionText[buflen-1] = '\0';
	return (int)strlen(SolutionText);
}



SOLVAPI double SOLVCALL CoinGetObjectValue(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->ObjectValue + pCoin->pProblem->ObjectConst;
}


SOLVAPI double SOLVCALL CoinGetMipBestBound(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->MipBestBound;
}



SOLVAPI int SOLVCALL CoinGetIterCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->IterCount;
}


SOLVAPI int SOLVCALL CoinGetMipNodeCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->MipNodeCount;
}



/************************************************************************/
/*  Solution retrieval                                                  */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
											 double* SlackValues, double* ShadowPrice)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (Activity && pResult->ColActivity) {
		memcpy(Activity, pResult->ColActivity, pProblem->ColCount * sizeof(double));
	}
	if (ReducedCost && pResult->ReducedCost) {
		memcpy(ReducedCost, pResult->ReducedCost, pProblem->ColCount * sizeof(double));
	}
	if (SlackValues && pResult->SlackValues) {
		memcpy(SlackValues, pResult->SlackValues, pProblem->RowCount * sizeof(double));
	}
	if (ShadowPrice && pResult->ShadowPrice) {
		memcpy(ShadowPrice, pResult->ShadowPrice, pProblem->RowCount * sizeof(double));
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinGetSolutionRanges(HPROB hProb, double* ObjLoRange, double* ObjUpRange,
											 double* RhsLoRange, double* RhsUpRange)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (ObjLoRange && pResult->ObjLoRange) {
		memcpy(ObjLoRange, pResult->ObjLoRange, pProblem->ColCount * sizeof(double));
	}
	if (ObjUpRange && pResult->ObjUpRange) {
		memcpy(ObjUpRange, pResult->ObjUpRange, pProblem->ColCount * sizeof(double));
	}
	if (RhsLoRange && pResult->RhsLoRange) {
		memcpy(RhsLoRange, pResult->RhsLoRange, pProblem->RowCount * sizeof(double));
	}
	if (RhsUpRange && pResult->RhsUpRange) {
		memcpy(RhsUpRange, pResult->RhsUpRange, pProblem->RowCount * sizeof(double));
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinGetSolutionBasis(HPROB hProb, int* ColStatus, double* RowStatus)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (ColStatus && pResult->ColStatus) {
		memcpy(ColStatus, pResult->ColStatus, pProblem->ColCount * sizeof(int));
	}
	if (RowStatus && pResult->RowStatus) {
		memcpy(RowStatus, pResult->RowStatus, pProblem->RowCount * sizeof(int));
	}
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  File Handling                                                       */
/************************************************************************/


SOLVAPI int SOLVCALL CoinReadFile(HPROB hProb, int FileType, const char* ReadFilename)
{
   PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		
			return CbcReadMpsFile(pCoin->pProblem, ReadFilename);

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinWriteFile(HPROB hProb, int FileType, const char* WriteFilename)
{
	PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		
			return CbcWriteMpsFile(pCoin->pProblem, WriteFilename);   

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_CALL_FAILED;

	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinOpenLogFile(HPROB hProb, const char* logFilename)
{
	PCOIN pCoin = (PCOIN)hProb;

	strcpy(pCoin->pSolver->LogFilename, logFilename);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinCloseLogFile(HPROB hProb)
{
	return SOLV_CALL_SUCCESS;
}


/************************************************************************/
/*  Option Handling                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetOptionCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionCount(pCoin->pSolver);
}

SOLVAPI int    SOLVCALL CoinLocateOptionID(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinLocateOptionID(pCoin->pSolver, OptionID);
}


SOLVAPI int    SOLVCALL CoinLocateOptionName(HPROB hProb, char* OptionName)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinLocateOptionName(pCoin->pSolver, OptionName);
}



SOLVAPI int SOLVCALL CoinGetOptionID(HPROB hProb, int OptionNr)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionID(pCoin->pSolver, OptionNr);
}


SOLVAPI int SOLVCALL CoinGetOptionInfo(HPROB hProb, int OptionNr, int* OptionID, int* GroupType, int* OptionType)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pSolver, OptionNr);
	if (OptionID)   *OptionID = optionID;
	if (OptionType) *OptionType = coinGetOptionType(pCoin->pSolver, optionID);
	if (GroupType)  *GroupType = coinGetOptionGroup(pCoin->pSolver, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int* MinValue, int* MaxValue)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pSolver, OptionNr);
	if (MinValue)   *MinValue = coinGetIntOptionMinValue(pCoin->pSolver, optionID);
	if (MaxValue)   *MaxValue = coinGetIntOptionMaxValue(pCoin->pSolver, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double* MinValue, double* MaxValue)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pSolver, OptionNr);
	if (MinValue)   *MinValue = coinGetRealOptionMinValue(pCoin->pSolver, optionID);
	if (MaxValue)   *MaxValue = coinGetRealOptionMaxValue(pCoin->pSolver, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetOptionNamesBuf(HPROB hProb, int OptionNr, char* OptionName, 
												char* ShortName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pSolver, OptionNr);
	if (OptionName) coinGetOptionNameBuf(pCoin->pSolver, optionID, OptionName, buflen);
	if (ShortName) coinGetOptionShortNameBuf(pCoin->pSolver, optionID, ShortName, buflen);
	return SOLV_CALL_SUCCESS;
}





SOLVAPI int SOLVCALL CoinGetOptionGroup(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionGroup(pCoin->pSolver, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionType(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionType(pCoin->pSolver, OptionID);
}




SOLVAPI int    SOLVCALL CoinGetIntOptionDefaultValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionDefaultValue(pCoin->pSolver, OptionID);
}


SOLVAPI int    SOLVCALL CoinGetIntOptionMinValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionMinValue(pCoin->pSolver, OptionID);
}


SOLVAPI int    SOLVCALL CoinGetIntOptionMaxValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionMaxValue(pCoin->pSolver, OptionID);
}



SOLVAPI double SOLVCALL CoinGetRealOptionDefaultValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionDefaultValue(pCoin->pSolver, OptionID);
}


SOLVAPI double SOLVCALL CoinGetRealOptionMinValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionMinValue(pCoin->pSolver, OptionID);
}


SOLVAPI double SOLVCALL CoinGetRealOptionMaxValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionMaxValue(pCoin->pSolver, OptionID);
}




SOLVAPI const char* SOLVCALL CoinGetOptionName(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionName(pCoin->pSolver, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionNameBuf(HPROB hProb, int OptionID, char* OptionName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionNameBuf(pCoin->pSolver, OptionID, OptionName, buflen);
}


SOLVAPI const char* SOLVCALL CoinGetOptionShortName(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionShortName(pCoin->pSolver, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionShortNameBuf(HPROB hProb, int OptionID, char* ShortName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionShortNameBuf(pCoin->pSolver, OptionID, ShortName, buflen);
}




SOLVAPI int SOLVCALL CoinGetOptionChanged(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionChanged(pCoin->pSolver, OptionID);
}


SOLVAPI int SOLVCALL CoinGetIntOption(HPROB hProb, int OptionID)
{   
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOption(pCoin->pSolver, OptionID);
}
  	


SOLVAPI int SOLVCALL CoinSetIntOption(HPROB hProb, int OptionID, int IntValue)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinSetIntOption(pCoin->pSolver, OptionID, IntValue);
}



SOLVAPI double SOLVCALL CoinGetRealOption(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOption(pCoin->pSolver, OptionID);
}



SOLVAPI int SOLVCALL CoinSetRealOption(HPROB hProb, int OptionID, double RealValue)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinSetRealOption(pCoin->pSolver, OptionID, RealValue);
}


SOLVAPI const char* SOLVCALL CoinGetStringOption(HPROB hProb, int OptionID)
{
   return "";
}


SOLVAPI int SOLVCALL CoinGetStringOptionBuf(HPROB hProb, int OptionID, char* StringValue, int buflen)
{
   return SOLV_CALL_FAILED;
}


SOLVAPI int SOLVCALL CoinSetStringOption(HPROB hProb, int OptionID, const char* StringValue)
{
   return SOLV_CALL_FAILED;
}



/************************************************************************/
/*  DLL Handling                                                        */
/************************************************************************/


#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


static HINSTANCE g_hInstance;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   switch (dwReason) {

      case DLL_PROCESS_ATTACH:
         g_hInstance = hInstance;
		   break;

      case DLL_PROCESS_DETACH:
	      break;
   }
   return 1;
}
#endif

