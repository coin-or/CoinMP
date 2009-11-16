
/*  CoinProblem.c  */


#include <stdlib.h>
#include <string.h>

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


int coinComputeRowLowerUpper(PPROBLEM pProblem, double CoinDblMax)
{
	int i;
	double RangeABS, RangeValue;

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


int coinSetupNamesList(char** NamesList, char* NamesBuf, int Count)
{
	int i,k,len;

	if (!NamesList || !NamesBuf || (Count == 0)) {
		return 0;
	}
	k = 0;
	for (i = 0; i < Count; i++) {
		NamesList[i] = &NamesBuf[k];
		len = (int)strlen(NamesList[i]) + 1;
		k += len;
	}
	return 1;
}

