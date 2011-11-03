
/*  CoinProblem.c  */


#include <stdlib.h>
#include <string.h>
#include <float.h>   /* DBL_MAX */

#include "CoinProblem.h"


PPROBLEM coinCreateProblemObject()
{
	PPROBLEM pProblem;

	pProblem = (PPROBLEM)malloc(sizeof(PROBLEMINFO));
	memset(pProblem, 0, sizeof(PROBLEMINFO));

	pProblem->ColCount    = 0;
	pProblem->RowCount    = 0;
	pProblem->NZCount     = 0;
	pProblem->RangeCount  = 0;
	pProblem->ObjectSense = 0;
	pProblem->ObjectConst = 0.0;

	pProblem->lenColNamesBuf   = 0;
	pProblem->lenRowNamesBuf   = 0;
	pProblem->lenObjNameBuf = 0;

	pProblem->ObjectCoeffs = NULL;
	pProblem->RHSValues    = NULL;
	pProblem->RangeValues  = NULL;
	pProblem->RowType      = NULL;
	pProblem->MatrixBegin  = NULL;
	pProblem->MatrixCount  = NULL;
	pProblem->MatrixIndex  = NULL; 
	pProblem->MatrixValues = NULL;
	pProblem->LowerBounds  = NULL;
	pProblem->UpperBounds  = NULL;
	pProblem->ColNamesBuf  = NULL;
	pProblem->RowNamesBuf  = NULL;
	pProblem->ColNamesList = NULL;
	pProblem->RowNamesList = NULL;
	pProblem->ObjectName   = NULL;

	pProblem->InitValues   = NULL;

	pProblem->RowLower     = NULL;
	pProblem->RowUpper     = NULL;

	pProblem->ColType      = NULL;

	pProblem->SolveAsMIP   = 0;
	pProblem->IntCount     = 0;
	pProblem->BinCount     = 0;
	pProblem->numInts      = 0;
	pProblem->IsInt        = NULL;

	pProblem->PriorCount   = 0;
	pProblem->PriorIndex   = NULL;
	pProblem->PriorValues  = NULL;
	pProblem->PriorBranch  = NULL;

	pProblem->SosCount     = 0;
	pProblem->SosNZCount   = 0;
	pProblem->SosType      = NULL;
	pProblem->SosPrior     = NULL;
	pProblem->SosBegin     = NULL;
	pProblem->SosIndex     = NULL;
	pProblem->SosRef       = NULL;

	pProblem->SemiCount    = 0;
	pProblem->SemiIndex    = NULL;
	pProblem->SemiIndex	   = NULL;

	return pProblem;
}



void coinClearProblemObject(PPROBLEM pProblem)
{
	if (!pProblem) {
		return;
	}
	if (pProblem->ObjectCoeffs) free(pProblem->ObjectCoeffs);
	if (pProblem->RHSValues)    free(pProblem->RHSValues);
	if (pProblem->RangeValues)  free(pProblem->RangeValues);
	if (pProblem->RowType)      free(pProblem->RowType);
	if (pProblem->MatrixBegin)  free(pProblem->MatrixBegin);
	if (pProblem->MatrixCount)  free(pProblem->MatrixCount);
	if (pProblem->MatrixIndex)  free(pProblem->MatrixIndex); 
	if (pProblem->MatrixValues) free(pProblem->MatrixValues);
	if (pProblem->LowerBounds)  free(pProblem->LowerBounds);
	if (pProblem->UpperBounds)  free(pProblem->UpperBounds);

	if (pProblem->ColNamesBuf)  free(pProblem->ColNamesBuf);
	if (pProblem->RowNamesBuf)  free(pProblem->RowNamesBuf);
	if (pProblem->ColNamesList) free(pProblem->ColNamesList);
	if (pProblem->RowNamesList) free(pProblem->RowNamesList);
	if (pProblem->ObjectName)   free(pProblem->ObjectName);

	if (pProblem->InitValues)   free(pProblem->InitValues);

	if (pProblem->RowLower)		free(pProblem->RowLower);
	if (pProblem->RowUpper)		free(pProblem->RowUpper);

	if (pProblem->ColType)      free(pProblem->ColType);

	if (pProblem->PriorIndex)   free(pProblem->PriorIndex);
	if (pProblem->PriorValues)  free(pProblem->PriorValues);
	if (pProblem->PriorBranch)  free(pProblem->PriorBranch);

	if (pProblem->SemiIndex)	free(pProblem->SemiIndex);

	if (pProblem->SosType)		free(pProblem->SosType);
	if (pProblem->SosPrior)		free(pProblem->SosPrior);
	if (pProblem->SosBegin)		free(pProblem->SosBegin);
	if (pProblem->SosIndex)		free(pProblem->SosIndex);
	if (pProblem->SosRef)		free(pProblem->SosRef);

	free(pProblem);
}



void coinSetProblemName(PPROBLEM pProblem, const char *ProblemName)
{
	size_t len;

	len = strlen(ProblemName);
	if (len >= sizeof(pProblem->ProblemName)) {
		len = sizeof(pProblem->ProblemName) - 1;
	}
	strncpy(pProblem->ProblemName, ProblemName, len);
	pProblem->ProblemName[len] = '\0';
}


int coinStoreMatrix(PPROBLEM pProblem,	int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, double* LowerBounds, 
				double* UpperBounds, const char* RowType, double* RHSValues, double* RangeValues, 
				int* MatrixBegin, int* MatrixCount, int* MatrixIndex, double* MatrixValues)
{
	if (ColCount == 0) {
		return 0;
	}
	pProblem->ColCount = ColCount;
	pProblem->RowCount = RowCount;
	pProblem->NZCount = NZCount;
	pProblem->RangeCount = RangeCount;
	pProblem->ObjectSense = ObjectSense;
	pProblem->ObjectConst = ObjectConst;

	if (ObjectCoeffs) pProblem->ObjectCoeffs = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (LowerBounds)  pProblem->LowerBounds  = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (UpperBounds)  pProblem->UpperBounds  = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (RowType)      pProblem->RowType      = (char*)   malloc(pProblem->RowCount     * sizeof(char));
	if (RHSValues)    pProblem->RHSValues    = (double*) malloc(pProblem->RowCount     * sizeof(double));
	if (RangeValues)  pProblem->RangeValues  = (double*) malloc(pProblem->RowCount     * sizeof(double));
	if (MatrixBegin)  pProblem->MatrixBegin  = (int*)    malloc((pProblem->ColCount+1) * sizeof(int));
	if (MatrixCount)  pProblem->MatrixCount  = (int*)    malloc(pProblem->ColCount     * sizeof(int));
	if (MatrixIndex)  pProblem->MatrixIndex  = (int*)    malloc(pProblem->NZCount      * sizeof(int)); 
	if (MatrixValues) pProblem->MatrixValues = (double*) malloc(pProblem->NZCount      * sizeof(double));

	if ((ObjectCoeffs && !pProblem->ObjectCoeffs) || 
		(LowerBounds  && !pProblem->LowerBounds) ||  
		(UpperBounds  && !pProblem->UpperBounds) || 
		(RowType      && !pProblem->RowType)     || 
		(RHSValues    && !pProblem->RHSValues)   ||  
		(RangeValues  && !pProblem->RangeValues) || 
		(MatrixBegin  && !pProblem->MatrixBegin) || 
		(MatrixCount  && !pProblem->MatrixCount) ||  
		(MatrixIndex  && !pProblem->MatrixIndex) || 
		(MatrixValues && !pProblem->MatrixValues)) {
		return 0;
	}
	if (ObjectCoeffs) memcpy(pProblem->ObjectCoeffs, ObjectCoeffs, pProblem->ColCount     * sizeof(double));
	if (LowerBounds)  memcpy(pProblem->LowerBounds,  LowerBounds,  pProblem->ColCount     * sizeof(double));
	if (UpperBounds)  memcpy(pProblem->UpperBounds,  UpperBounds,  pProblem->ColCount     * sizeof(double));
	if (RowType)      memcpy(pProblem->RowType,      RowType,      pProblem->RowCount     * sizeof(char));
	if (RHSValues)    memcpy(pProblem->RHSValues,    RHSValues,    pProblem->RowCount     * sizeof(double));
	if (RangeValues)  memcpy(pProblem->RangeValues,  RangeValues,  pProblem->RowCount     * sizeof(double));
	if (MatrixBegin)  memcpy(pProblem->MatrixBegin,  MatrixBegin,  (pProblem->ColCount+1) * sizeof(int));
	if (MatrixCount)  memcpy(pProblem->MatrixCount,  MatrixCount,  pProblem->ColCount     * sizeof(int));
	if (MatrixIndex)  memcpy(pProblem->MatrixIndex,  MatrixIndex,  pProblem->NZCount      * sizeof(int));
	if (MatrixValues) memcpy(pProblem->MatrixValues, MatrixValues, pProblem->NZCount      * sizeof(double));

	if (!coinComputeRowLowerUpper(pProblem, DBL_MAX)) {
		return 0;
	}
	return 1;
}


int coinStoreNamesList(PPROBLEM pProblem, char** ColNamesList, char** RowNamesList, const char* ObjectName)
{
	if (ObjectName) {
		pProblem->lenObjNameBuf  = (int)strlen(ObjectName) + 1;
	    pProblem->ObjectName   = (char*)   malloc(pProblem->lenObjNameBuf  * sizeof(char));
		if (!pProblem->ObjectName) {
			return 0;
		}
		memcpy(pProblem->ObjectName,   ObjectName,   pProblem->lenObjNameBuf  * sizeof(char));
	}
	if (ColNamesList) {
		pProblem->lenColNamesBuf = coinGetLenNameListBuf(ColNamesList, pProblem->ColCount);
		pProblem->ColNamesList = (char**)  malloc(pProblem->ColCount     * sizeof(char* ));
		pProblem->ColNamesBuf  = (char*)   malloc(pProblem->lenColNamesBuf * sizeof(char));
		if (!pProblem->ColNamesList && !pProblem->ColNamesBuf) {
			return 0;
		}
		coinCopyNamesList(pProblem->ColNamesList, pProblem->ColNamesBuf, ColNamesList, pProblem->ColCount);
	}
	if (RowNamesList) {
		pProblem->lenRowNamesBuf = coinGetLenNameListBuf(RowNamesList, pProblem->RowCount);
		pProblem->RowNamesList = (char**)  malloc(pProblem->RowCount     * sizeof(char* ));
		pProblem->RowNamesBuf  = (char*)   malloc(pProblem->lenRowNamesBuf * sizeof(char));
		if (!pProblem->RowNamesList && !pProblem->RowNamesBuf) {
			return 0;
		}
		coinCopyNamesList(pProblem->RowNamesList, pProblem->RowNamesBuf, RowNamesList, pProblem->RowCount);
	}
	return 1;
}


int coinStoreNamesBuf(PPROBLEM pProblem, const char* ColNamesBuf, const char* RowNamesBuf, const char* ObjectName)
{
	char** ColNamesList;
	char** RowNamesList;
	int result;

	ColNamesList = (char**)malloc(pProblem->ColCount * sizeof(char*));
	RowNamesList = (char**)malloc(pProblem->RowCount * sizeof(char*));
	if (!ColNamesList && !RowNamesList) {
		return 0;
	}
	coinSetupNamesList(ColNamesList, ColNamesBuf, pProblem->ColCount);
	coinSetupNamesList(RowNamesList, RowNamesBuf, pProblem->RowCount);
	result = coinStoreNamesList(pProblem, ColNamesList, RowNamesList, ObjectName);
	if (ColNamesList) free(ColNamesList);
	if (RowNamesList) free(RowNamesList);
	return result;

}

int coinStoreInitValues(PPROBLEM pProblem, double* InitValues)
{
	if (!InitValues) {
		return 0;
	}	
	pProblem->InitValues = (double* ) malloc(pProblem->ColCount * sizeof(double));
	if (!pProblem->InitValues) {
		return 0;
	}
	memcpy(pProblem->InitValues, InitValues, pProblem->ColCount * sizeof(double));
	return 1;
}


int coinStoreInteger(PPROBLEM pProblem, const char* ColType)
{
	int numInts;

	if (pProblem->ColCount == 0) {
		return 0;
	}
	if (!ColType) {
		return 0;
	}
	pProblem->ColType = (char* )malloc(pProblem->ColCount * sizeof(char));
	if (!pProblem->ColType) {
		return 0;
	}
	memcpy(pProblem->ColType, ColType, pProblem->ColCount * sizeof(char));
	numInts = coinComputeIntVariables(pProblem);
	pProblem->SolveAsMIP = (numInts > 0);
	return 1;
}


int coinStorePriority(PPROBLEM pProblem, int PriorCount, int* PriorIndex, 
				int* PriorValues, int* PriorBranch)
{
	if (PriorCount == 0) {
		return 0;
	}
	pProblem->PriorCount = PriorCount;
	if (PriorIndex)  pProblem->PriorIndex  = (int* )malloc(PriorCount * sizeof(int));
	if (PriorValues) pProblem->PriorValues = (int* )malloc(PriorCount * sizeof(int));
	if (PriorBranch) pProblem->PriorBranch = (int* )malloc(PriorCount * sizeof(int));

	if ((PriorIndex && !pProblem->PriorIndex) || 
		(PriorValues  && !pProblem->PriorValues) ||  
		(PriorBranch  && !pProblem->PriorBranch)) {
		return 0;
	}
	if (PriorIndex)  memcpy(pProblem->PriorIndex,  PriorIndex,  PriorCount * sizeof(int));
	if (PriorValues) memcpy(pProblem->PriorValues, PriorValues, PriorCount * sizeof(int));
	if (PriorBranch) memcpy(pProblem->PriorBranch, PriorBranch, PriorCount * sizeof(int));
	return 0;
}


int coinStoreSos(PPROBLEM pProblem, int SosCount, int SosNZCount, 
				int* SosType, int* SosPrior, int* SosBegin, 
				int* SosIndex, double* SosRef)
{
	if ((SosCount == 0) || (SosNZCount == 0)) {
		return 0;
	}
	pProblem->SosCount = SosCount;
	pProblem->SosNZCount = SosNZCount;

	if (SosType)  pProblem->SosType  = (int* )malloc(SosCount     * sizeof(int));
	if (SosPrior) pProblem->SosPrior = (int* )malloc(SosCount     * sizeof(int));
	if (SosBegin) pProblem->SosBegin = (int* )malloc((SosCount+1) * sizeof(int));
	if (SosIndex) pProblem->SosIndex = (int* )malloc(SosNZCount   * sizeof(int));
	if (SosRef)   pProblem->SosRef   = (double* )malloc(SosNZCount* sizeof(double));

	if ((SosType && !pProblem->SosType) || 
		(SosPrior  && !pProblem->SosPrior) ||  
		(SosBegin  && !pProblem->SosBegin) ||  
		(SosIndex  && !pProblem->SosIndex) ||  
		(SosRef  && !pProblem->SosRef)) {
		return 0;
	}
	if (SosType)  memcpy(pProblem->SosType,  SosType,  SosCount     * sizeof(int));
	if (SosPrior) memcpy(pProblem->SosPrior, SosPrior, SosCount     * sizeof(int));
	if (SosBegin) memcpy(pProblem->SosBegin, SosBegin, (SosCount+1) * sizeof(int));
	if (SosIndex) memcpy(pProblem->SosIndex, SosIndex, SosNZCount   * sizeof(int));
	if (SosRef)   memcpy(pProblem->SosRef,   SosRef,   SosNZCount   * sizeof(double));

	pProblem->SolveAsMIP = 1;
	return 1;
}


int coinStoreSemiCont(PPROBLEM pProblem, int SemiCount, int* SemiIndex)
{
	if (SemiCount == 0) {
		return 0;
	}
	if (!SemiIndex) {
		return 0;
	}
	pProblem->SemiCount = SemiCount;
	pProblem->SemiIndex = (int* )malloc(pProblem->SemiCount * sizeof(int));
	if (!pProblem->SemiIndex) {
		return 0;
	}
	memcpy(pProblem->SemiIndex, SemiIndex, pProblem->SemiCount * sizeof(int));

	pProblem->SolveAsMIP = 1;
	return 1;
}


int coinComputeRowLowerUpper(PPROBLEM pProblem, double CoinDblMax)
{
	int i;
	double RangeABS, RangeValue;

	if (pProblem->RowCount == 0) {
		return 0;
	}
	pProblem->RowLower = (double* )malloc(pProblem->RowCount*sizeof(double));
	pProblem->RowUpper = (double* )malloc(pProblem->RowCount*sizeof(double));
	if (!pProblem->RowLower || !pProblem->RowUpper) {
		return 0;
	}
	if (!pProblem->RowType) {
		/* if NO RowType, we treat RHSValues as RowLower and RangeValues as RowUpper */
		for (i = 0; i < pProblem->RowCount; i++) {
			pProblem->RowLower[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : -CoinDblMax;
			pProblem->RowUpper[i] = pProblem->RangeValues ? pProblem->RangeValues[i] : CoinDblMax;
		}
		return 1;
	}
	for (i = 0; i < pProblem->RowCount; i++ ){
		switch (pProblem->RowType[i]) {
			case 'L':
				pProblem->RowLower[i] = -CoinDblMax;
				pProblem->RowUpper[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : CoinDblMax;
				break;

			case 'G':
				pProblem->RowLower[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : -CoinDblMax;
				pProblem->RowUpper[i] = CoinDblMax;
				break;

			case 'E':
				pProblem->RowLower[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : 0.0;
				pProblem->RowUpper[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : 0.0;
				break;

			case 'R':
				RangeValue = pProblem->RangeValues ? pProblem->RangeValues[i] : 0.0;
				RangeABS = (RangeValue >= 0.0) ? RangeValue : -RangeValue;
				pProblem->RowLower[i] = (pProblem->RHSValues ? pProblem->RHSValues[i] : -CoinDblMax) - RangeABS;
				pProblem->RowUpper[i] = pProblem->RHSValues ? pProblem->RHSValues[i] : CoinDblMax;
				break;

			case 'N':
				pProblem->RowLower[i] = -CoinDblMax;
				pProblem->RowUpper[i] = CoinDblMax;
				break;

			default:
				return 0;
		}
	}
	return 1;
}


int coinComputeIntVariables(PPROBLEM pProblem)
{
	int i;

	if (pProblem->ColCount == 0) {
		return 0;
	}
	pProblem->IsInt = (char* )malloc(pProblem->ColCount * sizeof(char));
	if (!pProblem->IsInt) {
		return 0;
	}
	for (i = 0; i < pProblem->ColCount; i++ ) {
		switch (pProblem->ColType[i]) {
			case 'B': 
				pProblem->BinCount++;
				pProblem->IsInt[i] = 1;
				break;

			case 'I': 
				pProblem->IntCount++;
				pProblem->IsInt[i] = 1;
				break;

			default:
				pProblem->IsInt[i] = 0;
				break;
		}
	}
	pProblem->numInts = pProblem->IntCount + pProblem->BinCount;
	return pProblem->numInts;
}


int coinGetLenNameBuf(const char* NameBuf, int Count)
{
	int i, len;
	int lenBuf;
	const char* pName;

	lenBuf = 0;
	pName = &NameBuf[0];
	for (i = 0; i < Count; i++) {
		len = (int)strlen(pName) + 1;
		lenBuf += len;
		pName = pName + len;
	}
	return lenBuf;
}


int coinGetLenNameListBuf(char** NameList, int Count)
{
	int i, len;
	int lenBuf;

	lenBuf = 0;
	for (i = 0; i < Count; i++) {
		len = (int)strlen(NameList[i]) + 1;
		lenBuf += len;
	}
	return lenBuf;
}


int coinCopyNamesList(char** NamesList, char* NamesBuf, char** argNamesList, int Count)
{
	int i,k,len;

	if (!NamesList || !argNamesList || !NamesBuf || (Count == 0)) {
		return 0;
	}
	k = 0;
	for (i = 0; i < Count; i++) {
		NamesList[i] = &NamesBuf[k];
		strcpy(NamesList[i], argNamesList[i]);
		len = (int)strlen(NamesList[i]) + 1;
		k += len;
	}
	return 1;
}


int coinSetupNamesList(char** NamesList, const char* NamesBuf, int Count)
{
	int i,k,len;

	if (!NamesList || !NamesBuf || (Count == 0)) {
		return 0;
	}
	k = 0;
	for (i = 0; i < Count; i++) {
		NamesList[i] = (char*)&NamesBuf[k];
		len = (int)strlen(NamesList[i]) + 1;
		k += len;
	}
	return 1;
}

