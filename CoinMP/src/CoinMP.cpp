/************************************************************************/
/*                                                                      */
/*  CoinMP Solver                                     Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.cpp'                                        */
/*                                                                      */
/*  Version      :  1.8                                                 */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson, Maximal Software               */
/*                                                                      */
/*  Copyright (c) 2002-2015                     Bjarni Kristjansson     */
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
#include "CoinOption.h"

#include "CoinCbc.h"

#include "config_coinmp_default.h"


#define SOLVER_EXPORT
#define SOLV_LINK_LIB
#include "CoinMP.h"


const char*  COINMP_VERSIONSTR = "1.8.4";
const int    COINMP_VERSIONINT = 184;
const double COINMP_VERSIONDBL = 1.84;

const char* COINMP_NAME = "CoinMP";


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
	return COINMP_VERSIONDBL;
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
				POPTION   pOption;

				} COININFO, *PCOIN;
 


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
	pCoin->pOption = coinCreateOptionObject();

	coinSetProblemName(pCoin->pProblem, ProblemName);
	coinCopyOptionTable(pCoin->pOption, CbcGetOptionTable(), CbcGetOptionCount());

	return (HPROB)pCoin;
}


SOLVAPI int SOLVCALL CoinUnloadProblem(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;
	
	if (pCoin) {
		coinClearProblemObject(pCoin->pProblem);
		coinClearResultObject(pCoin->pResult);
		coinClearSolverObject(pCoin->pSolver);
		coinClearOptionObject(pCoin->pOption);
	}
	free(pCoin);
	pCoin = NULL;
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadMatrix(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, const char* RowType, 
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


SOLVAPI int SOLVCALL CoinLoadNames(HPROB hProb, char** ColNamesList, char** RowNamesList, const char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!coinStoreNamesList(pCoin->pProblem, ColNamesList, RowNamesList, ObjectName)) {
		return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}

void insertInArrayInt(int* matind, int position, int value, int lastPos) {
	for (int i = 0; i <= lastPos; i++) {
		if (i == position) {
			int tmp = matind[i];
			matind[i] = value;
			value = tmp;
			position = i + 1;
		}
	}
}

void insertInArrayDouble(double* matind, int position, double value, int lastPos) {
	for (int i = 0; i <= lastPos; i++) {
		if (i == position) {
			int tmp = matind[i];
			matind[i] = value;
			value = tmp;
			position = i + 1;
		}
	}
}

void addRangeValues(PCOIN pp, double* rowLower, double* rowUpper, int oldRow, char type)
{
	memcpy(pp->pProblem->RowLower, rowLower, sizeof(double) * oldRow);
	memcpy(pp->pProblem->RowUpper, rowUpper, sizeof(double) * oldRow);
	switch (type) {
		case 'L':
			pp->pProblem->RowLower[oldRow] = -COIN_DBL_MAX;
			pp->pProblem->RowUpper[oldRow] = pp->pProblem->RHSValues ? pp->pProblem->RHSValues[oldRow] : COIN_DBL_MAX;
			break;

		case 'G':
			pp->pProblem->RowLower[oldRow] = pp->pProblem->RHSValues ? pp->pProblem->RHSValues[oldRow] : -COIN_DBL_MAX;
			pp->pProblem->RowUpper[oldRow] = COIN_DBL_MAX;
			break;

		case 'E':
			pp->pProblem->RowLower[oldRow] = pp->pProblem->RHSValues ? pp->pProblem->RHSValues[oldRow] : 0.0;
			pp->pProblem->RowUpper[oldRow] = pp->pProblem->RHSValues ? pp->pProblem->RHSValues[oldRow] : 0.0;
			break;
	}
}

SOLVAPI int SOLVCALL CoinNullifyRow(HPROB hProb, int rowidx) {
	PCOIN pCoin = (PCOIN)hProb;
	if (rowidx >= pCoin->pProblem->RowCount) {
		return SOLV_CALL_FAILED;
	}
	for (int i = 0; i < pCoin->pProblem->NZCount; i++)
	{
		if (pCoin->pProblem->MatrixIndex[i] == rowidx) {
			pCoin->pProblem->MatrixValues[i] = 0;
		}
	}
	return SOLV_CALL_SUCCESS;
}

SOLVAPI int SOLVCALL CoinAddColumn(HPROB hProb, double coeff, double upperbound, double lowerbound) {
	PCOIN pCoin = (PCOIN)hProb;
	int oldColumns = pCoin->pProblem->ColCount;
	pCoin->pProblem->ColCount = oldColumns + 1;
	pCoin->pProblem->LowerBounds = (double*)realloc(pCoin->pProblem->LowerBounds, sizeof(double) * pCoin->pProblem->ColCount);
	pCoin->pProblem->UpperBounds = (double*)realloc(pCoin->pProblem->UpperBounds, sizeof(double) * pCoin->pProblem->ColCount);
	pCoin->pProblem->MatrixBegin = (int*)realloc(pCoin->pProblem->MatrixBegin, sizeof(int) * (pCoin->pProblem->ColCount + 1));
	pCoin->pProblem->MatrixCount = (int*)realloc(pCoin->pProblem->MatrixCount, sizeof(int) * pCoin->pProblem->ColCount);
	pCoin->pProblem->ObjectCoeffs = (double*)realloc(pCoin->pProblem->ObjectCoeffs, sizeof(double) * pCoin->pProblem->ColCount); 
	pCoin->pProblem->LowerBounds[oldColumns] = lowerbound;
	pCoin->pProblem->UpperBounds[oldColumns] = upperbound;
	pCoin->pProblem->MatrixBegin[oldColumns+1] = pCoin->pProblem->MatrixBegin[oldColumns];
	pCoin->pProblem->MatrixCount[oldColumns] = 0 ;
	pCoin->pProblem->ObjectCoeffs[oldColumns] = coeff;
	return SOLV_CALL_SUCCESS;
}

SOLVAPI HPROB SOLVCALL CoinAddRow(HPROB hProb,
	double* RowValues, double RHSValue, char RowType, char* RowName)
{	
	PCOIN pCoin = (PCOIN)hProb;
	if (!RowValues  || !RowType || !RowName) {
		return hProb;
	}
	// Rialloca array interni
	char* name = pCoin->pProblem->ProblemName;
	int newRowCount = pCoin->pProblem->RowCount;
	int colCount;
	
	memcpy(&colCount, &pCoin->pProblem->ColCount, sizeof(int));
	int newNZC;
	memcpy(&newNZC, &pCoin->pProblem->NZCount, sizeof(int));
	int range;
	memcpy(&range, &pCoin->pProblem->RangeCount, sizeof(int));
	int objs;
	memcpy(&objs, &pCoin->pProblem->ObjectSense, sizeof(int));
	int objc;
	memcpy(&objc, &pCoin->pProblem->ObjectConst, sizeof(int));
	double* objcoe = (double*)malloc(sizeof(double) * pCoin->pProblem->ColCount);
	memcpy(objcoe, pCoin->pProblem->ObjectCoeffs, sizeof(double) * pCoin->pProblem->ColCount);
	double* lob = (double*)malloc(sizeof(double) * pCoin->pProblem->ColCount);
	memcpy(lob, pCoin->pProblem->LowerBounds, sizeof(double) * pCoin->pProblem->ColCount);
	double* upb = (double*)malloc(sizeof(double) * pCoin->pProblem->ColCount);
	memcpy(upb, pCoin->pProblem->UpperBounds, sizeof(double) * pCoin->pProblem->ColCount);
	
	int* matbeg = pCoin->pProblem->MatrixBegin;
	int* matcou = pCoin->pProblem->MatrixCount;
	int nz = 0;
	for (int i = 0; i < colCount; i++) {
		if (RowValues[i] != 0) {
			nz = nz + 1;
		}
	}
	int oldNZ = newNZC;
	newNZC += nz;

	int* matind = (int*)malloc(sizeof(int) * newNZC);
	memcpy(matind, pCoin->pProblem->MatrixIndex, sizeof(int) * oldNZ);

	double* matval = (double*)malloc(sizeof(double) * newNZC);
	memcpy(matval, pCoin->pProblem->MatrixValues, sizeof(double) * oldNZ);


	int previous = 0;
	for (int i = 0; i < colCount; i++) {
		if (RowValues[i] != 0)
		{
			int position = previous + matcou[i];
			matcou[i] += 1;
			insertInArrayInt(matind,position, newRowCount, oldNZ);
			insertInArrayDouble(matval, position, RowValues[i], oldNZ);
			oldNZ++;
		}
		previous += matcou[i];
	}


	for (int i = 0; i < colCount + 1; i++) {
		if (i == 0) {
			matbeg[i] = 0;
		}
		else {
			matbeg[i] = matbeg[i - 1] + matcou[i - 1];
		}
	}
	double* rhsV = (double*)malloc(sizeof(double) * newRowCount + 1);
	memcpy(rhsV, pCoin->pProblem->RHSValues, sizeof(double) * newRowCount);
	rhsV[newRowCount] = RHSValue;
	
	newRowCount += 1;
	HPROB p1 = CoinCreateProblem(name);
	CoinLoadProblem(p1, colCount, newRowCount, newNZC, range, objs, objc, objcoe, lob, upb, pCoin->pProblem->RowType, rhsV, 0, matbeg, matcou, matind, matval, pCoin->pProblem->ColNamesList, pCoin->pProblem->RowNamesList, pCoin->pProblem->ObjectName);
	PCOIN pp = (PCOIN)p1;
	addRangeValues(pp, pCoin->pProblem->RowLower, pCoin->pProblem->RowUpper, pCoin->pProblem->RowCount, RowType);
	pp->pProblem->RowType[newRowCount - 1] = RowType;
	pp->pProblem->ObjectName = (char*)malloc(pCoin->pProblem->lenObjNameBuf * sizeof(char));
	memcpy(pp->pProblem->ObjectName, pCoin->pProblem->ObjectName, pCoin->pProblem->lenObjNameBuf * sizeof(char));
	CoinUnloadProblem(hProb);
	return 	p1;
}

SOLVAPI int SOLVCALL CoinLoadNamesBuf(HPROB hProb, const char* ColNamesBuf, const char* RowNamesBuf, const char* ObjectName)
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
				double* LowerBounds, double* UpperBounds, const char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char** ColNamesList, char** RowNamesList, const char* ObjectName)
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
				double* LowerBounds, double* UpperBounds, const char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				const char* ColNamesBuf, const char* RowNamesBuf, const char* ObjectName)
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


SOLVAPI int SOLVCALL CoinLoadInteger(HPROB hProb, const char* ColType)
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



SOLVAPI int SOLVCALL CoinRegisterMsgLogCallback(HPROB hProb, COIN_MSGLOG_CB MsgLogCB, void *UserParam)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MsgLogCB = MsgLogCB;
	pCoin->pSolver->MsgLogParam = UserParam;
	return SOLV_CALL_SUCCESS;
}

SOLVAPI int SOLVCALL CoinRegisterLPIterCallback(HPROB hProb, COIN_LPITER_CB LPIterCB, void *UserParam)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->LPIterCB = LPIterCB;
	pCoin->pSolver->LPIterParam = UserParam;
	return SOLV_CALL_SUCCESS;
}

SOLVAPI int SOLVCALL CoinRegisterMipNodeCallback(HPROB hProb, COIN_MIPNODE_CB MipNodeCB, void *UserParam)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MipNodeCB = MipNodeCB;
	pCoin->pSolver->MipNodeParam = UserParam;
	return SOLV_CALL_SUCCESS;
}


// Depreciated, use CoinRegisterMsgLogCallback instead
SOLVAPI int SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MsgLogCallback = MsgLogCallback;
	return SOLV_CALL_SUCCESS;
}

// Depreciated, use CoinRegisterLPIterCallback instead
SOLVAPI int SOLVCALL CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->IterCallback = IterCallback;
	return SOLV_CALL_SUCCESS;
}

// Depreciated, use CoinRegisterMipNodeCallback instead
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

	return CbcOptimizeProblem(pCoin->pProblem, pCoin->pResult, pCoin->pSolver, pCoin->pOption, Method);
}



/************************************************************************/
/*  Solution status                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionStatus(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionStatus;
}


  

SOLVAPI const char* SOLVCALL CoinGetSolutionText(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionText;
}


SOLVAPI int SOLVCALL CoinGetSolutionTextBuf(HPROB hProb, char* SolutionText, int buflen)
{
	strncpy(SolutionText, CoinGetSolutionText(hProb), buflen-1);
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



SOLVAPI int SOLVCALL CoinGetSolutionBasis(HPROB hProb, int* ColStatus, int* RowStatus)
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

	return coinGetOptionCount(pCoin->pOption);
}

SOLVAPI int    SOLVCALL CoinLocateOptionID(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinLocateOptionID(pCoin->pOption, OptionID);
}


SOLVAPI int    SOLVCALL CoinLocateOptionName(HPROB hProb, const char* OptionName)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinLocateOptionName(pCoin->pOption, OptionName);
}



SOLVAPI int SOLVCALL CoinGetOptionID(HPROB hProb, int OptionNr)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionID(pCoin->pOption, OptionNr);
}


SOLVAPI int SOLVCALL CoinGetOptionInfo(HPROB hProb, int OptionNr, int* OptionID, int* GroupType, int* OptionType)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pOption, OptionNr);
	if (OptionID)   *OptionID = optionID;
	if (OptionType) *OptionType = coinGetOptionType(pCoin->pOption, optionID);
	if (GroupType)  *GroupType = coinGetOptionGroup(pCoin->pOption, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int* MinValue, int* MaxValue)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pOption, OptionNr);
	if (MinValue)   *MinValue = coinGetIntOptionMinValue(pCoin->pOption, optionID);
	if (MaxValue)   *MaxValue = coinGetIntOptionMaxValue(pCoin->pOption, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double* MinValue, double* MaxValue)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pOption, OptionNr);
	if (MinValue)   *MinValue = coinGetRealOptionMinValue(pCoin->pOption, optionID);
	if (MaxValue)   *MaxValue = coinGetRealOptionMaxValue(pCoin->pOption, optionID);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetOptionNamesBuf(HPROB hProb, int OptionNr, char* OptionName, 
												char* ShortName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;
	int optionID;

	optionID = coinGetOptionID(pCoin->pOption, OptionNr);
	if (OptionName) coinGetOptionNameBuf(pCoin->pOption, optionID, OptionName, buflen);
	if (ShortName) coinGetOptionShortNameBuf(pCoin->pOption, optionID, ShortName, buflen);
	return SOLV_CALL_SUCCESS;
}





SOLVAPI int SOLVCALL CoinGetOptionGroup(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionGroup(pCoin->pOption, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionType(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionType(pCoin->pOption, OptionID);
}




SOLVAPI int    SOLVCALL CoinGetIntOptionDefaultValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionDefaultValue(pCoin->pOption, OptionID);
}


SOLVAPI int    SOLVCALL CoinGetIntOptionMinValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionMinValue(pCoin->pOption, OptionID);
}


SOLVAPI int    SOLVCALL CoinGetIntOptionMaxValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOptionMaxValue(pCoin->pOption, OptionID);
}



SOLVAPI double SOLVCALL CoinGetRealOptionDefaultValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionDefaultValue(pCoin->pOption, OptionID);
}


SOLVAPI double SOLVCALL CoinGetRealOptionMinValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionMinValue(pCoin->pOption, OptionID);
}


SOLVAPI double SOLVCALL CoinGetRealOptionMaxValue(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOptionMaxValue(pCoin->pOption, OptionID);
}




SOLVAPI const char* SOLVCALL CoinGetOptionName(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionName(pCoin->pOption, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionNameBuf(HPROB hProb, int OptionID, char* OptionName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionNameBuf(pCoin->pOption, OptionID, OptionName, buflen);
}


SOLVAPI const char* SOLVCALL CoinGetOptionShortName(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionShortName(pCoin->pOption, OptionID);
}


SOLVAPI int SOLVCALL CoinGetOptionShortNameBuf(HPROB hProb, int OptionID, char* ShortName, int buflen)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionShortNameBuf(pCoin->pOption, OptionID, ShortName, buflen);
}




SOLVAPI int SOLVCALL CoinGetOptionChanged(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetOptionChanged(pCoin->pOption, OptionID);
}


SOLVAPI int SOLVCALL CoinGetIntOption(HPROB hProb, int OptionID)
{   
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetIntOption(pCoin->pOption, OptionID);
}
  	


SOLVAPI int SOLVCALL CoinSetIntOption(HPROB hProb, int OptionID, int IntValue)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinSetIntOption(pCoin->pOption, OptionID, IntValue);
}



SOLVAPI double SOLVCALL CoinGetRealOption(HPROB hProb, int OptionID)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinGetRealOption(pCoin->pOption, OptionID);
}



SOLVAPI int SOLVCALL CoinSetRealOption(HPROB hProb, int OptionID, double RealValue)
{
	PCOIN pCoin = (PCOIN)hProb;

	return coinSetRealOption(pCoin->pOption, OptionID, RealValue);
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

