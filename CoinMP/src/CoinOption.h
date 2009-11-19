
/*  CoinOption.h  */


#ifndef _COINOPTION_H_
#define _COINOPTION_H_


#if defined(_MSC_VER) && !defined(HAVE_CONFIG_H)
#define SOLVCALL   __stdcall
#else 
#define SOLVCALL
#endif


#undef MAXINT
#undef MAXREAL

#define MAXINT          2100000000L
#define MAXREAL         COIN_DBL_MAX

#define ROUND(x)       (((x)>0)?((long)((x)+0.5)):((long)((x)-0.5)))


#define OPT_NONE			0
#define OPT_ONOFF			1
#define OPT_LIST			2
#define OPT_INT				3
#define OPT_REAL			4
#define OPT_STRING			5

#define GRP_NONE			0
#define GRP_OTHER			0

#define GRP_SIMPLEX			1
#define GRP_PREPROC			2
#define GRP_LOGFILE			3
#define GRP_LIMITS			4
#define GRP_MIPSTRAT		5
#define GRP_MIPCUTS			6
#define GRP_MIPTOL			7
#define GRP_BARRIER			8
#define GRP_NETWORK			9


typedef int    OPTINT;
typedef double OPTVAL;

typedef struct {
			char   OptionName[32];
			char   ShortName[32];
			int    GroupType;
			OPTVAL DefaultValue;
			OPTVAL CurrentValue;
			OPTVAL MinValue;
			OPTVAL MaxValue;
			int		OptionType;
			int    changed;
			int    OptionID;
        } SOLVOPTINFO, *PSOLVOPT;


typedef PSOLVOPT OPTIONTABLE;


typedef struct {
				int OptionCount;
				int OptionCopy;
				OPTIONTABLE OptionTable;

				} OPTIONINFO, *POPTION;


#ifdef __cplusplus
extern "C" {
#endif


POPTION coinCreateOptionObject(void);

void    coinClearOptionObject(POPTION pOption);

void    coinSetOptionTable(POPTION pOption, OPTIONTABLE OptionTable, int OptionCount);
void    coinCopyOptionTable(POPTION pOption, OPTIONTABLE OptionTable, int OptionCount);

int     coinGetOptionCount(POPTION pOption);
int     coinGetOptionID(POPTION pOption, int OptionNr);
int     coinLocateOptionID(POPTION pOption, int OptionID);

int     coinGetOptionType(POPTION pOption, int OptionID);
int     coinGetOptionGroup(POPTION pOption, int OptionID);

int     coinLocateOptionName(POPTION pOption, const char* OptionName);
const char*   coinGetOptionName(POPTION pOption, int OptionID);
int     coinGetOptionNameBuf(POPTION pOption, int OptionID, char* OptionName, int buflen);
const char*   coinGetOptionShortName(POPTION pOption, int OptionNr);
int     coinGetOptionShortNameBuf(POPTION pOption, int OptionNr, char* ShortName, int buflen);

int     coinGetIntOptionDefaultValue(POPTION pOption, int OptionID);
int     coinGetIntOptionMinValue(POPTION pOption, int OptionID);
int     coinGetIntOptionMaxValue(POPTION pOption, int OptionID);

double  coinGetRealOptionDefaultValue(POPTION pOption, int OptionID);
double  coinGetRealOptionMinValue(POPTION pOption, int OptionID);
double  coinGetRealOptionMaxValue(POPTION pOption, int OptionID);

int     coinGetOptionChanged(POPTION pOption, int OptionID);

int     coinGetIntOption(POPTION pOption, int OptionID);
int     coinSetIntOption(POPTION pOption, int OptionID, int IntValue);

double  coinGetRealOption(POPTION pOption, int OptionID);
int     coinSetRealOption(POPTION pOption, int OptionID, double RealValue);

#ifdef __cplusplus
}
#endif


#endif  /* _COINOPTION_H_ */

