/************************************************************************/
/*                                                                      */
/*  CoinMP.dll                                        Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.cpp'                                        */
/*                                                                      */
/*  Version      :  1.4                                                 */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson, Maximal Software               */
/*                                                                      */
/*  Copyright (c) 2002-2009                     Bjarni Kristjansson     */
/*                                                                      */
/************************************************************************/


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

#include <cfloat>

#include "CoinHelperFunctions.hpp"
#include "CoinMessageHandler.hpp"

#include "ClpSimplex.hpp"
#include "ClpPrimalColumnSteepest.hpp"
#include "ClpDualRowSteepest.hpp"
#include "ClpEventHandler.hpp"

#include "OsiSolverInterface.hpp"
#include "OsiClpSolverInterface.hpp"

#include "CbcModel.hpp"
#include "CbcSolver.hpp"
#include "CbcEventHandler.hpp"
#include "CbcBranchActual.hpp"

#include "CglProbing.hpp"
#include "CglGomory.hpp"
#include "CglKnapsackCover.hpp"
#include "CglOddHole.hpp"
#include "CglClique.hpp"
#include "CglLiftAndProject.hpp"
#include "CglSimpleRounding.hpp"



#ifndef COIN_DBL_MAX
#define COIN_DBL_MAX DBL_MAX
#endif


#define SOLVER_EXPORT
#define SOLV_LINK_LIB
#include "CoinMP.h"


#define NEW_STYLE_CBCMAIN


/************************************************************************/
/*  Solver Initialization                                               */
/************************************************************************/


SOLVAPI int SOLVCALL CoinInitSolver(const char* LicenseStr)
{
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinFreeSolver(void)
{
	return SOLV_CALL_SUCCESS;
}

/************************************************************************/
/*  Solver Queries                                                      */
/************************************************************************/


SOLVAPI const char* SOLVCALL CoinGetSolverName(void)
{
	return "CoinMP";
}


SOLVAPI int  SOLVCALL CoinGetSolverNameBuf(char* SolverName, int buflen)
{
	strncpy(SolverName, CoinGetSolverName(), buflen-1);
	SolverName[buflen-1] = '\0';
	return (int)strlen(SolverName);
}


SOLVAPI const char* SOLVCALL CoinGetVersionStr(void)
{
	return "1.4";
}


SOLVAPI int  SOLVCALL CoinGetVersionStrBuf(char* VersionStr, int buflen)
{
	strncpy(VersionStr, CoinGetVersionStr(), buflen-1);
	VersionStr[buflen-1] = '\0';
	return (int)strlen(VersionStr);
}


SOLVAPI double SOLVCALL CoinGetVersion(void)
{
	return 1.4;
}


SOLVAPI int SOLVCALL CoinGetFeatures(void)
{
	return SOLV_FEATURE_LP |
			SOLV_FEATURE_MIP;
}


SOLVAPI int SOLVCALL CoinGetMethods(void)
{
	return SOLV_METHOD_PRIMAL |
			SOLV_METHOD_DUAL | 
			SOLV_METHOD_BARRIER;
}



SOLVAPI double SOLVCALL CoinGetInfinity(void)
{
	return COIN_DBL_MAX;
}


/************************************************************************/
/*  Message Callback Handler                                            */
/************************************************************************/


class CBMessageHandler : public CoinMessageHandler {
public: 
	void setCallback(MSGLOGCALLBACK msgCallback);
	virtual int print();
private:
	MSGLOGCALLBACK msgCallback_;
};


void CBMessageHandler::setCallback(MSGLOGCALLBACK msgCallback)
{
  msgCallback_ = msgCallback;
}


int CBMessageHandler::print()
{
	msgCallback_(const_cast<char*>(messageBuffer()));
	return CoinMessageHandler::print();
}



/************************************************************************/
/*  Iteration Callback Handler                                          */
/************************************************************************/


class CBIterHandler : public ClpEventHandler {

public: 
   void setIterCallback(ITERCALLBACK iterCallback);

	virtual int event(Event whichEvent);
   
  /** Default constructor. */
  CBIterHandler();
  /// Constructor with pointer to model (redundant as setEventHandler does)
  CBIterHandler(ClpSimplex * model);
  /** Destructor */
  virtual ~CBIterHandler();
  /** The copy constructor. */
  CBIterHandler(const CBIterHandler & rhs);
  /// Assignment
  CBIterHandler& operator=(const CBIterHandler & rhs);
  /// Clone
  virtual ClpEventHandler * clone() const ;


private:
	ITERCALLBACK iterCallback_;
};


void CBIterHandler::setIterCallback(ITERCALLBACK iterCallback)
{
  iterCallback_ = iterCallback;
}



int CBIterHandler::event(Event whichEvent)
{
	int numIter;
	double objValue;
	int isPrimalFeasible;
	int isDualFeasible;
	double sumPrimalInfeas; 
	double sumDualInfeas;
	int cancelAsap;

	if ((whichEvent==endOfIteration)) {
		numIter = model_->getIterationCount();
		objValue = model_->getObjValue();
		sumDualInfeas = model_->sumDualInfeasibilities();
		sumPrimalInfeas = model_->sumPrimalInfeasibilities();
		isPrimalFeasible = model_->primalFeasible();
		isDualFeasible = model_->dualFeasible();
		cancelAsap = iterCallback_(numIter, objValue, isPrimalFeasible&&isDualFeasible, sumPrimalInfeas);
		if (cancelAsap) {
			return 5;
		}
	}
	return -1;
}


// Default Constructor 
CBIterHandler::CBIterHandler () : ClpEventHandler() {}

// Copy constructor 
CBIterHandler::CBIterHandler (const CBIterHandler & rhs) : ClpEventHandler(rhs) {}

// Constructor with pointer to model
CBIterHandler::CBIterHandler(ClpSimplex * model) : ClpEventHandler(model) {}

// Destructor 
CBIterHandler::~CBIterHandler () {}

// Assignment operator 
CBIterHandler & CBIterHandler::operator=(const CBIterHandler& rhs)
{
  if (this != &rhs) {
    ClpEventHandler::operator=(rhs);
  }
  return *this;
}

// Clone
ClpEventHandler * CBIterHandler::clone() const
{
	CBIterHandler * iterhandler;

   iterhandler = new CBIterHandler(*this);
	iterhandler->iterCallback_ = this->iterCallback_;
	return iterhandler;
}



/************************************************************************/
/*  Mip Node Callback Handler                                           */
/************************************************************************/


class CBNodeHandler : public CbcEventHandler {

public: 
   void setCallback(MIPNODECALLBACK mipNodeCallback);

	virtual CbcAction event(CbcEvent whichEvent);
   
  /** Default constructor. */
  CBNodeHandler();
  /// Constructor with pointer to model (redundant as setEventHandler does)
  //JPF CBNodeHandler(ClpSimplex * model);
  CBNodeHandler(CbcModel * model);
  

  /** Destructor */
  virtual ~CBNodeHandler();
  /** The copy constructor. */
  CBNodeHandler(const CBNodeHandler & rhs);
  /// Assignment
  CBNodeHandler& operator=(const CBNodeHandler & rhs);
  /// Clone
  virtual CbcEventHandler * clone() const ;


private:
	MIPNODECALLBACK mipNodeCallback_;
	int lastSolCount_;
};


void CBNodeHandler::setCallback(MIPNODECALLBACK mipNodeCallback)
{
  mipNodeCallback_ = mipNodeCallback;
  lastSolCount_ = 0;
}


CBNodeHandler::CbcAction CBNodeHandler::event(CbcEvent whichEvent)
{
	int numIter;
	int numNodes;
	double bestBound;
	double objValue;
	int solCount;
	int cancelAsap;

	if (whichEvent==node) {
		numIter = model_->getIterationCount();
		numNodes = model_->getNodeCount();
		objValue = model_->getObjValue();
		bestBound = model_->getBestPossibleObjValue();
		solCount = model_->getSolutionCount();
		cancelAsap = mipNodeCallback_(numIter, numNodes, bestBound, objValue, solCount != lastSolCount_);
		lastSolCount_ = solCount;
		if (cancelAsap) {
			return stop;
		}
	}
	return noAction;
}


// Default Constructor 
CBNodeHandler::CBNodeHandler () : CbcEventHandler() {}

// Copy constructor 
CBNodeHandler::CBNodeHandler (const CBNodeHandler & rhs) : CbcEventHandler(rhs) {}

// Constructor with pointer to model
//JPF: CBNodeHandler::CBNodeHandler(ClpSimplex * model) : CbcEventHandler(model) {}
CBNodeHandler::CBNodeHandler(CbcModel * model) : CbcEventHandler(model) {}

// Destructor 
CBNodeHandler::~CBNodeHandler () {}

// Assignment operator 
CBNodeHandler & CBNodeHandler::operator=(const CBNodeHandler& rhs)
{
  if (this != &rhs) {
    CbcEventHandler::operator=(rhs);
  }
  return *this;
}

// Clone
CbcEventHandler * CBNodeHandler::clone() const
{
	CBNodeHandler * nodehandler;

	nodehandler = new CBNodeHandler(*this);
	nodehandler->mipNodeCallback_ = this->mipNodeCallback_;
	nodehandler->lastSolCount_ = this->lastSolCount_;
	return nodehandler;
}



/************************************************************************/
/*  Coin Problem Info Structure                                         */
/************************************************************************/


typedef struct {
				ClpSimplex *clp;
				ClpSolve *clp_presolve;
				OsiClpSolverInterface *osi;
				CbcModel *cbc;
				int CbcMain0Already;

				CBMessageHandler *msghandler;
				CBIterHandler *iterhandler;
				CBNodeHandler *nodehandler;

				CglProbing *probing;
				CglGomory *gomory;
				CglKnapsackCover *knapsack;
				CglOddHole *oddhole;
				CglClique *clique;
				CglLiftAndProject *liftpro;
				CglSimpleRounding *rounding;

				int LoadNamesType;

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
				char* IsInt;

				int SosCount;
				int SosNZCount;
				int* SosType;
				int* SosPrior;
				int* SosBegin;
				int* SosIndex;
				double* SosRef;

				int PriorCount;
				int* PriorIndex;
				int* PriorValues;
				int* BranchDir;

				int SolutionStatus;
				char SolutionText[200];

				MSGLOGCALLBACK  MessageLogCallback;
				ITERCALLBACK    IterationCallback;
				MIPNODECALLBACK MipNodeCallback;
				} COININFO, *PCOIN;
 


PCOIN global_pCoin;



/************************************************************************/
/*  Create/Load Problem                                                 */
/************************************************************************/


SOLVAPI HPROB SOLVCALL CoinCreateProblem(const char* ProblemName)
{
	PCOIN pCoin;

	pCoin = (PCOIN) malloc(sizeof(COININFO));
	global_pCoin = pCoin;
	pCoin->clp = new ClpSimplex();
	pCoin->clp_presolve = new ClpSolve();
	pCoin->osi = new OsiClpSolverInterface(pCoin->clp);
	pCoin->cbc = NULL;  /* ERRORFIX 2/22/05: Crashes if not NULL when trying to set message handler */
	pCoin->CbcMain0Already = 0;

	pCoin->msghandler = NULL;
	pCoin->iterhandler = NULL;
	pCoin->nodehandler = NULL;

	pCoin->LoadNamesType = SOLV_LOADNAMES_LIST;

	strcpy(pCoin->ProblemName, ProblemName);

	pCoin->ColCount    = 0;
	pCoin->RowCount    = 0;
	pCoin->NZCount     = 0;
	pCoin->RangeCount  = 0;
	pCoin->ObjectSense = 0;
	pCoin->ObjectConst = 0.0;

	pCoin->lenColNamesBuf   = 0;
	pCoin->lenRowNamesBuf   = 0;
	pCoin->lenObjNameBuf = 0;

	pCoin->ObjectCoeffs = NULL;
	pCoin->RHSValues    = NULL;
	pCoin->RangeValues  = NULL;
	pCoin->RowType      = NULL;
	pCoin->MatrixBegin  = NULL;
	pCoin->MatrixCount  = NULL;
	pCoin->MatrixIndex  = NULL; 
	pCoin->MatrixValues = NULL;
	pCoin->LowerBounds  = NULL;
	pCoin->UpperBounds  = NULL;
	pCoin->ColNamesBuf  = NULL;
	pCoin->RowNamesBuf  = NULL;
	pCoin->ColNamesList = NULL;
	pCoin->RowNamesList = NULL;
	pCoin->ObjectName   = NULL;

	pCoin->InitValues	= NULL;

	pCoin->RowLower		= NULL;
	pCoin->RowUpper		= NULL;

	pCoin->ColType		= NULL;

	pCoin->SolveAsMIP	= 0;
	pCoin->IntCount		= 0;
	pCoin->BinCount		= 0;
	pCoin->numInts		= 0;
	pCoin->IsInt		= NULL;

	pCoin->SosCount		= 0;
	pCoin->SosNZCount	= 0;
	pCoin->SosType		= NULL;
	pCoin->SosPrior		= NULL;
	pCoin->SosBegin		= NULL;
	pCoin->SosIndex		= NULL;
	pCoin->SosRef		= NULL;

	pCoin->PriorCount	= 0;
	pCoin->PriorIndex	= NULL;
	pCoin->PriorValues	= NULL;
	pCoin->BranchDir	= NULL;

	pCoin->SolutionStatus = 0;
	strcpy(pCoin->SolutionText, "");

	pCoin->MessageLogCallback = NULL;
	pCoin->IterationCallback = NULL;
	pCoin->MipNodeCallback = NULL;

	return (HPROB)pCoin;
}


int coinComputeRowLowerUpper(PCOIN pCoin, char* RowType, double* RHSValues, double* RangeValues)
{
	int i;
	double RangeABS;

	if (!pCoin->RowLower || !pCoin->RowUpper) {
		return 0;
	}
	if (!RowType) {
		/* if NO RowType, we treat RHSValues as RowLower and RangeValues as RowUpper */
		for (i = 0; i < pCoin->RowCount; i++) {
			pCoin->RowLower[i] = RHSValues ? RHSValues[i] : -COIN_DBL_MAX;
			pCoin->RowUpper[i] = RangeValues ? RangeValues[i] : COIN_DBL_MAX;
		}
		return 1;
	}
	for (i = 0; i < pCoin->RowCount; i++ ){
		switch (RowType[i]) {
			case 'L':
				pCoin->RowLower[i] = -COIN_DBL_MAX;
				pCoin->RowUpper[i] = RHSValues ? RHSValues[i] : COIN_DBL_MAX;
				break;

			case 'G':
				pCoin->RowLower[i] = RHSValues ? RHSValues[i] : -COIN_DBL_MAX;
				pCoin->RowUpper[i] = COIN_DBL_MAX;
				break;

			case 'E':
				pCoin->RowLower[i] = RHSValues ? RHSValues[i] : 0.0;
				pCoin->RowUpper[i] = RHSValues ? RHSValues[i] : 0.0;
				break;

			case 'R':
				RangeABS = (RangeValues) ? ((RangeValues[i] >= 0.0) ? RangeValues[i] : -RangeValues[i]) : 0.0;
				pCoin->RowLower[i] = (RHSValues ? RHSValues[i] : -COIN_DBL_MAX) - RangeABS;
				pCoin->RowUpper[i] = RHSValues ? RHSValues[i] : COIN_DBL_MAX;
				break;

			case 'N':
				pCoin->RowLower[i] = -COIN_DBL_MAX;
				pCoin->RowUpper[i] = COIN_DBL_MAX;
				break;

			default:
				return 0;
		}
	}
	return 1;
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


int coinGetLenNameBufType(char** NameList, int Count, int LoadNameType)
{
	int i, len;
	int lenBuf;
	char* NameBuf;

	lenBuf = 0;
	switch (LoadNameType) {

		case SOLV_LOADNAMES_BUFFER:
			NameBuf = (char* )NameList;
			lenBuf = coinGetLenNameBuf(NameBuf, Count);
			break;

		case SOLV_LOADNAMES_DEFAULT:
		case SOLV_LOADNAMES_LIST:
			for (i = 0; i < Count; i++) {
				len = (int)strlen(NameList[i]) + 1;
				lenBuf += len;
			}
			break;

		default:
			return 0;
	}
	return lenBuf;
}


int coinSetupNamesList(char** NamesList, char* NamesBuf, char** argNamesList, int Count, int LoadNamesType)
{
	int i,k,len;

	if (!NamesList || !argNamesList || !NamesBuf || Count == 0) {
		return 0;
	}
	k = 0;
	for (i = 0; i < Count; i++) {
		NamesList[i] = &NamesBuf[k];
		if (LoadNamesType == SOLV_LOADNAMES_LIST) {
			strcpy(NamesList[i], argNamesList[i]);
		}
		len = (int)strlen(NamesList[i]) + 1;
		k += len;
	}
	return 1;
}



void coinLoadNamesList(PCOIN pCoin, char** ColNamesList, char** RowNamesList, char* objectName)
{
	int i;

	/* Load names */
	if (RowNamesList || ColNamesList) {
		std::vector<std::string> rowNamesVect;
		std::vector<std::string> colNamesVect;
		rowNamesVect.reserve(pCoin->RowCount);
		colNamesVect.reserve(pCoin->ColCount);
		if (RowNamesList) {
			for (i = 0; i < pCoin->RowCount; i++) {
				rowNamesVect.push_back(RowNamesList[i]);
			}
		}
		if (ColNamesList) {
			for (i = 0; i < pCoin->ColCount; i++) {
				colNamesVect.push_back(ColNamesList[i]);
			}
		}
		pCoin->clp->copyNames(rowNamesVect, colNamesVect);
	}
}


SOLVAPI int SOLVCALL CoinLoadProblem(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char** ColNames, char** RowNames, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (ColCount == 0) {
		return SOLV_CALL_FAILED;
	}
	pCoin->ColCount = ColCount;
	pCoin->RowCount = RowCount;
	pCoin->NZCount = NZCount;
	pCoin->RangeCount = RangeCount;
	pCoin->ObjectSense = ObjectSense;
	pCoin->ObjectConst = ObjectConst;

	if (ColNames)   pCoin->lenColNamesBuf = coinGetLenNameBufType(ColNames, ColCount, pCoin->LoadNamesType);
	if (RowNames)   pCoin->lenRowNamesBuf = coinGetLenNameBufType(RowNames, RowCount, pCoin->LoadNamesType);
	if (ObjectName) pCoin->lenObjNameBuf  = (int)strlen(ObjectName) + 1;

	if (ObjectCoeffs) pCoin->ObjectCoeffs = (double*) malloc(pCoin->ColCount     * sizeof(double));
	if (RHSValues)    pCoin->RHSValues    = (double*) malloc(pCoin->RowCount     * sizeof(double));
	if (RangeValues)  pCoin->RangeValues  = (double*) malloc(pCoin->RowCount     * sizeof(double));
	if (RowType)      pCoin->RowType      = (char*)   malloc(pCoin->RowCount     * sizeof(char));
	if (MatrixBegin)  pCoin->MatrixBegin  = (int*)    malloc((pCoin->ColCount+1) * sizeof(int));
	if (MatrixCount)  pCoin->MatrixCount  = (int*)    malloc(pCoin->ColCount     * sizeof(int));
	if (MatrixIndex)  pCoin->MatrixIndex  = (int*)    malloc(pCoin->NZCount      * sizeof(int)); 
	if (MatrixValues) pCoin->MatrixValues = (double*) malloc(pCoin->NZCount      * sizeof(double));
	if (LowerBounds)  pCoin->LowerBounds  = (double*) malloc(pCoin->ColCount     * sizeof(double));
	if (UpperBounds)  pCoin->UpperBounds  = (double*) malloc(pCoin->ColCount     * sizeof(double));
	if (ColNames)     pCoin->ColNamesList = (char**)  malloc(pCoin->ColCount     * sizeof(char* ));
	if (RowNames)     pCoin->RowNamesList = (char**)  malloc(pCoin->RowCount     * sizeof(char* ));
	if (ColNames)     pCoin->ColNamesBuf  = (char*)   malloc(pCoin->lenColNamesBuf * sizeof(char));
	if (RowNames)     pCoin->RowNamesBuf  = (char*)   malloc(pCoin->lenRowNamesBuf * sizeof(char));
	if (ObjectName)   pCoin->ObjectName   = (char*)   malloc(pCoin->lenObjNameBuf  * sizeof(char));

	if (pCoin->RowCount > 0) {
		pCoin->RowLower = (double* )malloc(pCoin->RowCount*sizeof(double));
		pCoin->RowUpper = (double* )malloc(pCoin->RowCount*sizeof(double));
		if (!pCoin->RowLower || !pCoin->RowUpper) {
			return SOLV_CALL_FAILED;
		}
	}

	if (pCoin->ObjectCoeffs) memcpy(pCoin->ObjectCoeffs, ObjectCoeffs, pCoin->ColCount     * sizeof(double));
	if (pCoin->RHSValues)    memcpy(pCoin->RHSValues,    RHSValues,    pCoin->RowCount     * sizeof(double));
	if (pCoin->RangeValues)  memcpy(pCoin->RangeValues,  RangeValues,  pCoin->RowCount     * sizeof(double));
	if (pCoin->RowType)      memcpy(pCoin->RowType,      RowType,      pCoin->RowCount     * sizeof(char));
	if (pCoin->MatrixBegin)  memcpy(pCoin->MatrixBegin,  MatrixBegin,  (pCoin->ColCount+1) * sizeof(int));
	if (pCoin->MatrixCount)  memcpy(pCoin->MatrixCount,  MatrixCount,  pCoin->ColCount     * sizeof(int));
	if (pCoin->MatrixIndex)  memcpy(pCoin->MatrixIndex,  MatrixIndex,  pCoin->NZCount      * sizeof(int));
	if (pCoin->MatrixValues) memcpy(pCoin->MatrixValues, MatrixValues, pCoin->NZCount      * sizeof(double));
	if (pCoin->LowerBounds)  memcpy(pCoin->LowerBounds,  LowerBounds,  pCoin->ColCount     * sizeof(double));
	if (pCoin->UpperBounds)  memcpy(pCoin->UpperBounds,  UpperBounds,  pCoin->ColCount     * sizeof(double));
	if (pCoin->ObjectName)   memcpy(pCoin->ObjectName,   ObjectName,   pCoin->lenObjNameBuf  * sizeof(char));

	if (pCoin->LoadNamesType == SOLV_LOADNAMES_BUFFER) {
		if (pCoin->ColNamesBuf)  memcpy(pCoin->ColNamesBuf,  (char* )ColNames,     pCoin->lenColNamesBuf * sizeof(char));
		if (pCoin->RowNamesBuf)  memcpy(pCoin->RowNamesBuf,  (char* )RowNames,     pCoin->lenRowNamesBuf * sizeof(char));
	}
	coinSetupNamesList(pCoin->ColNamesList, pCoin->ColNamesBuf, ColNames, ColCount, pCoin->LoadNamesType);
	coinSetupNamesList(pCoin->RowNamesList, pCoin->RowNamesBuf, RowNames, RowCount, pCoin->LoadNamesType);

	pCoin->clp->setOptimizationDirection(ObjectSense);

	if (!coinComputeRowLowerUpper(pCoin, RowType, RHSValues, RangeValues)) {
		return SOLV_CALL_FAILED;
	}

	pCoin->clp->loadProblem(ColCount, RowCount, MatrixBegin, MatrixIndex, MatrixValues,
							LowerBounds, UpperBounds, ObjectCoeffs, pCoin->RowLower, pCoin->RowUpper);

	coinLoadNamesList(pCoin, pCoin->ColNamesList, pCoin->RowNamesList, ObjectName);

	return SOLV_CALL_SUCCESS;
}

// CoinLoadProblemBuf accepts ColNames/RowNames as character buffer with \0 after each name

SOLVAPI int SOLVCALL CoinLoadProblemBuf(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char* ColNames, char* RowNames, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;
	int StoreLoadNamesType;
	int result;

	StoreLoadNamesType = pCoin->LoadNamesType;
	pCoin->LoadNamesType = SOLV_LOADNAMES_BUFFER;
	result = CoinLoadProblem(hProb, ColCount, RowCount, NZCount, RangeCount,
				ObjectSense, ObjectConst, ObjectCoeffs, LowerBounds, UpperBounds, RowType,
				RHSValues, RangeValues, MatrixBegin, MatrixCount, MatrixIndex, MatrixValues,
				(char**)ColNames, (char**)RowNames, ObjectName);
	pCoin->LoadNamesType = StoreLoadNamesType;
	return result;
}


SOLVAPI int SOLVCALL CoinLoadInitValues(HPROB hProb, double* InitValues)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (InitValues)   pCoin->InitValues   = (double* ) malloc(pCoin->ColCount     * sizeof(double));

	if (pCoin->InitValues)   memcpy(pCoin->InitValues,   InitValues,   pCoin->ColCount     * sizeof(double));
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadInteger(HPROB hProb, char* ColType)
{   
	PCOIN pCoin = (PCOIN)hProb;
	int i;

	if (pCoin->ColCount == 0) {
		return SOLV_CALL_FAILED;
	}
	if (ColType) {
		pCoin->ColType = (char* )malloc(pCoin->ColCount * sizeof(char));
		if (!pCoin->ColType) {
			return SOLV_CALL_FAILED;
		}
		memcpy(pCoin->ColType, ColType, pCoin->ColCount * sizeof(char));
	}
	pCoin->IsInt = (char* )malloc(pCoin->ColCount*sizeof(char));
	if (!pCoin->IsInt) {
		return SOLV_CALL_FAILED;
	}
	for (i = 0; i < pCoin->ColCount; i++ ) {
		switch (ColType[i]) {
			case 'B': 
				pCoin->BinCount++;
				pCoin->IsInt[i] = 1;
				pCoin->SolveAsMIP = 1;
				break;

			case 'I': 
				pCoin->IntCount++;
				pCoin->IsInt[i] = 1;
				pCoin->SolveAsMIP = 1;
				break;

			case 'C':
				pCoin->IsInt[i] = 0;
				break;

			default:
				pCoin->IsInt[i] = 0;
				return SOLV_CALL_FAILED;
		}
	}
	if (pCoin->SolveAsMIP) {
		if (!pCoin->cbc) {
			pCoin->cbc = new CbcModel(*pCoin->osi);
		}
		for (i = 0; i < pCoin->ColCount; i++) {
			if (pCoin->IsInt[i]) {
				pCoin->cbc->solver()->setInteger(i);
				pCoin->osi->setInteger(i);
				pCoin->numInts++;
			}
		}
#ifdef NEW_STYLE_CBCMAIN
		if (CoinGetIntOption(hProb, COIN_INT_MIPUSECBCMAIN)) {
			if (!pCoin->CbcMain0Already) {
				CbcMain0(*pCoin->cbc);
				pCoin->CbcMain0Already = 1;
			}
		}
#endif
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinLoadPriority(HPROB hProb, int PriorCount, int* PriorIndex, 
									  int* PriorValues, int* BranchDir)
{
	PCOIN pCoin = (PCOIN)hProb;
	int *priorVar;
	int *priorCbc;
	int i,k;

	pCoin->PriorCount = PriorCount;
	if (PriorIndex)  pCoin->PriorIndex  = (int* )malloc(PriorCount * sizeof(int));
	if (PriorValues) pCoin->PriorValues = (int* )malloc(PriorCount * sizeof(int));
	if (BranchDir)	 pCoin->BranchDir   = (int* )malloc(PriorCount * sizeof(int));
	if (pCoin->PriorIndex)  memcpy(pCoin->PriorIndex,  PriorIndex,  PriorCount * sizeof(int));
	if (pCoin->PriorValues) memcpy(pCoin->PriorValues, PriorValues, PriorCount * sizeof(int));
	if (pCoin->BranchDir)   memcpy(pCoin->BranchDir,   BranchDir,   PriorCount * sizeof(int));

	if (!pCoin->SolveAsMIP) {
		return SOLV_CALL_FAILED;
	}
	if (!pCoin->IsInt) {
		return SOLV_CALL_FAILED;
	}

	priorVar = (int *)malloc(pCoin->ColCount * sizeof(int));
	if (!priorVar) {
		return SOLV_CALL_FAILED;
	}
	//reset the priorVar
	for (i = 0; i < pCoin->ColCount; i++) {
		priorVar[i] = 1000;
	}
	for (i = 0; i < PriorCount; i++) {
		if ((PriorIndex[i] < 0) || (PriorIndex[i] >= pCoin->ColCount)) {
			free(priorVar);
			return SOLV_CALL_FAILED;
		}
		priorVar[PriorIndex[i]] = PriorValues[i];
	}
	//Create an array to give to cbc
	priorCbc = (int *)malloc(pCoin->numInts * sizeof(int));
	if (!priorCbc) {
		free(priorVar);
		return SOLV_CALL_FAILED;
	}
	k = 0;
	for (i = 0; i < pCoin->ColCount; i++) {
		if (pCoin->IsInt[i]) {
			priorCbc[k++] = priorVar[i];
		}
	}
	pCoin->cbc->passInPriorities(priorCbc, false);
	free(priorCbc);
	free(priorVar);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadSemiCont(HPROB hProb, int SemiCount, int* SemiIndex)
{
	return SOLV_CALL_FAILED;
}


SOLVAPI int SOLVCALL CoinLoadSos(HPROB hProb, int SosCount, int SosNZCount, 
								int* SosType, int* SosPrior, int* SosBegin, 
								int* SosIndex, double* SosRef)
{
	PCOIN pCoin = (PCOIN)hProb;
	int sos, count;
	int* which;
	int type;

	if ((SosCount == 0) || (SosNZCount == 0)) {
		return SOLV_CALL_FAILED;
	}
	pCoin->SosCount = SosCount;
	pCoin->SosNZCount = SosNZCount;

	if (SosType)  pCoin->SosType  = (int* )malloc(SosCount     * sizeof(int));
	if (SosPrior) pCoin->SosPrior = (int* )malloc(SosCount     * sizeof(int));
	if (SosBegin) pCoin->SosBegin = (int* )malloc((SosCount+1) * sizeof(int));
	if (SosIndex) pCoin->SosIndex = (int* )malloc(SosNZCount   * sizeof(int));
	if (SosRef)   pCoin->SosRef   = (double* )malloc(SosNZCount* sizeof(double));

	if (pCoin->SosType)  memcpy(pCoin->SosType,  SosType,  SosCount     * sizeof(int));
	if (pCoin->SosPrior) memcpy(pCoin->SosPrior, SosPrior, SosCount     * sizeof(int));
	if (pCoin->SosBegin) memcpy(pCoin->SosBegin, SosBegin, (SosCount+1) * sizeof(int));
	if (pCoin->SosIndex) memcpy(pCoin->SosIndex, SosIndex, SosNZCount   * sizeof(int));
	if (pCoin->SosRef)   memcpy(pCoin->SosRef,   SosRef,   SosNZCount   * sizeof(double));

	pCoin->SolveAsMIP = 1;
	if (!pCoin->cbc) {
		pCoin->cbc = new CbcModel(*pCoin->osi);
	}
#ifdef NEW_STYLE_CBCMAIN
	if (CoinGetIntOption(hProb, COIN_INT_MIPUSECBCMAIN)) {
		if (!pCoin->CbcMain0Already) {
			CbcMain0(*pCoin->cbc);
			pCoin->CbcMain0Already = 1;
		}
	}
#endif
	for (sos = 0; sos < SosCount; sos++) {
		count = SosBegin[sos+1] - SosBegin[sos];
		which = &SosIndex[SosBegin[sos]];
		type = SosType[sos];
		CbcObject *sosObject = new CbcSOS(pCoin->cbc, count, which, NULL, 0, type);
		pCoin->cbc->addObjects(1, &sosObject);
		delete sosObject;
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadQuadratic(HPROB hProb, int* QuadBegin, int* QuadCount, 
										int* QuadIndex, double* QuadValues)
{
	return SOLV_CALL_FAILED;
}


SOLVAPI int SOLVCALL CoinLoadNonlinear(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
										int* NlpBegin, int* NlpOper, int* NlpArg1, 
										int* NlpArg2, int* NlpIndex1, int* NlpIndex2, 
										double* NlpValue1, double* NlpValue2)
{
	return SOLV_CALL_FAILED;
}



SOLVAPI int SOLVCALL CoinUnloadProblem(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;
	
	if (pCoin) {
		delete pCoin->msghandler;
		delete pCoin->iterhandler;
		delete pCoin->nodehandler;
		delete pCoin->clp;
		delete pCoin->clp_presolve;
		delete pCoin->cbc;

		if (pCoin->ObjectCoeffs) free(pCoin->ObjectCoeffs);
		if (pCoin->RHSValues)    free(pCoin->RHSValues);
		if (pCoin->RangeValues)  free(pCoin->RangeValues);
		if (pCoin->RowType)      free(pCoin->RowType);
		if (pCoin->MatrixBegin)  free(pCoin->MatrixBegin);
		if (pCoin->MatrixCount)  free(pCoin->MatrixCount);
		if (pCoin->MatrixIndex)  free(pCoin->MatrixIndex); 
		if (pCoin->MatrixValues) free(pCoin->MatrixValues);
		if (pCoin->LowerBounds)  free(pCoin->LowerBounds);
		if (pCoin->UpperBounds)  free(pCoin->UpperBounds);

		if (pCoin->ColNamesBuf)  free(pCoin->ColNamesBuf);
		if (pCoin->RowNamesBuf)  free(pCoin->RowNamesBuf);
		if (pCoin->ColNamesList) free(pCoin->ColNamesList);
		if (pCoin->RowNamesList) free(pCoin->RowNamesList);
		if (pCoin->ObjectName)   free(pCoin->ObjectName);

		if (pCoin->InitValues)   free(pCoin->InitValues);

		if (pCoin->RowLower)	 free(pCoin->RowLower);
		if (pCoin->RowUpper)	 free(pCoin->RowUpper);

		if (pCoin->ColType)		 free(pCoin->ColType);
		if (pCoin->IsInt)		 free(pCoin->IsInt);

		if (pCoin->SosType)		 free(pCoin->SosType);
		if (pCoin->SosPrior)	 free(pCoin->SosPrior);
		if (pCoin->SosBegin)	 free(pCoin->SosBegin);
		if (pCoin->SosIndex)	 free(pCoin->SosIndex);
		if (pCoin->SosRef)		 free(pCoin->SosRef);

		if (pCoin->PriorIndex)	 free(pCoin->PriorIndex);
		if (pCoin->PriorValues)	 free(pCoin->PriorValues);
		if (pCoin->BranchDir)	 free(pCoin->BranchDir);
	}
	free(pCoin);
	pCoin = NULL;
	return SOLV_CALL_SUCCESS;
}



#define CHECK_MAXNAMELEN   100


SOLVAPI int SOLVCALL CoinCheckProblem(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;
	int i;

	if (pCoin->ColCount == 0) {
		return SOLV_CHECK_COLCOUNT;
	}
	if ((pCoin->RowCount < 0) ||
		(pCoin->NZCount < 0) ||
		(pCoin->RangeCount < 0)) {
		return SOLV_CHECK_ROWCOUNT;
	}
	if ((pCoin->RangeCount > pCoin->RowCount)) {
		return SOLV_CHECK_RANGECOUNT;
	}
	if ((pCoin->ObjectSense < -1) || 
		(pCoin->ObjectSense > 1)) {
		return SOLV_CHECK_OBJSENSE;
	}
	if (pCoin->RowType && (pCoin->RowCount > 0)) {
		for (i = 0; i < pCoin->RowCount; i++) {
			if ((pCoin->RowType[i] != 'L') &&
				(pCoin->RowType[i] != 'E') &&
				(pCoin->RowType[i] != 'G') &&
				(pCoin->RowType[i] != 'R') &&
				(pCoin->RowType[i] != 'N')) {
				return SOLV_CHECK_ROWTYPE;
			}
		}
	}
	if (pCoin->NZCount > 0) {
		for (i = 0; i < pCoin->ColCount; i++) {
			if (pCoin->MatrixBegin[i] < 0) {
				return SOLV_CHECK_MATBEGIN;
			}
			if (pCoin->MatrixCount[i] < 0) {
				return SOLV_CHECK_MATCOUNT;
			}
			if (pCoin->MatrixBegin[i+1] - pCoin->MatrixBegin[i] != pCoin->MatrixCount[i]) {
				return SOLV_CHECK_MATBEGCNT;
			}
		}
		if (pCoin->MatrixBegin[pCoin->ColCount] != pCoin->NZCount) {
			return 100+pCoin->MatrixBegin[pCoin->ColCount];
		}
		for (i = 0; i < pCoin->NZCount; i++) {
			if (pCoin->MatrixIndex[i] < 0) {
				return SOLV_CHECK_MATINDEX;
			}
			if (pCoin->MatrixIndex[i] >= pCoin->RowCount) {
				return SOLV_CHECK_MATINDEXROW;
			}
		}
	}
	if (pCoin->LowerBounds && pCoin->UpperBounds) {
		for (i = 0; i < pCoin->ColCount; i++) {
			if (pCoin->LowerBounds[i] > pCoin->UpperBounds[i]) {
				return SOLV_CHECK_BOUNDS;
			}
		}
	}
	if (pCoin->ColType) {
		for (i = 0; i < pCoin->ColCount; i++) {
			if ((pCoin->ColType[i] != 'C') &&
				(pCoin->ColType[i] != 'B') &&
				(pCoin->ColType[i] != 'I')) {
				return SOLV_CHECK_COLTYPE;
			}
		}
	}
	if (pCoin->ColNamesBuf) {
		if (pCoin->lenColNamesBuf <= 0) {
			return SOLV_CHECK_COLNAMES;
		}
		if (pCoin->lenColNamesBuf > pCoin->ColCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_COLNAMESLEN;
		}
	}
	if (pCoin->RowNamesBuf) {
		if (pCoin->lenRowNamesBuf <= 0) {
			return SOLV_CHECK_ROWNAMES;
		}
		if (pCoin->lenRowNamesBuf > pCoin->RowCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_ROWNAMSLEN;
		}
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinSetLoadNamesType(HPROB hProb, int LoadNamesType)
{
	PCOIN pCoin = (PCOIN)hProb;

	if ((LoadNamesType < SOLV_LOADNAMES_DEFAULT) &&  
		(LoadNamesType > SOLV_LOADNAMES_BUFFER)) {
		return SOLV_CALL_FAILED;
	}
	if (LoadNamesType == SOLV_LOADNAMES_DEFAULT)
		pCoin->LoadNamesType = SOLV_LOADNAMES_LIST;
	else {
		pCoin->LoadNamesType = LoadNamesType; 
	}
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Problem Queries                                                     */
/************************************************************************/


SOLVAPI const char* SOLVCALL CoinGetProblemName(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->ProblemName;
}


SOLVAPI int SOLVCALL CoinGetProblemNameBuf(HPROB hProb, char* ProbName, int buflen)
{
	strncpy(ProbName, CoinGetProblemName(hProb), buflen-1);
	ProbName[buflen-1] = '\0';
	return (int)strlen(ProbName);
}



SOLVAPI int SOLVCALL CoinGetColCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->ColCount;
}


SOLVAPI int SOLVCALL CoinGetRowCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->RowCount;
}



SOLVAPI const char* SOLVCALL CoinGetColName(HPROB hProb, int col)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->ColNamesList[col];
}


SOLVAPI int SOLVCALL CoinGetColNameBuf(HPROB hProb, int col, char* ColName, int buflen)
{
	strncpy(ColName, CoinGetColName(hProb, col), buflen-1);
	ColName[buflen-1] = '\0';
	return (int)strlen(ColName);
}


SOLVAPI const char* SOLVCALL CoinGetRowName(HPROB hProb, int row)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->RowNamesList[row];
}


SOLVAPI int SOLVCALL CoinGetRowNameBuf(HPROB hProb, int row, char* RowName, int buflen)
{
	strncpy(RowName, CoinGetRowName(hProb, row), buflen-1);
	RowName[buflen-1] = '\0';
	return (int)strlen(RowName);
}


/************************************************************************/
/*  Callback Handling                                                   */
/************************************************************************/


int coinWriteMsgLog(const char* FormatStr, ...)
{
	va_list pVa;
	char strbuf[256];

	va_start(pVa,FormatStr);
	vsprintf(strbuf,FormatStr,pVa);
	if (global_pCoin->MessageLogCallback) {
		global_pCoin->MessageLogCallback(strbuf);
	}
	return SOLV_CALL_SUCCESS;
}


int coinIterLogCallback(int IterCount, double ObjectValue, int IsFeasible, double InfeasValue)
{
	if (!global_pCoin->SolveAsMIP) {
		if (((IterCount < 100) && ((IterCount % 10) == 0)) ||
			 ((IterCount % 100) == 0)) {
			if (!IsFeasible)
				coinWriteMsgLog("Iteration: %5d  %16.8lg  %16.8lg",IterCount, ObjectValue, InfeasValue);
			else {
				coinWriteMsgLog("Iteration: %5d  %16.8lg",IterCount, ObjectValue);
			}
		}
	}
	if (global_pCoin->IterationCallback) {
		global_pCoin->IterationCallback(IterCount, ObjectValue, IsFeasible, InfeasValue);
	}
	return SOLV_CALL_SUCCESS;
}


int coinNodeLogCallback(int IterCount, int NodeCount, double BestBound, double BestObject, int IsMipImproved)
{
	if ((NodeCount > 0) && (((NodeCount % 100) == 0) || (IsMipImproved))) {
		coinWriteMsgLog("Node: %5d  %s  %16.8lg  %16.8lg", 
		                   NodeCount, (IsMipImproved) ? "*" : " ", BestBound, BestObject);
	}
	if (global_pCoin->MipNodeCallback) {
		global_pCoin->MipNodeCallback(IterCount, NodeCount, BestBound, BestObject, IsMipImproved);
	}
	return SOLV_CALL_SUCCESS;
}




SOLVAPI int SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->MessageLogCallback = MsgLogCallback;
	delete pCoin->msghandler;
	pCoin->msghandler = new CBMessageHandler();
	pCoin->msghandler->setCallback(MsgLogCallback);
	pCoin->msghandler->setLogLevel(CoinGetIntOption(hProb, COIN_INT_LOGLEVEL));
	if (pCoin->clp) pCoin->clp->passInMessageHandler(pCoin->msghandler);
	if (pCoin->cbc) pCoin->cbc->passInMessageHandler(pCoin->msghandler);
	if (pCoin->osi) pCoin->osi->passInMessageHandler(pCoin->msghandler);
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->IterationCallback = IterCallback;
	delete pCoin->iterhandler;
	pCoin->iterhandler = new CBIterHandler(pCoin->clp);
	pCoin->iterhandler->setIterCallback(IterCallback);
	if (pCoin->clp) pCoin->clp->passInEventHandler(pCoin->iterhandler);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinSetMipNodeCallback(HPROB hProb, MIPNODECALLBACK MipNodeCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->MipNodeCallback = MipNodeCallback;
	delete pCoin->nodehandler;

	//JPF: pCoin->nodehandler = new CBNodeHandler(pCoin->clp);
	pCoin->nodehandler = new CBNodeHandler(pCoin->cbc);

	pCoin->nodehandler->setCallback(MipNodeCallback);
#ifdef NEW_STYLE_CBCMAIN
	if (CoinGetIntOption(hProb, COIN_INT_MIPUSECBCMAIN)) {
		if (pCoin->cbc) pCoin->cbc->passInEventHandler(pCoin->nodehandler);
		}
	else
#endif
	{
		if (pCoin->cbc) pCoin->cbc->passInEventHandler(pCoin->nodehandler);
	}
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Option Setting                                                      */
/************************************************************************/


int coinSetClpOptions(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;
	ClpSolve::SolveType method;
	ClpSolve::PresolveType presolve;

	/* check if it has been changed, leave alone otherwise */
	if (CoinGetOptionChanged(hProb, COIN_INT_SCALING))			pCoin->clp->scaling(CoinGetIntOption(hProb,COIN_INT_SCALING));
	if (CoinGetOptionChanged(hProb, COIN_INT_PERTURBATION))		pCoin->clp->setPerturbation(CoinGetIntOption(hProb, COIN_INT_PERTURBATION));

	if (CoinGetOptionChanged(hProb, COIN_INT_MAXITER))			pCoin->clp->setMaximumIterations(CoinGetIntOption(hProb, COIN_INT_MAXITER));

	if (CoinGetOptionChanged(hProb, COIN_REAL_PRIMALOBJLIM))	pCoin->clp->setPrimalObjectiveLimit(CoinGetIntOption(hProb, COIN_REAL_PRIMALOBJLIM));
	if (CoinGetOptionChanged(hProb, COIN_REAL_DUALOBJLIM))		pCoin->clp->setDualObjectiveLimit(CoinGetIntOption(hProb, COIN_REAL_DUALOBJLIM));
	if (CoinGetOptionChanged(hProb, COIN_REAL_PRIMALOBJTOL))	pCoin->clp->setPrimalTolerance(CoinGetIntOption(hProb, COIN_REAL_PRIMALOBJTOL));
	if (CoinGetOptionChanged(hProb, COIN_REAL_DUALOBJTOL))		pCoin->clp->setDualTolerance(CoinGetIntOption(hProb, COIN_REAL_DUALOBJTOL));

	if (CoinGetOptionChanged(hProb, COIN_INT_PRIMALPIVOTALG)) {
		ClpPrimalColumnSteepest primalSteepest(CoinGetIntOption(hProb, COIN_INT_PRIMALPIVOTALG));
		pCoin->clp->setPrimalColumnPivotAlgorithm(primalSteepest);
	}

	if (CoinGetOptionChanged(hProb, COIN_INT_DUALPIVOTALG)) {
		ClpDualRowSteepest dualSteepest(CoinGetIntOption(hProb, COIN_INT_DUALPIVOTALG));
		pCoin->clp->setDualRowPivotAlgorithm(dualSteepest);
	}

	if (CoinGetOptionChanged(hProb, COIN_INT_CRASHIND)) { 
		if (CoinGetIntOption(hProb, COIN_INT_CRASHIND)) {
			pCoin->clp->crash(CoinGetIntOption(hProb, COIN_REAL_CRASHGAP),
								CoinGetIntOption(hProb, COIN_INT_CRASHPIVOT));
		}
	}

	if (CoinGetOptionChanged(hProb, COIN_INT_SOLVEMETHOD)) {
		switch (CoinGetIntOption(hProb,COIN_INT_SOLVEMETHOD)) {
			case 0: method = ClpSolve::useDual;				break;
			case 1: method = ClpSolve::usePrimal;			break;
			case 2: method = ClpSolve::usePrimalorSprint;	break;
			case 3: method = ClpSolve::useBarrier;			break;
			case 4: method = ClpSolve::useBarrierNoCross;	break;
			case 5: method = ClpSolve::automatic;			break;
			default: method = ClpSolve::usePrimal;
		}
		pCoin->clp->setSolveType(method);   //ClpSolve::usePrimal
	}

	if (CoinGetOptionChanged(hProb, COIN_INT_PRESOLVETYPE)) {   
		switch (CoinGetIntOption(hProb,COIN_INT_PRESOLVETYPE)) {
			case 0: presolve = ClpSolve::presolveOn;		 break;
			case 1: presolve = ClpSolve::presolveOff;		 break;
			case 2: presolve = ClpSolve::presolveNumber;	 break;
			default: presolve = ClpSolve::presolveOn;
		}
		pCoin->clp_presolve->setPresolveType(presolve);   //ClpSolve::presolveOn
	}
	return 1;
}


int coinSetCbcOptions(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->cbc) {
		return 0;
	}
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPMAXNODES))		pCoin->cbc->setMaximumNodes(CoinGetIntOption(hProb, COIN_INT_MIPMAXNODES));
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPMAXSOL))		pCoin->cbc->setMaximumSolutions(CoinGetIntOption(hProb, COIN_INT_MIPMAXSOL));
	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPMAXSEC))		pCoin->cbc->setDblParam(CbcModel::CbcMaximumSeconds,CoinGetRealOption(hProb, COIN_REAL_MIPMAXSEC));

	if (CoinGetOptionChanged(hProb, COIN_INT_MIPFATHOMDISC))	pCoin->cbc->setIntParam(CbcModel::CbcFathomDiscipline,CoinGetIntOption(hProb, COIN_INT_MIPFATHOMDISC));

	// JPF commented: pCoin->cbc->setHotstartStrategy(CoinGetIntOption(hProb, COIN_INT_MIPHOTSTART));
	//		pCoin->cbc->setForcePriority(CoinGetIntOption(hProb, COIN_INT_MIPFORCEPRIOR));

	if (CoinGetOptionChanged(hProb, COIN_INT_MIPMINIMUMDROP))	pCoin->cbc->setMinimumDrop(CoinGetIntOption(hProb, COIN_INT_MIPMINIMUMDROP));
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPMAXPASSROOT))	pCoin->cbc->setMaximumCutPassesAtRoot(CoinGetIntOption(hProb, COIN_INT_MIPMAXPASSROOT));
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPMAXCUTPASS))	pCoin->cbc->setMaximumCutPasses(CoinGetIntOption(hProb, COIN_INT_MIPMAXCUTPASS));
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPSTRONGBRANCH))	pCoin->cbc->setNumberStrong(CoinGetIntOption(hProb, COIN_INT_MIPSTRONGBRANCH));
	if (CoinGetOptionChanged(hProb, COIN_INT_MIPSCANGLOBCUTS))	pCoin->cbc->setHowOftenGlobalScan(CoinGetIntOption(hProb, COIN_INT_MIPSCANGLOBCUTS));

	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPINTTOL))		pCoin->cbc->setIntegerTolerance(CoinGetRealOption(hProb, COIN_REAL_MIPINTTOL));
	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPINFWEIGHT))	pCoin->cbc->setInfeasibilityWeight(CoinGetRealOption(hProb, COIN_REAL_MIPINFWEIGHT));
	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPCUTOFF))		pCoin->cbc->setDblParam(CbcModel::CbcCutoffIncrement,CoinGetRealOption(hProb, COIN_REAL_MIPCUTOFF));
	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPABSGAP))		pCoin->cbc->setAllowableGap(CoinGetRealOption(hProb, COIN_REAL_MIPABSGAP));
	return 1;
}


int coinSetCglOptions(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	/* see CbcModel.hpp has commments on calling cuts */
	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_PROBING)) {
		pCoin->probing = new CglProbing();
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_MODE))	pCoin->probing->setMode(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MODE));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_USEOBJ))	pCoin->probing->setUsingObjective(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_USEOBJ) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_MAXPASS))	pCoin->probing->setMaxPass(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXPASS));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_MAXPROBE))	pCoin->probing->setMaxProbe(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXPROBE));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_MAXLOOK))	pCoin->probing->setMaxLook(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXLOOK));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPPROBE_ROWCUTS))	pCoin->probing->setRowCuts(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_ROWCUTS));
		pCoin->cbc->addCutGenerator(pCoin->probing,CoinGetIntOption(hProb, COIN_INT_MIPPROBE_FREQ),"Probing");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_GOMORY)) {
		pCoin->gomory = new CglGomory();
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPGOMORY_LIMIT))	pCoin->gomory->setLimit(CoinGetIntOption(hProb, COIN_INT_MIPGOMORY_LIMIT));
		if (CoinGetOptionChanged(hProb, COIN_REAL_MIPGOMORY_AWAY))	pCoin->gomory->setAway(CoinGetRealOption(hProb, COIN_REAL_MIPGOMORY_AWAY));
		pCoin->cbc->addCutGenerator(pCoin->gomory,CoinGetIntOption(hProb, COIN_INT_MIPGOMORY_FREQ),"Gomory");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_KNAPSACK)) {
		pCoin->knapsack = new CglKnapsackCover();
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPKNAPSACK_MAXIN))	pCoin->knapsack->setMaxInKnapsack(CoinGetIntOption(hProb, COIN_INT_MIPKNAPSACK_MAXIN));
		pCoin->cbc->addCutGenerator(pCoin->knapsack,CoinGetIntOption(hProb, COIN_INT_MIPKNAPSACK_FREQ),"Knapsack");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_ODDHOLE)) {
		pCoin->oddhole= new CglOddHole();
		if (CoinGetOptionChanged(hProb, COIN_REAL_MIPODDHOLE_MINVIOL))	pCoin->oddhole->setMinimumViolation(CoinGetRealOption(hProb, COIN_REAL_MIPODDHOLE_MINVIOL));
		if (CoinGetOptionChanged(hProb, COIN_REAL_MIPODDHOLE_MINVIOLPER))	pCoin->oddhole->setMinimumViolationPer(CoinGetRealOption(hProb, COIN_REAL_MIPODDHOLE_MINVIOLPER));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPODDHOLE_MAXENTRIES))	pCoin->oddhole->setMaximumEntries(CoinGetIntOption(hProb, COIN_INT_MIPODDHOLE_MAXENTRIES));
		pCoin->cbc->addCutGenerator(pCoin->oddhole,CoinGetIntOption(hProb, COIN_INT_MIPODDHOLE_FREQ),"OddHole");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_CLIQUE)) {
		pCoin->clique= new CglClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_PACKING) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_STAR))		pCoin->clique->setDoStarClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STAR) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_STARMETHOD))	pCoin->clique->setStarCliqueNextNodeMethod((CglClique::scl_next_node_method)CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARMETHOD));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_STARMAXLEN))	pCoin->clique->setStarCliqueCandidateLengthThreshold(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARMAXLEN));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_STARREPORT))	pCoin->clique->setStarCliqueReport(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARREPORT) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_ROW))		pCoin->clique->setDoRowClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROW) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_ROWMAXLEN))	pCoin->clique->setRowCliqueCandidateLengthThreshold(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROWMAXLEN));
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPCLIQUE_ROWREPORT))	pCoin->clique->setRowCliqueReport(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROWREPORT) ? true : false);
		if (CoinGetOptionChanged(hProb, COIN_REAL_MIPCLIQUE_MINVIOL))	pCoin->clique->setMinViolation(CoinGetRealOption(hProb, COIN_REAL_MIPCLIQUE_MINVIOL));
		pCoin->cbc->addCutGenerator(pCoin->clique,CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_FREQ),"Clique");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_LIFTPROJECT)) {
		pCoin->liftpro = new CglLiftAndProject();
		if (CoinGetOptionChanged(hProb, COIN_INT_MIPLIFTPRO_BETAONE))	pCoin->liftpro->setBeta(CoinGetIntOption(hProb, COIN_INT_MIPLIFTPRO_BETAONE) ? 1 : -1);
		pCoin->cbc->addCutGenerator(pCoin->liftpro,CoinGetIntOption(hProb, COIN_INT_MIPLIFTPRO_FREQ),"LiftProject");
	}

	if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_SIMPROUND)) {
		pCoin->rounding = new CglSimpleRounding();
		pCoin->cbc->addCutGenerator(pCoin->rounding,CoinGetIntOption(hProb, COIN_INT_MIPSIMPROUND_FREQ),"Rounding");
	}
	return 1;
}

/************************************************************************/
/*  SOS Handling                                                        */
/************************************************************************/


int coinSetupSosObject(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;
	int sos, count;
	int* which;
	int type;

	for (sos = 0; sos < pCoin->SosCount; sos++) {
		count = pCoin->SosBegin[sos+1] - pCoin->SosBegin[sos];
		which = &pCoin->SosIndex[pCoin->SosBegin[sos]];
		type = pCoin->SosType[sos];
		CbcObject *sosObject = new CbcSOS(pCoin->cbc, count, which, NULL, 0, type);
		pCoin->cbc->addObjects(1, &sosObject);
		delete sosObject;
	}
	return 1;
}


/************************************************************************/
/*  Optimization                                                        */
/************************************************************************/


extern int CbcOrClpRead_mode;


SOLVAPI int SOLVCALL CoinOptimizeProblem(HPROB hProb, int Method)
{		
	PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) {
		coinSetClpOptions(hProb);
		if (CoinGetOptionChanged(hProb, COIN_INT_PRESOLVETYPE))
			pCoin->clp->initialSolve(*pCoin->clp_presolve);
		else {
			pCoin->clp->initialSolve();
		}
		pCoin->SolutionStatus = pCoin->clp->status();
		}
	else {
#ifdef NEW_STYLE_CBCMAIN
		if (CoinGetIntOption(hProb, COIN_INT_MIPUSECBCMAIN)) {
			//coinSetClpOptions(hProb);
			//coinSetCbcOptions(hProb);
			//coinSetCglOptions(hProb);  BK: CbcMain1 should be calling the Cgl's automatically
			CbcOrClpRead_mode = 1;  // BK: Fix bug in CbcMain1, CbcOrClpRead_mode not initialized  (CpcSolver.cpp, stable 2.2)
			const int argc = 3;
			const char* argv[] = {"CoinMP", "-solve", "-quit"};
			CbcMain1(argc,argv,*pCoin->cbc);
			pCoin->SolutionStatus = pCoin->cbc->status();
			}
		else 
#endif
		{
			coinSetClpOptions(hProb);
			coinSetCbcOptions(hProb);
			coinSetCglOptions(hProb);

			pCoin->cbc->initialSolve();
			pCoin->cbc->branchAndBound();
			pCoin->SolutionStatus = pCoin->cbc->status();
		}
	}	
	return pCoin->SolutionStatus;
}



/************************************************************************/
/*  Solution status                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionStatus(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->SolutionStatus;
}


  

SOLVAPI const char* SOLVCALL CoinGetSolutionText(HPROB hProb, int SolutionStatus)
{
	PCOIN pCoin = (PCOIN)hProb;

	switch (SolutionStatus) {
		case 0:	strcpy(pCoin->SolutionText, "Optimal solution found");		break;
		case 1:	strcpy(pCoin->SolutionText, "Problem primal infeasible");	break;
		case 2:	strcpy(pCoin->SolutionText, "Problem dual infeasible");		break;
		case 3:	strcpy(pCoin->SolutionText, "Stopped on iterations");			break;
		case 4: strcpy(pCoin->SolutionText, "Stopped due to errors");			break;
		case 5: strcpy(pCoin->SolutionText, "Stopped by user");		break;
		default: 
			sprintf(pCoin->SolutionText, "Unknown solution status (%d)", SolutionStatus);
			break;
	}
	return pCoin->SolutionText;
}


SOLVAPI int SOLVCALL CoinGetSolutionTextBuf(HPROB hProb, int SolutionStatus, char* SolutionText, int buflen)
{
	strncpy(SolutionText, CoinGetSolutionText(hProb, SolutionStatus), buflen-1);
	SolutionText[buflen-1] = '\0';
	return (int)strlen(SolutionText);
}



SOLVAPI double SOLVCALL CoinGetObjectValue(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return pCoin->clp->objectiveValue() + pCoin->ObjectConst;
	else {
		return pCoin->cbc->getObjValue() + pCoin->ObjectConst;
	}
}


SOLVAPI double SOLVCALL CoinGetMipBestBound(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return 0;
	else {
		return pCoin->cbc->getBestPossibleObjValue();
	}
}



SOLVAPI int SOLVCALL CoinGetIterCount(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return pCoin->clp->numberIterations();
	else {
		return pCoin->cbc->getIterationCount();
	}
}


SOLVAPI int SOLVCALL CoinGetMipNodeCount(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return 0;
	else {
		return pCoin->cbc->getNodeCount();
	}
}



/************************************************************************/
/*  Solution retrieval                                                  */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
											 double* SlackValues, double* ShadowPrice)
{
   PCOIN pCoin = (PCOIN)hProb;
	const double* columnPrimal;
	const double* columnDual;
	const double* rowPrimal;
	const double* rowDual;
	int i;

	if (pCoin->SolveAsMIP) {
		if (Activity) {
			columnPrimal = pCoin->cbc->solver()->getColSolution();
			for (i = 0; i < pCoin->ColCount; i++) {
				Activity[i] = columnPrimal[i];
			}
			return SOLV_CALL_SUCCESS;
		}
		return SOLV_CALL_FAILED;
	}

	if (Activity) {
		columnPrimal = pCoin->clp->primalColumnSolution();
		for (i = 0; i < pCoin->ColCount; i++) {
			Activity[i] = columnPrimal[i];
		}
	}
	if (ReducedCost) {
		columnDual = pCoin->clp->dualColumnSolution();
		for (i = 0; i < pCoin->ColCount; i++) {
			ReducedCost[i] = columnDual[i];
		}
	}
	if (SlackValues) {
		rowPrimal = pCoin->clp->primalRowSolution();
		for (i = 0; i < pCoin->RowCount; i++) {
			SlackValues[i] = rowPrimal[i];
		}
	}
	if (ShadowPrice) {
		rowDual = pCoin->clp->dualRowSolution();
		for (i = 0; i < pCoin->RowCount; i++) {
			ShadowPrice[i] = rowDual[i];
		}
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinGetSolutionRanges(HPROB hProb, double* ObjLoRange, double* ObjUpRange,
											 double* RhsLoRange, double* RhsUpRange)
{
	return SOLV_CALL_FAILED;
}



SOLVAPI int SOLVCALL CoinGetSolutionBasis(HPROB hProb, int* ColStatus, double* RowStatus)
{
	return SOLV_CALL_FAILED;
}



/************************************************************************/
/*  File Handling                                                       */
/************************************************************************/


SOLVAPI int SOLVCALL CoinReadFile(HPROB hProb, int FileType, const char* ReadFilename)
{
   PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		
			pCoin->clp->readMps(ReadFilename, 1, 0);    
			break;

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_CALL_FAILED;
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinWriteFile(HPROB hProb, int FileType, const char* WriteFilename)
{
	PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		
			pCoin->clp->writeMps(WriteFilename);   
			break;

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_CALL_FAILED;

	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinOpenLogFile(HPROB hProb, const char* logFilename)
{
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinCloseLogFile(HPROB hProb)
{
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Option Table                                                        */
/************************************************************************/

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


#define OPTIONCOUNT    67



SOLVOPTINFO OptionTable[OPTIONCOUNT] = {

	  "SolveMethod",			    "SolveMethod",  GRP_SIMPLEX,        0,        0,      0,       5,  OPT_LIST,   0,   COIN_INT_SOLVEMETHOD,
      /*enum SolveType { useDual=0, usePrimal, usePrimalorSprint, useBarrier, useBarrierNoCross, automatic*/
	  "PresolveType",	   	    "Presolve",     GRP_SIMPLEX,        0,        0,      0,       2,  OPT_LIST,   0,   COIN_INT_PRESOLVETYPE,
		/*enum PresolveType { presolveOn=0, presolveOff, presolveNumber }; */
	  "Scaling",       		 	 "Scaling",      GRP_SIMPLEX,        3,        3,      0,       4,  OPT_LIST,   0,   COIN_INT_SCALING, 
      /* Sets or unsets scaling, 0 -off, 1 equilibrium, 2 geometric, 3, auto, 4 dynamic(later) */
     "Perturbation",   			 "Perturb",      GRP_SIMPLEX,      100,      100,      0,     100,  OPT_INT,    0,   COIN_INT_PERTURBATION, 
		/* 0 - Off, 50 - On, 100 - Only if clp stalls */
	  "PrimalColPivotAlg",		 "PrimPivAlg",   GRP_SIMPLEX,        1,        1,      0,       4,  OPT_LIST,   0,   COIN_INT_PRIMALPIVOTALG, 
      /*0 is exact devex, 1 full steepest, 2 is partial exact devex
      3 switches between 0 and 2 depending on factorization
      4 starts as partial dantzig/devex but then may switch between 0 and 2.*/
	  "DualRowPivotAlg",	    	 "DualPivAlg",   GRP_SIMPLEX,        1,        1,      0,       3,  OPT_LIST,   0,   COIN_INT_DUALPIVOTALG, 
      /*0 is uninitialized, 1 full, 2 is partial uninitialized,
      3 starts as 2 but may switch to 1.*/
	  "LogLevel",               "LogLevel",     GRP_LOGFILE,        1,        1,      0,       4,  OPT_LIST,   0,   COIN_INT_LOGLEVEL, 
	  "MaxIterations",			 "MaxIter",      GRP_LIMITS,  99999999, 99999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MAXITER, 
     "MaxSeconds",	    		 "MaxSeconds",   GRP_LIMITS,      -1.0,     -1.0,   -1.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MAXSECONDS, 

     "CrashInd",	           	 "CrashInd",     GRP_SIMPLEX,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_CRASHIND, 
     "CrashPivot",	        	 "CrashPivot",   GRP_SIMPLEX,        0,        0,     -1,       2,  OPT_LIST,   0,   COIN_INT_CRASHPIVOT, 
     "CrashGap",	          	 "CrashGap",     GRP_SIMPLEX,   1000.0,   1000.0,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_CRASHGAP, 
     "PrimalObjectLimit",		 "PrimalObjLim", GRP_SIMPLEX,  MAXREAL,  MAXREAL,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_PRIMALOBJLIM, 
     "DualObjectLimit",			 "DualObjLim",   GRP_SIMPLEX,  MAXREAL,  MAXREAL,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_DUALOBJLIM, 
     "PrimalTolerance",	       "PrimalTol",    GRP_SIMPLEX,     1e-7,     1e-7,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_PRIMALOBJTOL, 
     "DualTolerance",	       "DualTol",      GRP_SIMPLEX,     1e-7,     1e-7,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_DUALOBJTOL, 

     "MipMaxNodes",            "MipMaxNodes",  GRP_LIMITS,   9999999,  9999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXNODES, 
	  "MipMaxSolutions",        "MipMaxSol",    GRP_LIMITS,   9999999,  9999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXSOL, 
	  "MipMaxSeconds",          "MipMaxSec",    GRP_LIMITS,     1e100,    1e100,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPMAXSEC, 

	  "MipFathomDiscipline",    "FathomDisc",   GRP_MIPSTRAT,       0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPFATHOMDISC, 
	  "MipHotStart",            "HotStart",     GRP_MIPSTRAT,       0,        0,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPHOTSTART,
//	  "MipForcePriority",       "ForcePrior",   GRP_MIPSTRAT,      -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPFORCEPRIOR,
	  "MipMinimumDrop",         "MinimumDrop",  GRP_MIPSTRAT,    1e-4,     1e-4,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMINIMUMDROP,
	  "MipMaxCutPasses",        "MaxCutPass",   GRP_MIPSTRAT,       2,        2,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXCUTPASS,
	  "MipMaxCutPassAtRoot",    "MaxPassRoot",  GRP_MIPSTRAT,      50,       50,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXPASSROOT,
	  "MipStrongBranching",     "StrongBranch", GRP_MIPSTRAT,       5,        5,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSTRONGBRANCH,
	  "MipScanGlobalCuts",      "ScanGlobCuts", GRP_MIPSTRAT,       1,        1,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSCANGLOBCUTS,

	  "MipIntegerTolerance",    "MipIntTol",    GRP_MIPTOL,      1e-6,     1e-6,    0.0,     1.0,  OPT_REAL,   0,   COIN_REAL_MIPINTTOL, 
	  "MipInfeasWeight",        "MipInfWeight", GRP_MIPTOL,       0.0,      0.0,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPINFWEIGHT, 
	  "MipCutoffIncrement",     "MipCutIncr",   GRP_MIPTOL,      1e-5,     1e-5,    0.0,     1.0,  OPT_REAL,   0,   COIN_REAL_MIPCUTOFF, 
	  "MipAllowableGap",        "MipAbsGap",    GRP_MIPTOL,     1e-10,    1e-10,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPABSGAP, 

	  /* Probing */
	  "MipCutProbing",          "CutProbing",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_PROBING,
	  "MipProbeFrequency",      "ProbeFreq",    GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_FREQ,
	  "MipProbeMode",           "ProbeMode",    GRP_MIPCUTS,        1,        1,      0,       2,  OPT_LIST,   0,   COIN_INT_MIPPROBE_MODE,
	  "MipProbeUseObjective",   "UseObject",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPPROBE_USEOBJ,
	  "MipProbeMaxPass",        "MaxPass",      GRP_MIPCUTS,        3,        3,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXPASS,
	  "MipProbeMaxProbe",       "MaxProbe",     GRP_MIPCUTS,      100,      100,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXPROBE,
	  "MipProbeMaxLook",        "MaxLook",      GRP_MIPCUTS,       50,       50,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXLOOK,
	  "MipProbeRowCuts",        "RowCuts",      GRP_MIPCUTS,        3,        3,      0,       3,  OPT_LIST,   0,   COIN_INT_MIPPROBE_ROWCUTS,

	  /* Gomory cuts */
  	  "MipCutGomory",           "CutGomory",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_GOMORY,
	  "MipGomoryFrequency",     "GomoryFreq",   GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPGOMORY_FREQ,
	  "MipGomoryLimit",         "GomoryLimit",  GRP_MIPCUTS,       50,       50,      1,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPGOMORY_LIMIT,
	  "MipGomoryAway",          "GomoryAway",   GRP_MIPCUTS,     0.05,     0.05, 0.0001,     0.5,  OPT_REAL,   0,   COIN_REAL_MIPGOMORY_AWAY,

	  /* Knapsack cuts */
	  "MipCutKnapsack",         "CutKnapsack",  GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_KNAPSACK,
	  "MipKnapsackFrequency",   "KrapsackFreq", GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPKNAPSACK_FREQ,
	  "MipKnapsackMaxIn",       "KnapsackMaxIn",GRP_MIPCUTS,       50,       50,      1,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPKNAPSACK_MAXIN,

	  /* Oddhole cuts */
	  "MipCutOddHole",          "CutOddHole",   GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_ODDHOLE,
	  "MipOddHoleFrequency",    "OddHoleFreq",  GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPODDHOLE_FREQ,
	  "MipOddHoleMinViolation", "MinViolation", GRP_MIPCUTS,    0.001,    0.001,1.01e-8,     0.5,  OPT_REAL,   0,   COIN_REAL_MIPODDHOLE_MINVIOL,
	  "MipOddHoleMinViolPer",   "MinViolPer",   GRP_MIPCUTS,   0.0003,   0.0003,1.01e-8,    0.25,  OPT_REAL,   0,   COIN_REAL_MIPODDHOLE_MINVIOLPER,
	  "MipOddHoleMaxEntries",   "MaxEntries",   GRP_MIPCUTS,      100,      100,      3,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPODDHOLE_MAXENTRIES,

	  /* Clique cuts */
	  "MipCutClique",           "CutClique",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_CLIQUE,
	  "MipCliqueFrequency",     "CliqueFreq",   GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_FREQ,
	  "MipCliquePacking",       "CliquePacking",GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_PACKING,
	  "MipCliqueStar",          "CliqueStar",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_STAR,
	  "MipCliqueStarMethod",    "StarMethod",   GRP_MIPCUTS,        2,        2,      0,       2,  OPT_LIST,   0,   COIN_INT_MIPCLIQUE_STARMETHOD,
	  "MipCliqueStarMaxLen",    "StarMaxLen",   GRP_MIPCUTS,       12,       12,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_STARMAXLEN,
	  "MipCliqueStarReport",    "StarReport",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_STARREPORT,
	  "MipCliqueRow",           "CliqueRow",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_ROW,
	  "MipCliqueRowMaxLen",     "ClqRowMaxLen", GRP_MIPCUTS,       12,       12,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_ROWMAXLEN,
	  "MipCliqueRowReport",     "ClqRowReport", GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_ROWREPORT,
	  "MipCliqueMinViolation",  "ClqMinViol",   GRP_MIPCUTS,     -1.0,     -1.0,-MAXREAL,MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPCLIQUE_MINVIOL,

	  /* Lift and Project */
	  "MipCutLiftAndProject",   "CutLiftPro",   GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_LIFTPROJECT,
	  "MipLiftCoverFrequency",  "LiftProFreq",  GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPLIFTPRO_FREQ,
	  "MipLiftCoverBetaOne",    "LiftProBeta",  GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPLIFTPRO_BETAONE,

	  /* Simple Rounding */
	  "MipCutSimpleRounding",   "CutSimpRound", GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_SIMPROUND,
	  "MipSimpleRoundFrequency","SimpRoundFreq",GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSIMPROUND_FREQ,

	  "MipUseCbcMain",          "UseCbcMain",   GRP_MIPSTRAT,       1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPUSECBCMAIN
	};



/************************************************************************/
/*  Option Handling                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetOptionCount(HPROB hProb)
{
	return OPTIONCOUNT;
}


SOLVAPI int SOLVCALL CoinGetOptionInfo(HPROB hProb, int OptionNr, int* OptionID, int* GroupType, int* OptionType)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return SOLV_CALL_FAILED;
	}
	if (OptionID)   *OptionID = OptionTable[OptionNr].OptionID;
	if (OptionType) *OptionType = OptionTable[OptionNr].OptionType;
	if (GroupType)  *GroupType = OptionTable[OptionNr].GroupType;
	return SOLV_CALL_SUCCESS;
}


SOLVAPI const char* SOLVCALL CoinGetOptionName(HPROB hProb, int OptionNr)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return "";
	}
	return OptionTable[OptionNr].OptionName;
}


SOLVAPI int SOLVCALL CoinGetOptionNameBuf(HPROB hProb, int OptionNr, char* OptionName, int buflen)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return SOLV_CALL_FAILED;
	}
	if (OptionName) {
		strncpy(OptionName, CoinGetOptionName(hProb, OptionNr), buflen-1);
		OptionName[buflen-1] = '\0';
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI const char* SOLVCALL CoinGetOptionShortName(HPROB hProb, int OptionNr)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return "";
	}
	return OptionTable[OptionNr].ShortName;
}


SOLVAPI int SOLVCALL CoinGetOptionShortNameBuf(HPROB hProb, int OptionNr, char* ShortName, int buflen)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return SOLV_CALL_FAILED;
	}
	if (ShortName) {
		strncpy(ShortName, CoinGetOptionShortName(hProb, OptionNr), buflen-1);
		ShortName[buflen-1] = '\0';
	}
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int* MinValue, int* MaxValue)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return SOLV_CALL_FAILED;
	}
	if (MinValue)   *MinValue = ROUND(OptionTable[OptionNr].MinValue);
	if (MaxValue)   *MaxValue = ROUND(OptionTable[OptionNr].MaxValue);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double* MinValue, double* MaxValue)
{
	if ((OptionNr < 0) || (OptionNr >= OPTIONCOUNT)) {
		return SOLV_CALL_FAILED;
	}
	if (MinValue)   *MinValue = OptionTable[OptionNr].MinValue;
	if (MaxValue)   *MaxValue = OptionTable[OptionNr].MaxValue;
	return SOLV_CALL_SUCCESS;
}


int coinLocateOptionID(int OptionID)
{
	int i;

	for (i = 0; i < OPTIONCOUNT; i++) {
		if (OptionID == OptionTable[i].OptionID) {
			return i;
		}
	}
	return -1;
}


SOLVAPI int SOLVCALL CoinGetOptionChanged(HPROB hProb, int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(OptionID);
	if (OptionNr < 0) {
		return 0;
	}
	return OptionTable[OptionNr].changed;
}


SOLVAPI int SOLVCALL CoinGetIntOption(HPROB hProb,int OptionID)
{   
	int OptionNr;

	OptionNr = coinLocateOptionID(OptionID);
	if (OptionNr < 0) {
		return 0;
	}
	if (OptionTable[OptionNr].OptionType != OPT_REAL)
		return (int)OptionTable[OptionNr].CurrentValue;
	else {
		return 0;
	}
}
  	


SOLVAPI int SOLVCALL CoinSetIntOption(HPROB hProb,int OptionID, int IntValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(OptionID);
	if (OptionNr < 0) {
		return SOLV_CALL_FAILED;
	}
	if (OptionTable[OptionNr].OptionType != OPT_REAL) {
		coinWriteMsgLog("%s[%d] = %d (was %d)",OptionTable[OptionNr].OptionName, OptionNr, IntValue, (int)OptionTable[OptionNr].CurrentValue);
		OptionTable[OptionNr].CurrentValue = IntValue;
		OptionTable[OptionNr].changed = 1;
	   return SOLV_CALL_SUCCESS;
	}
	return SOLV_CALL_FAILED;
}



SOLVAPI double SOLVCALL CoinGetRealOption(HPROB hProb,int OptionID)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(OptionID);
	if (OptionNr < 0) {
		return 0.0;
	}
	if (OptionTable[OptionNr].OptionType == OPT_REAL) {
		return OptionTable[OptionNr].CurrentValue;
	}
	return 0.0;
}



SOLVAPI int SOLVCALL CoinSetRealOption(HPROB hProb,int OptionID, double RealValue)
{
	int OptionNr;

	OptionNr = coinLocateOptionID(OptionID);
	if (OptionNr < 0) {
		return SOLV_CALL_FAILED;
	}
	if (OptionTable[OptionNr].OptionType == OPT_REAL) {
		coinWriteMsgLog("%s[%d] = %lg (was %lg)",OptionTable[OptionNr].OptionName, OptionNr, RealValue, OptionTable[OptionNr].CurrentValue);
		OptionTable[OptionNr].CurrentValue = RealValue;
		OptionTable[OptionNr].changed = 1;
	   return SOLV_CALL_SUCCESS;
	}
	return SOLV_CALL_FAILED;
}


SOLVAPI const char* SOLVCALL CoinGetStringOption(HPROB hProb, int OptionID)
{
   return "";
}


SOLVAPI int SOLVCALL CoinGetStringOptionBuf(HPROB hProb, int OptionID, char* StringValue, int buflen)
{
   return SOLV_CALL_FAILED;
}


SOLVAPI int SOLVCALL CoinSetStringOption(HPROB hProb, int OptionID, const char* StringValue)
{
   return SOLV_CALL_FAILED;
}



/************************************************************************/
/*  DLL Handling                                                        */
/************************************************************************/


#if defined(_MSC_VER)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


static HINSTANCE g_hInstance;


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
   switch (dwReason) {

      case DLL_PROCESS_ATTACH:
         g_hInstance = hInstance;
		   break;

      case DLL_PROCESS_DETACH:
	      break;
   }
   return 1;
}
#endif



//double
// DualTolerance
// PrimalTolerance
// DualBound
// PrimalWeight (setInfeasibilityCost)
// MaxTime      (setMaximumSeconds)
// ObjScale     (setObjectiveScale)
// RhsScale     (setRhsScale)

//int
//LogLevel      (setLogLovel/model->factorization()->messageLevel(8);)
//MaxFactor     (model->factorization()->maximumPivots(value);
//PertValue     (setPerturbation)
//MaxIteration  (setMaximumIterations)

//AutoScale     off
//Barrier
//BiasLU        UU  (UX, LX, LL)
//Crash         off
//Direction     min
//Directory
//dualBound     1e-20, 1e12
//dualPivot     Automatic, (Dantzig, partial, Steepest)
//DualSimplex
//dualTolerance 1e-20,1e12
//Exit
//ErrorsAllowed
//FakeBound
//Help
//Idiot
//Import
//KeepNames
//LogLevel
//Maximize
//MaxFactor     1, 999999
//MaxIteration  0, 99999999
//Messages
//ObjScale      -1e20, 1e20
//PresolvePass  0, 100
//PertValue     -5000, 102, false
//Perturbation  On, off
//PlusMinus     -1, false
//Presolve      On
//PrimalPivot     Automatic, (Dantzig, partial, Steepest, change, sprint)
//PrimalSimplex
//PrimalTolerance 1e-20,1e12
//PrimalWeight    1e20, 1e20
//RhsScale        -1e20, 1.20
//Scaling         Off, Equilibrium, geometric, automatic
//MaxTime         0.0, 1e12
//SparseFactor    On, Off
//SprintCrash     -1, 500

