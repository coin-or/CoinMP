
/*  CoinSolver.c  */

#include <stdlib.h>
#include <string.h>

#include "CoinSolver.h"


PSOLVER coinCreateSolverObject()
{
	PSOLVER pSolver;

	pSolver = (PSOLVER)malloc(sizeof(SOLVERINFO));
	memset(pSolver, 0, sizeof(SOLVERINFO));

	strcpy(pSolver->LogFilename, "");

	pSolver->MsgLogCB	= NULL;
	pSolver->LPIterCB	= NULL;
	pSolver->MipNodeCB	= NULL;

	pSolver->MsgLogParam = NULL;
	pSolver->LPIterParam = NULL;
	pSolver->MipNodeParam = NULL;

	pSolver->MsgLogCallback = NULL;
	pSolver->IterCallback = NULL;
	pSolver->MipNodeCallback = NULL;

	return pSolver;
}


void coinClearSolverObject(PSOLVER pSolver)
{
	if (!pSolver) {
		return;
	}
	free(pSolver);
}


void coinSetLogFilename(PSOLVER pSolver, const char* LogFilename)
{
	if (LogFilename) {
		strcpy(pSolver->LogFilename, LogFilename);
	}
}


const char* coinGetLogFilename(PSOLVER pSolver)
{
	return pSolver->LogFilename;
}

