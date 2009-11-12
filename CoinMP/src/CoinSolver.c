
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

	pSolver->OptionCount = 0;
	pSolver->OptionCopy = 0;
	pSolver->OptionTable = NULL;

	pSolver->MsgLogCallback = NULL;
	pSolver->IterCallback = NULL;
	pSolver->MipNodeCallback = NULL;

	return pSolver;
}


void coinClearSolverObject(PSOLVER pSolver)
{
	if (!pSolver) {
		if (pSolver->OptionCopy && pSolver->OptionTable) {
			free(pSolver->OptionTable);
		}
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



void coinCopyOptionTable(PSOLVER pSolver, OPTIONTABLE OptionTable, int OptionCount)
{
	if (!pSolver) {
		return;
	}
	if (pSolver->OptionCount > 0) {
		pSolver->OptionCount = OptionCount;
		pSolver->OptionTable = (PSOLVOPT)malloc(OptionCount * sizeof(SOLVOPTINFO));
		memcpy(pSolver->OptionTable, OptionTable, OptionCount * sizeof(SOLVOPTINFO));
		pSolver->OptionCopy = 1;
	}
}


void coinSetOptionTable(PSOLVER pSolver, OPTIONTABLE OptionTable, int OptionCount)
{
	if (!pSolver) {
		return;
	}
	pSolver->OptionTable = OptionTable;
	pSolver->OptionCount = OptionCount;
}


int coinGetOptionCount(PSOLVER pSolver)
{
	return pSolver->OptionCount;
}


int coinGetOptionID(PSOLVER pSolver, int OptionNr)
{
	return pSolver->OptionTable[OptionNr].OptionID;
}


int coinLocateOptionID(PSOLVER pSolver, int OptionID)
{
	int i;

	for (i = 0; i < pSolver->OptionCount; i++) {
		if (OptionID == pSolver->OptionTable[i].OptionID) {
			return i;
		}
	}
	return -1;
}


int coinGetOptionType(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return OPT_NONE;
	}
	return pSolver->OptionTable[OptionNr].OptionType;
}


int coinGetOptionGroup(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return GRP_NONE;
	}
	return pSolver->OptionTable[OptionNr].GroupType;
}


#ifdef _WIN32
 #define STRING_COMPARE_NOCASE(s1, s2)  strcmpi(s1, s2)
#else
 #define STRING_COMPARE_NOCASE(s1, s2)  strcasecmp(s1, s2)
#endif

int coinLocateOptionName(PSOLVER pSolver, char* OptionName)
{
	int i;

	for (i = 0; i < pSolver->OptionCount; i++) {
		if (STRING_COMPARE_NOCASE(OptionName, pSolver->OptionTable[i].OptionName) != 0) {
			return pSolver->OptionTable[i].OptionID;
		}
		if (STRING_COMPARE_NOCASE(OptionName, pSolver->OptionTable[i].ShortName) != 0) {
			return pSolver->OptionTable[i].OptionID;
		}
	}
	return -1;
}


char* coinGetOptionName(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return GRP_NONE;
	}
	return pSolver->OptionTable[OptionNr].OptionName;
}


int coinGetOptionNameBuf(PSOLVER pSolver, int OptionID, char* OptionName, int buflen)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		if (OptionName) {
			strcpy(OptionName, "");
		}
		return -1;
	}
	if (OptionName) {
		strncpy(OptionName, pSolver->OptionTable[OptionNr].OptionName, buflen-1);
		OptionName[buflen-1] = '\0';
	}
	return 0;
}

char* coinGetOptionShortName(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return GRP_NONE;
	}
	return pSolver->OptionTable[OptionNr].ShortName;
}


int coinGetOptionShortNameBuf(PSOLVER pSolver, int OptionID, char* ShortName, int buflen)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		if (ShortName) {
			strcpy(ShortName, "");
		}
		return -1;
	}
	if (ShortName) {
		strncpy(ShortName, pSolver->OptionTable[OptionNr].ShortName, buflen-1);
		ShortName[buflen-1] = '\0';
	}
	return 0;
}


int coinGetIntOptionDefaultValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pSolver->OptionTable[OptionNr].DefaultValue;
}



int coinGetIntOptionMinValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pSolver->OptionTable[OptionNr].MinValue;
}



int coinGetIntOptionMaxValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pSolver->OptionTable[OptionNr].MaxValue;
}




double coinGetRealOptionDefaultValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pSolver->OptionTable[OptionNr].DefaultValue;
}



double coinGetRealOptionMinValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pSolver->OptionTable[OptionNr].MinValue;
}



double coinGetRealOptionMaxValue(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pSolver->OptionTable[OptionNr].MaxValue;
}

int coinGetOptionChanged(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return pSolver->OptionTable[OptionNr].changed;
}


int coinGetIntOption(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pSolver->OptionTable[OptionNr].CurrentValue;
}

int coinSetIntOption(PSOLVER pSolver, int OptionID, int IntValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return -1;
	}
	pSolver->OptionTable[OptionNr].CurrentValue = IntValue;
	pSolver->OptionTable[OptionNr].changed = 1;
	return 0;
}


double coinGetRealOption(PSOLVER pSolver, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pSolver->OptionTable[OptionNr].CurrentValue;
}


int coinSetRealOption(PSOLVER pSolver, int OptionID, double RealValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pSolver, OptionID);
	if (OptionNr == -1) {
		return -1;
	}
	pSolver->OptionTable[OptionNr].CurrentValue = RealValue;
	pSolver->OptionTable[OptionNr].changed = 1;
	return 0;
}

