
/*  CoinOption.c  */

#include <stdlib.h>
#include <string.h>

#include "CoinOption.h"


#ifdef _WIN32
 #define STRING_COMPARE_NOCASE(s1, s2)  strcmpi(s1, s2)
#else
 #define STRING_COMPARE_NOCASE(s1, s2)  strcasecmp(s1, s2)
#endif


POPTION coinCreateOptionObject()
{
	POPTION pOption;

	pOption = (POPTION)malloc(sizeof(OPTIONINFO));
	memset(pOption, 0, sizeof(OPTIONINFO));

	pOption->OptionCount = 0;
	pOption->OptionCopy = 0;
	pOption->OptionTable = NULL;

	return pOption;
}


void coinClearOptionObject(POPTION pOption)
{
	if (!pOption) {
		return;
	}
	if (pOption->OptionCopy && pOption->OptionTable) {
		free(pOption->OptionTable);
	}
	free(pOption);
}


void coinCopyOptionTable(POPTION pOption, OPTIONTABLE OptionTable, int OptionCount)
{
	if (!pOption) {
		return;
	}
	if (OptionCount > 0) {
		pOption->OptionCount = OptionCount;
		pOption->OptionTable = (PSOLVOPT)malloc(OptionCount * sizeof(SOLVOPTINFO));
		memcpy(pOption->OptionTable, OptionTable, OptionCount * sizeof(SOLVOPTINFO));
		pOption->OptionCopy = 1;
	}
}


void coinSetOptionTable(POPTION pOption, OPTIONTABLE OptionTable, int OptionCount)
{
	if (!pOption) {
		return;
	}
	pOption->OptionTable = OptionTable;
	pOption->OptionCount = OptionCount;
}


int coinGetOptionCount(POPTION pOption)
{
	if (pOption == NULL) {
		return 0;
	}
	return pOption->OptionCount;
}


int coinGetOptionID(POPTION pOption, int OptionNr)
{
	if ((OptionNr < 0) || (OptionNr >= pOption->OptionCount)) {
		return -1;
	}
	return pOption->OptionTable[OptionNr].OptionID;
}


int coinLocateOptionID(POPTION pOption, int OptionID)
{
	int i;

	for (i = 0; i < pOption->OptionCount; i++) {
		if (OptionID == pOption->OptionTable[i].OptionID) {
			return i;
		}
	}
	return -1;
}


int coinGetOptionType(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return OPT_NONE;
	}
	return pOption->OptionTable[OptionNr].OptionType;
}


int coinGetOptionGroup(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return GRP_NONE;
	}
	return pOption->OptionTable[OptionNr].GroupType;
}


int coinLocateOptionName(POPTION pOption, char* OptionName)
{
	int i;

	for (i = 0; i < pOption->OptionCount; i++) {
		if (STRING_COMPARE_NOCASE(OptionName, pOption->OptionTable[i].OptionName) != 0) {
			return pOption->OptionTable[i].OptionID;
		}
		if (STRING_COMPARE_NOCASE(OptionName, pOption->OptionTable[i].ShortName) != 0) {
			return pOption->OptionTable[i].OptionID;
		}
	}
	return -1;
}


const char* coinGetOptionName(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return "";
	}
	return pOption->OptionTable[OptionNr].OptionName;
}


int coinGetOptionNameBuf(POPTION pOption, int OptionID, char* OptionName, int buflen)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		if (OptionName) {
			strcpy(OptionName, "");
		}
		return -1;
	}
	if (OptionName) {
		strncpy(OptionName, pOption->OptionTable[OptionNr].OptionName, buflen-1);
		OptionName[buflen-1] = '\0';
	}
	return 0;
}

const char* coinGetOptionShortName(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return "";
	}
	return pOption->OptionTable[OptionNr].ShortName;
}


int coinGetOptionShortNameBuf(POPTION pOption, int OptionID, char* ShortName, int buflen)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		if (ShortName) {
			strcpy(ShortName, "");
		}
		return -1;
	}
	if (ShortName) {
		strncpy(ShortName, pOption->OptionTable[OptionNr].ShortName, buflen-1);
		ShortName[buflen-1] = '\0';
	}
	return 0;
}


int coinGetIntOptionDefaultValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pOption->OptionTable[OptionNr].DefaultValue;
}



int coinGetIntOptionMinValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pOption->OptionTable[OptionNr].MinValue;
}



int coinGetIntOptionMaxValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pOption->OptionTable[OptionNr].MaxValue;
}




double coinGetRealOptionDefaultValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pOption->OptionTable[OptionNr].DefaultValue;
}



double coinGetRealOptionMinValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pOption->OptionTable[OptionNr].MinValue;
}



double coinGetRealOptionMaxValue(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pOption->OptionTable[OptionNr].MaxValue;
}

int coinGetOptionChanged(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return pOption->OptionTable[OptionNr].changed;
}


int coinGetIntOption(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0;
	}
	return (int)pOption->OptionTable[OptionNr].CurrentValue;
}

int coinSetIntOption(POPTION pOption, int OptionID, int IntValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return -1;
	}
	pOption->OptionTable[OptionNr].CurrentValue = IntValue;
	pOption->OptionTable[OptionNr].changed = 1;
	return 0;
}


double coinGetRealOption(POPTION pOption, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return 0.0;
	}
	return pOption->OptionTable[OptionNr].CurrentValue;
}


int coinSetRealOption(POPTION pOption, int OptionID, double RealValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(pOption, OptionID);
	if (OptionNr == -1) {
		return -1;
	}
	pOption->OptionTable[OptionNr].CurrentValue = RealValue;
	pOption->OptionTable[OptionNr].changed = 1;
	return 0;
}

