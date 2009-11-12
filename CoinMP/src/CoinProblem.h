
/*  CoinProblem.h  */


#ifndef _COINPROBLEM_H_
#define _COINPROBLEM_H_


typedef struct {
				char ProblemName[200];

				int ColCount;
				int RowCount;
				int NZCount;
				int RangeCount;
				int ObjectSense;
				double ObjectConst;

				int lenColNamesBuf;
				int lenRowNamesBuf;
				int lenObjNameBuf;

				double* ObjectCoeffs;
				double* RHSValues;
				double* RangeValues;
				char* RowType;
				int* MatrixBegin;
				int* MatrixCount;
				int* MatrixIndex; 
				double* MatrixValues;
				double* LowerBounds;
				double* UpperBounds;
				char* ColNamesBuf;
				char* RowNamesBuf;
				char** ColNamesList;
				char** RowNamesList;
				char* ObjectName;

				double* InitValues;

				double* RowLower;
				double* RowUpper;

				char* ColType;

				int SolveAsMIP;
				int IntCount;
				int BinCount;
				int numInts;
				char *IsInt;

				int PriorCount;
				int* PriorIndex;
				int* PriorValues;
				int* BranchDir;

				int SosCount;
				int SosNZCount;
				int* SosType;
				int* SosPrior;
				int* SosBegin;
				int* SosIndex;
				double* SosRef;

				int SemiCount;
				int* SemiIndex;
				double* SemiLower;

				} PROBLEMINFO, *PPROBLEM;


#ifdef __cplusplus
extern "C" {
#endif


PPROBLEM coinCreateProblemObject(void);
void coinClearProblemObject(PPROBLEM pProblem);

void coinSetProblemName(PPROBLEM pProblem, const char *ProblemName);

int coinComputeRowLowerUpper(PPROBLEM pProblem, double CoinDblMax);

int coinGetLenNameBuf(const char* NameBuf, int Count);
int coinGetLenNameListBuf(char** NameList, int Count);

int coinCopyNamesList(char** NamesList, char* NamesBuf, char** argNamesList, int Count);
int coinSetupNamesList(char** NamesList, char* NamesBuf, int Count);

#ifdef __cplusplus
}
#endif


#endif  /* _COINPROBLEM_H_ */

