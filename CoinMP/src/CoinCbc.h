
/*  CoinCbc.cpp  */


#ifndef _COINCBC_H_
#define _COINCBC_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

#include <cfloat>

#include "ClpSimplex.hpp"
#include "CbcModel.hpp"

#include "CglProbing.hpp"
#include "CglGomory.hpp"
#include "CglKnapsackCover.hpp"
#include "CglOddHole.hpp"
#include "CglClique.hpp"
#include "CglLiftAndProject.hpp"
#include "CglSimpleRounding.hpp"



#if defined(_MSC_VER) && !defined(HAVE_CONFIG_H)
#define SOLVCALL   __stdcall
#else
#define SOLVCALL
#endif


typedef void *HCBC;

#ifdef __cplusplus
extern "C" {
#endif

#define CBC_CALL_SUCCESS   0
#define CBC_CALL_FAILED    -1

int CbcOptimizeProblem(PPROBLEM pProblem, PRESULT pResult, PSOLVER pSolver, POPTION pOption, int Method);

int CbcReadMpsFile(PPROBLEM pProblem, const char *ReadFilename);
int CbcWriteMpsFile(PPROBLEM pProblem, const char *WriteFilename);

int CbcGetOptionCount(void);
int CbcGetOptionID(int OptionNr);
SOLVOPTINFO *CbcGetOptionTable(void);
//int CbcLocateOptionID(int OptionID);
SOLVOPTINFO *CbcGetOptionEntry(int OptionID);

int CbcGetOptionChanged(int OptionID);
int CbcGetIntOption(int OptionID);
int CbcSetIntOption(int OptionID, int IntValue);
double CbcGetRealOption(int OptionID);
int CbcSetRealOption(int OptionID, double RealValue);


#ifdef __cplusplus
}
#endif

#define COIN_INT_SOLVEMETHOD     1
#define COIN_INT_PRESOLVETYPE    2
#define COIN_INT_SCALING         3
#define COIN_INT_PERTURBATION    4
#define COIN_INT_PRIMALPIVOTALG  5
#define COIN_INT_DUALPIVOTALG    6
#define COIN_INT_LOGLEVEL        7
#define COIN_INT_MAXITER         8
#define COIN_INT_CRASHIND        9     
#define COIN_INT_CRASHPIVOT     10
#define COIN_REAL_CRASHGAP      11
#define COIN_REAL_PRIMALOBJLIM  12
#define COIN_REAL_DUALOBJLIM    13
#define COIN_REAL_PRIMALOBJTOL  14
#define COIN_REAL_DUALOBJTOL    15
#define COIN_REAL_MAXSECONDS    16

#define COIN_INT_MIPMAXNODES    17
#define COIN_INT_MIPMAXSOL      18
#define COIN_REAL_MIPMAXSEC     19

#define COIN_INT_MIPFATHOMDISC     20
#define COIN_INT_MIPHOTSTART       21
//#define COIN_INT_MIPFORCEPRIOR     21
#define COIN_INT_MIPMINIMUMDROP    22
#define COIN_INT_MIPMAXCUTPASS     23
#define COIN_INT_MIPMAXPASSROOT    24
#define COIN_INT_MIPSTRONGBRANCH   25
#define COIN_INT_MIPSCANGLOBCUTS   26

#define COIN_REAL_MIPINTTOL        30
#define COIN_REAL_MIPINFWEIGHT     31
#define COIN_REAL_MIPCUTOFF        32
#define COIN_REAL_MIPABSGAP        33
#define COIN_REAL_MIPFRACGAP       34

#define COIN_INT_MIPCUT_PROBING          110
#define COIN_INT_MIPPROBE_FREQ           111
#define COIN_INT_MIPPROBE_MODE           112
#define COIN_INT_MIPPROBE_USEOBJ         113
#define COIN_INT_MIPPROBE_MAXPASS        114
#define COIN_INT_MIPPROBE_MAXPROBE       115
#define COIN_INT_MIPPROBE_MAXLOOK        116
#define COIN_INT_MIPPROBE_ROWCUTS        117

#define COIN_INT_MIPCUT_GOMORY           120
#define COIN_INT_MIPGOMORY_FREQ          121
#define COIN_INT_MIPGOMORY_LIMIT         122
#define COIN_REAL_MIPGOMORY_AWAY         123


#define COIN_INT_MIPCUT_KNAPSACK         130
#define COIN_INT_MIPKNAPSACK_FREQ        131
#define COIN_INT_MIPKNAPSACK_MAXIN       132

#define COIN_INT_MIPCUT_ODDHOLE          140
#define COIN_INT_MIPODDHOLE_FREQ         141
#define COIN_REAL_MIPODDHOLE_MINVIOL     142
#define COIN_REAL_MIPODDHOLE_MINVIOLPER  143
#define COIN_INT_MIPODDHOLE_MAXENTRIES   144

#define COIN_INT_MIPCUT_CLIQUE           150
#define COIN_INT_MIPCLIQUE_FREQ          151
#define COIN_INT_MIPCLIQUE_PACKING       152
#define COIN_INT_MIPCLIQUE_STAR          153
#define COIN_INT_MIPCLIQUE_STARMETHOD    154
#define COIN_INT_MIPCLIQUE_STARMAXLEN    155
#define COIN_INT_MIPCLIQUE_STARREPORT    156
#define COIN_INT_MIPCLIQUE_ROW           157
#define COIN_INT_MIPCLIQUE_ROWMAXLEN     158
#define COIN_INT_MIPCLIQUE_ROWREPORT     159
#define COIN_REAL_MIPCLIQUE_MINVIOL      160

#define COIN_INT_MIPCUT_LIFTPROJECT      170
#define COIN_INT_MIPLIFTPRO_FREQ         171
#define COIN_INT_MIPLIFTPRO_BETAONE      172

#define COIN_INT_MIPCUT_SIMPROUND        180
#define COIN_INT_MIPSIMPROUND_FREQ       181   

#define COIN_INT_MIPUSECBCMAIN           200   

#endif  /* _COINCBC_H_ */
