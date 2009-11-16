
/*  CoinSolver.h  */


#ifndef _COINSOLVER_H_
#define _COINSOLVER_H_


#if defined(_MSC_VER) && !defined(HAVE_CONFIG_H)
#define SOLVCALL   __stdcall
#else 
#define SOLVCALL
#endif


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


#ifdef __cplusplus
}
#endif


#endif  /* _COINSOLVER_H_ */

