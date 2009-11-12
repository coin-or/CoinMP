
/*  CoinSolver.h  */


#ifndef _COINSOLVER_H_
#define _COINSOLVER_H_


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


typedef int (SOLVCALL  *MSGLOGCALLBACK)(const char* MessageStr);

typedef int (SOLVCALL  *ITERCALLBACK)(int    IterCount, 
							double ObjectValue,
							int    IsFeasible, 
							double InfeasValue);

typedef int (SOLVCALL *MIPNODECALLBACK)(int    IterCount, 
							   int	  MipNodeCount,
							   double BestBound,
							   double BestInteger,
							   int    IsMipImproved);


typedef struct {
				char LogFilename[260];

				int OptionCount;
				int OptionCopy;
				OPTIONTABLE OptionTable;

				MSGLOGCALLBACK  MsgLogCallback;
				ITERCALLBACK    IterCallback;
				MIPNODECALLBACK MipNodeCallback;

				} SOLVERINFO, *PSOLVER;


#ifdef __cplusplus
extern "C" {
#endif


PSOLVER coinCreateSolverObject(void);

void coinClearSolverObject(PSOLVER pSolver);

void coinSetLogFilename(PSOLVER pSolver, const char* LogFilename);
const char* coinGetLogFilename(PSOLVER pSolver);

void    coinSetOptionTable(PSOLVER pSolver, OPTIONTABLE OptionTable, int OptionCount);
void    coinCopyOptionTable(PSOLVER pSolver, OPTIONTABLE OptionTable, int OptionCount);

int     coinGetOptionCount(PSOLVER pSolver);
int     coinGetOptionID(PSOLVER pSolver, int OptionNr);
int     coinLocateOptionID(PSOLVER pSolver, int OptionID);

int     coinGetOptionType(PSOLVER pSolver, int OptionID);
int     coinGetOptionGroup(PSOLVER pSolver, int OptionID);

int     coinLocateOptionName(PSOLVER pSolver, char* OptionName);
char*   coinGetOptionName(PSOLVER pSolver, int OptionID);
int     coinGetOptionNameBuf(PSOLVER pSolver, int OptionID, char* OptionName, int buflen);
char*   coinGetOptionShortName(PSOLVER pSolver, int OptionNr);
int     coinGetOptionShortNameBuf(PSOLVER pSolver, int OptionNr, char* ShortName, int buflen);

int     coinGetIntOptionDefaultValue(PSOLVER pSolver, int OptionID);
int     coinGetIntOptionMinValue(PSOLVER pSolver, int OptionID);
int     coinGetIntOptionMaxValue(PSOLVER pSolver, int OptionID);

double  coinGetRealOptionDefaultValue(PSOLVER pSolver, int OptionID);
double  coinGetRealOptionMinValue(PSOLVER pSolver, int OptionID);
double  coinGetRealOptionMaxValue(PSOLVER pSolver, int OptionID);

int     coinGetOptionChanged(PSOLVER pSolver, int OptionID);

int     coinGetIntOption(PSOLVER pSolver, int OptionID);
int     coinSetIntOption(PSOLVER pSolver, int OptionID, int IntValue);

double  coinGetRealOption(PSOLVER pSolver, int OptionID);
int     coinSetRealOption(PSOLVER pSolver, int OptionID, double RealValue);
#ifdef __cplusplus
}
#endif


#endif  /* _COINSOLVER_H_ */

