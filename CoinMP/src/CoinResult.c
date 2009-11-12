
  /* CoinResult.h */

// $Id$

#include <stdlib.h>
#include <string.h>

#include "CoinResult.h"


PRESULT coinCreateResultObject(void)
{
	PRESULT pResult;

	pResult = (PRESULT)malloc(sizeof(RESULTINFO));
	memset(pResult, 0, sizeof(RESULTINFO));

	pResult->SolutionStatus = 0;
	strcpy(pResult->SolutionText, "");

	pResult->ObjectValue  = 0.0;
	pResult->MipBestBound = 0.0;
	pResult->IterCount    = 0;
	pResult->MipNodeCount = 0;

	pResult->ColActivity = NULL;
	pResult->ReducedCost = NULL;
	pResult->SlackValues = NULL;
	pResult->ShadowPrice = NULL;

	pResult->ObjLoRange = NULL;
	pResult->ObjUpRange = NULL;
	pResult->RhsLoRange = NULL;
	pResult->RhsUpRange = NULL;

	pResult->ColStatus = NULL;
	pResult->RowStatus = NULL;

	return pResult;
}


void coinClearResultObject(PRESULT pResult)
{
	if (!pResult) {
		return;
	}
	if (pResult->ColActivity)   free(pResult->ColActivity);
	if (pResult->ReducedCost)   free(pResult->ReducedCost);
	if (pResult->SlackValues)   free(pResult->SlackValues);
	if (pResult->ShadowPrice)   free(pResult->ShadowPrice);

	if (pResult->ObjLoRange)    free(pResult->ObjLoRange);
	if (pResult->ObjUpRange)    free(pResult->ObjUpRange);
	if (pResult->ObjUpRange)    free(pResult->ObjUpRange);
	if (pResult->RhsUpRange)    free(pResult->RhsUpRange);

	if (pResult->ColStatus)     free(pResult->ColStatus);
	if (pResult->ColStatus)     free(pResult->ColStatus);
	free(pResult);
}


