/************************************************************************/
/*                                                                      */
/*  CoinMP Solver                                     Maximal Software  */
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

// $Id$

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

#include "CoinProblem.h"
#include "CoinResult.h"
#include "CoinSolver.h"


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

				PPROBLEM  pProblem;
				PRESULT   pResult;
				PSOLVER   pSolver;

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

	pCoin->pProblem = coinCreateProblemObject();
	pCoin->pResult = coinCreateResultObject();
	pCoin->pSolver = coinCreateSolverObject();

	coinSetProblemName(pCoin->pProblem, ProblemName);

	return (HPROB)pCoin;
}



void coinCopyNamesList(PPROBLEM pProblem, PCOIN pCoin)
{
	int i;

	/* Load names */
	if (pProblem->RowNamesList || pProblem->ColNamesList) {
		std::vector<std::string> rowNamesVect;
		std::vector<std::string> colNamesVect;
		rowNamesVect.reserve(pProblem->RowCount);
		colNamesVect.reserve(pProblem->ColCount);
		if (pProblem->RowNamesList) {
			for (i = 0; i < pProblem->RowCount; i++) {
				rowNamesVect.push_back(pProblem->RowNamesList[i]);
			}
		}
		if (pProblem->ColNamesList) {
			for (i = 0; i < pProblem->ColCount; i++) {
				colNamesVect.push_back(pProblem->ColNamesList[i]);
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
	PPROBLEM pProblem = pCoin->pProblem;

	if (ColCount == 0) {
		return SOLV_CALL_FAILED;
	}
	pProblem->ColCount = ColCount;
	pProblem->RowCount = RowCount;
	pProblem->NZCount = NZCount;
	pProblem->RangeCount = RangeCount;
	pProblem->ObjectSense = ObjectSense;
	pProblem->ObjectConst = ObjectConst;

	if (ColNames)   pProblem->lenColNamesBuf = coinGetLenNameListBuf(ColNames, ColCount);
	if (RowNames)   pProblem->lenRowNamesBuf = coinGetLenNameListBuf(RowNames, RowCount);
	if (ObjectName) pProblem->lenObjNameBuf  = (int)strlen(ObjectName) + 1;

	if (ObjectCoeffs) pProblem->ObjectCoeffs = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (RHSValues)    pProblem->RHSValues    = (double*) malloc(pProblem->RowCount     * sizeof(double));
	if (RangeValues)  pProblem->RangeValues  = (double*) malloc(pProblem->RowCount     * sizeof(double));
	if (RowType)      pProblem->RowType      = (char*)   malloc(pProblem->RowCount     * sizeof(char));
	if (MatrixBegin)  pProblem->MatrixBegin  = (int*)    malloc((pProblem->ColCount+1) * sizeof(int));
	if (MatrixCount)  pProblem->MatrixCount  = (int*)    malloc(pProblem->ColCount     * sizeof(int));
	if (MatrixIndex)  pProblem->MatrixIndex  = (int*)    malloc(pProblem->NZCount      * sizeof(int)); 
	if (MatrixValues) pProblem->MatrixValues = (double*) malloc(pProblem->NZCount      * sizeof(double));
	if (LowerBounds)  pProblem->LowerBounds  = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (UpperBounds)  pProblem->UpperBounds  = (double*) malloc(pProblem->ColCount     * sizeof(double));
	if (ColNames)     pProblem->ColNamesList = (char**)  malloc(pProblem->ColCount     * sizeof(char* ));
	if (RowNames)     pProblem->RowNamesList = (char**)  malloc(pProblem->RowCount     * sizeof(char* ));
	if (ColNames)     pProblem->ColNamesBuf  = (char*)   malloc(pProblem->lenColNamesBuf * sizeof(char));
	if (RowNames)     pProblem->RowNamesBuf  = (char*)   malloc(pProblem->lenRowNamesBuf * sizeof(char));
	if (ObjectName)   pProblem->ObjectName   = (char*)   malloc(pProblem->lenObjNameBuf  * sizeof(char));

	if (pProblem->RowCount > 0) {
		pProblem->RowLower = (double* )malloc(pProblem->RowCount*sizeof(double));
		pProblem->RowUpper = (double* )malloc(pProblem->RowCount*sizeof(double));
		if (!pProblem->RowLower || !pProblem->RowUpper) {
			return SOLV_CALL_FAILED;
		}
	}

	if (pProblem->ObjectCoeffs) memcpy(pProblem->ObjectCoeffs, ObjectCoeffs, pProblem->ColCount     * sizeof(double));
	if (pProblem->RHSValues)    memcpy(pProblem->RHSValues,    RHSValues,    pProblem->RowCount     * sizeof(double));
	if (pProblem->RangeValues)  memcpy(pProblem->RangeValues,  RangeValues,  pProblem->RowCount     * sizeof(double));
	if (pProblem->RowType)      memcpy(pProblem->RowType,      RowType,      pProblem->RowCount     * sizeof(char));
	if (pProblem->MatrixBegin)  memcpy(pProblem->MatrixBegin,  MatrixBegin,  (pProblem->ColCount+1) * sizeof(int));
	if (pProblem->MatrixCount)  memcpy(pProblem->MatrixCount,  MatrixCount,  pProblem->ColCount     * sizeof(int));
	if (pProblem->MatrixIndex)  memcpy(pProblem->MatrixIndex,  MatrixIndex,  pProblem->NZCount      * sizeof(int));
	if (pProblem->MatrixValues) memcpy(pProblem->MatrixValues, MatrixValues, pProblem->NZCount      * sizeof(double));
	if (pProblem->LowerBounds)  memcpy(pProblem->LowerBounds,  LowerBounds,  pProblem->ColCount     * sizeof(double));
	if (pProblem->UpperBounds)  memcpy(pProblem->UpperBounds,  UpperBounds,  pProblem->ColCount     * sizeof(double));
	if (pProblem->ObjectName)   memcpy(pProblem->ObjectName,   ObjectName,   pProblem->lenObjNameBuf  * sizeof(char));

	coinCopyNamesList(pProblem->ColNamesList, pProblem->ColNamesBuf, ColNames, ColCount);
	coinCopyNamesList(pProblem->RowNamesList, pProblem->RowNamesBuf, RowNames, RowCount);

	pCoin->clp->setOptimizationDirection(ObjectSense);

	if (!coinComputeRowLowerUpper(pProblem, COIN_DBL_MAX)) {
		return SOLV_CALL_FAILED;
	}

	pCoin->clp->loadProblem(ColCount, RowCount, MatrixBegin, MatrixIndex, MatrixValues,
							LowerBounds, UpperBounds, ObjectCoeffs, pProblem->RowLower, pProblem->RowUpper);

	coinCopyNamesList(pProblem, pCoin);

	return SOLV_CALL_SUCCESS;
}

// CoinLoadProblemBuf accepts ColNames/RowNames as character buffer with \0 after each name

SOLVAPI int SOLVCALL CoinLoadProblemBuf(HPROB hProb, 
				int ColCount, int RowCount, int NZCount, int RangeCount, 
				int ObjectSense, double ObjectConst, double* ObjectCoeffs, 
				double* LowerBounds, double* UpperBounds, char* RowType, 
				double* RHSValues, double* RangeValues, int* MatrixBegin, 
				int* MatrixCount, int* MatrixIndex, double* MatrixValues, 
				char* ColNamesBuf, char* RowNamesBuf, char* ObjectName)
{
	PCOIN pCoin = (PCOIN)hProb;
	char** ColNamesList;
	char** RowNamesList;
	int result;

	ColNamesList = (char**)malloc(ColCount * sizeof(char*));
	RowNamesList = (char**)malloc(RowCount * sizeof(char*));
	coinSetupNamesList(ColNamesList, ColNamesBuf, ColCount);
	coinSetupNamesList(RowNamesList, RowNamesBuf, RowCount);
	result = CoinLoadProblem(hProb, ColCount, RowCount, NZCount, RangeCount,
				ObjectSense, ObjectConst, ObjectCoeffs, LowerBounds, UpperBounds, RowType,
				RHSValues, RangeValues, MatrixBegin, MatrixCount, MatrixIndex, MatrixValues,
				ColNamesList, RowNamesList, ObjectName);
	if (ColNamesList) free(ColNamesList);
	if (RowNamesList) free(RowNamesList);
	return result;
}


SOLVAPI int SOLVCALL CoinLoadInitValues(HPROB hProb, double* InitValues)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (InitValues)   pProblem->InitValues   = (double* ) malloc(pProblem->ColCount     * sizeof(double));

	if (pProblem->InitValues)   memcpy(pProblem->InitValues,   InitValues,   pProblem->ColCount     * sizeof(double));
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinLoadInteger(HPROB hProb, char* ColType)
{   
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	int i;

	if (pProblem->ColCount == 0) {
		return SOLV_CALL_FAILED;
	}
	if (ColType) {
		pProblem->ColType = (char* )malloc(pProblem->ColCount * sizeof(char));
		if (!pProblem->ColType) {
			return SOLV_CALL_FAILED;
		}
		memcpy(pProblem->ColType, ColType, pProblem->ColCount * sizeof(char));
	}
	pProblem->IsInt = (char* )malloc(pProblem->ColCount*sizeof(char));
	if (!pProblem->IsInt) {
		return SOLV_CALL_FAILED;
	}
	for (i = 0; i < pProblem->ColCount; i++ ) {
		switch (ColType[i]) {
			case 'B': 
				pProblem->BinCount++;
				pProblem->IsInt[i] = 1;
				pProblem->SolveAsMIP = 1;
				break;

			case 'I': 
				pProblem->IntCount++;
				pProblem->IsInt[i] = 1;
				pProblem->SolveAsMIP = 1;
				break;

			case 'C':
				pProblem->IsInt[i] = 0;
				break;

			default:
				pProblem->IsInt[i] = 0;
				return SOLV_CALL_FAILED;
		}
	}
	if (pProblem->SolveAsMIP) {
		if (!pCoin->cbc) {
			pCoin->cbc = new CbcModel(*pCoin->osi);
		}
		for (i = 0; i < pProblem->ColCount; i++) {
			if (pProblem->IsInt[i]) {
				pCoin->cbc->solver()->setInteger(i);
				pCoin->osi->setInteger(i);
				pProblem->numInts++;
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
	PPROBLEM pProblem = pCoin->pProblem;
	int *priorVar;
	int *priorCbc;
	int i,k;

	pProblem->PriorCount = PriorCount;
	if (PriorIndex)  pProblem->PriorIndex  = (int* )malloc(PriorCount * sizeof(int));
	if (PriorValues) pProblem->PriorValues = (int* )malloc(PriorCount * sizeof(int));
	if (BranchDir)	 pProblem->BranchDir   = (int* )malloc(PriorCount * sizeof(int));
	if (pProblem->PriorIndex)  memcpy(pProblem->PriorIndex,  PriorIndex,  PriorCount * sizeof(int));
	if (pProblem->PriorValues) memcpy(pProblem->PriorValues, PriorValues, PriorCount * sizeof(int));
	if (pProblem->BranchDir)   memcpy(pProblem->BranchDir,   BranchDir,   PriorCount * sizeof(int));

	if (!pProblem->SolveAsMIP) {
		return SOLV_CALL_FAILED;
	}
	if (!pProblem->IsInt) {
		return SOLV_CALL_FAILED;
	}

	priorVar = (int *)malloc(pProblem->ColCount * sizeof(int));
	if (!priorVar) {
		return SOLV_CALL_FAILED;
	}
	//reset the priorVar
	for (i = 0; i < pProblem->ColCount; i++) {
		priorVar[i] = 1000;
	}
	for (i = 0; i < PriorCount; i++) {
		if ((PriorIndex[i] < 0) || (PriorIndex[i] >= pProblem->ColCount)) {
			free(priorVar);
			return SOLV_CALL_FAILED;
		}
		priorVar[PriorIndex[i]] = PriorValues[i];
	}
	//Create an array to give to cbc
	priorCbc = (int *)malloc(pProblem->numInts * sizeof(int));
	if (!priorCbc) {
		free(priorVar);
		return SOLV_CALL_FAILED;
	}
	k = 0;
	for (i = 0; i < pProblem->ColCount; i++) {
		if (pProblem->IsInt[i]) {
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
	PPROBLEM pProblem = pCoin->pProblem;
	int sos, count;
	int* which;
	int type;

	if ((SosCount == 0) || (SosNZCount == 0)) {
		return SOLV_CALL_FAILED;
	}
	pProblem->SosCount = SosCount;
	pProblem->SosNZCount = SosNZCount;

	if (SosType)  pProblem->SosType  = (int* )malloc(SosCount     * sizeof(int));
	if (SosPrior) pProblem->SosPrior = (int* )malloc(SosCount     * sizeof(int));
	if (SosBegin) pProblem->SosBegin = (int* )malloc((SosCount+1) * sizeof(int));
	if (SosIndex) pProblem->SosIndex = (int* )malloc(SosNZCount   * sizeof(int));
	if (SosRef)   pProblem->SosRef   = (double* )malloc(SosNZCount* sizeof(double));

	if (pProblem->SosType)  memcpy(pProblem->SosType,  SosType,  SosCount     * sizeof(int));
	if (pProblem->SosPrior) memcpy(pProblem->SosPrior, SosPrior, SosCount     * sizeof(int));
	if (pProblem->SosBegin) memcpy(pProblem->SosBegin, SosBegin, (SosCount+1) * sizeof(int));
	if (pProblem->SosIndex) memcpy(pProblem->SosIndex, SosIndex, SosNZCount   * sizeof(int));
	if (pProblem->SosRef)   memcpy(pProblem->SosRef,   SosRef,   SosNZCount   * sizeof(double));

	pProblem->SolveAsMIP = 1;
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

		coinClearProblemObject(pCoin->pProblem);
		coinClearResultObject(pCoin->pResult);
	}
	free(pCoin);
	pCoin = NULL;
	return SOLV_CALL_SUCCESS;
}



#define CHECK_MAXNAMELEN   100


SOLVAPI int SOLVCALL CoinCheckProblem(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	int i;

	if (pProblem->ColCount == 0) {
		return SOLV_CHECK_COLCOUNT;
	}
	if ((pProblem->RowCount < 0) ||
		(pProblem->NZCount < 0) ||
		(pProblem->RangeCount < 0)) {
		return SOLV_CHECK_ROWCOUNT;
	}
	if ((pProblem->RangeCount > pProblem->RowCount)) {
		return SOLV_CHECK_RANGECOUNT;
	}
	if ((pProblem->ObjectSense < -1) || 
		(pProblem->ObjectSense > 1)) {
		return SOLV_CHECK_OBJSENSE;
	}
	if (pProblem->RowType && (pProblem->RowCount > 0)) {
		for (i = 0; i < pProblem->RowCount; i++) {
			if ((pProblem->RowType[i] != 'L') &&
				(pProblem->RowType[i] != 'E') &&
				(pProblem->RowType[i] != 'G') &&
				(pProblem->RowType[i] != 'R') &&
				(pProblem->RowType[i] != 'N')) {
				return SOLV_CHECK_ROWTYPE;
			}
		}
	}
	if (pProblem->NZCount > 0) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if (pProblem->MatrixBegin[i] < 0) {
				return SOLV_CHECK_MATBEGIN;
			}
			if (pProblem->MatrixCount[i] < 0) {
				return SOLV_CHECK_MATCOUNT;
			}
			if (pProblem->MatrixBegin[i+1] - pProblem->MatrixBegin[i] != pProblem->MatrixCount[i]) {
				return SOLV_CHECK_MATBEGCNT;
			}
		}
		if (pProblem->MatrixBegin[pProblem->ColCount] != pProblem->NZCount) {
			return 100+pProblem->MatrixBegin[pProblem->ColCount];
		}
		for (i = 0; i < pProblem->NZCount; i++) {
			if (pProblem->MatrixIndex[i] < 0) {
				return SOLV_CHECK_MATINDEX;
			}
			if (pProblem->MatrixIndex[i] >= pProblem->RowCount) {
				return SOLV_CHECK_MATINDEXROW;
			}
		}
	}
	if (pProblem->LowerBounds && pProblem->UpperBounds) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if (pProblem->LowerBounds[i] > pProblem->UpperBounds[i]) {
				return SOLV_CHECK_BOUNDS;
			}
		}
	}
	if (pProblem->ColType) {
		for (i = 0; i < pProblem->ColCount; i++) {
			if ((pProblem->ColType[i] != 'C') &&
				(pProblem->ColType[i] != 'B') &&
				(pProblem->ColType[i] != 'I')) {
				return SOLV_CHECK_COLTYPE;
			}
		}
	}
	if (pProblem->ColNamesBuf) {
		if (pProblem->lenColNamesBuf <= 0) {
			return SOLV_CHECK_COLNAMES;
		}
		if (pProblem->lenColNamesBuf > pProblem->ColCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_COLNAMESLEN;
		}
	}
	if (pProblem->RowNamesBuf) {
		if (pProblem->lenRowNamesBuf <= 0) {
			return SOLV_CHECK_ROWNAMES;
		}
		if (pProblem->lenRowNamesBuf > pProblem->RowCount * CHECK_MAXNAMELEN) {
			return SOLV_CHECK_ROWNAMSLEN;
		}
	}
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Problem Queries                                                     */
/************************************************************************/


SOLVAPI const char* SOLVCALL CoinGetProblemName(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->ProblemName;
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

	return pCoin->pProblem->ColCount;
}


SOLVAPI int SOLVCALL CoinGetRowCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->RowCount;
}



SOLVAPI const char* SOLVCALL CoinGetColName(HPROB hProb, int col)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pProblem->ColNamesList[col];
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

	return pCoin->pProblem->RowNamesList[row];
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
	if (global_pCoin->pSolver->MsgLogCallback) {
		global_pCoin->pSolver->MsgLogCallback(strbuf);
	}
	return SOLV_CALL_SUCCESS;
}


int coinIterLogCallback(int IterCount, double ObjectValue, int IsFeasible, double InfeasValue)
{
	if (!global_pCoin->pProblem->SolveAsMIP) {
		if (((IterCount < 100) && ((IterCount % 10) == 0)) ||
			 ((IterCount % 100) == 0)) {
			if (!IsFeasible)
				coinWriteMsgLog("Iteration: %5d  %16.8lg  %16.8lg",IterCount, ObjectValue, InfeasValue);
			else {
				coinWriteMsgLog("Iteration: %5d  %16.8lg",IterCount, ObjectValue);
			}
		}
	}
	if (global_pCoin->pSolver->IterCallback) {
		global_pCoin->pSolver->IterCallback(IterCount, ObjectValue, IsFeasible, InfeasValue);
	}
	return SOLV_CALL_SUCCESS;
}


int coinNodeLogCallback(int IterCount, int NodeCount, double BestBound, double BestObject, int IsMipImproved)
{
	if ((NodeCount > 0) && (((NodeCount % 100) == 0) || (IsMipImproved))) {
		coinWriteMsgLog("Node: %5d  %s  %16.8lg  %16.8lg", 
		                   NodeCount, (IsMipImproved) ? "*" : " ", BestBound, BestObject);
	}
	if (global_pCoin->pSolver->MipNodeCallback) {
		global_pCoin->pSolver->MipNodeCallback(IterCount, NodeCount, BestBound, BestObject, IsMipImproved);
	}
	return SOLV_CALL_SUCCESS;
}




SOLVAPI int SOLVCALL CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MsgLogCallback = MsgLogCallback;
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

	pCoin->pSolver->IterCallback = IterCallback;
	delete pCoin->iterhandler;
	pCoin->iterhandler = new CBIterHandler(pCoin->clp);
	pCoin->iterhandler->setIterCallback(IterCallback);
	if (pCoin->clp) pCoin->clp->passInEventHandler(pCoin->iterhandler);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinSetMipNodeCallback(HPROB hProb, MIPNODECALLBACK MipNodeCallback)
{
	PCOIN pCoin = (PCOIN)hProb;

	pCoin->pSolver->MipNodeCallback = MipNodeCallback;
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

	if (CoinGetOptionChanged(hProb, COIN_REAL_MAXSECONDS))		pCoin->clp->setMaximumSeconds(CoinGetRealOption(hProb, COIN_REAL_MAXSECONDS));
	if (CoinGetOptionChanged(hProb, COIN_INT_MAXITER))			pCoin->clp->setMaximumIterations(CoinGetIntOption(hProb, COIN_INT_MAXITER));

	if (CoinGetOptionChanged(hProb, COIN_REAL_PRIMALOBJLIM))	pCoin->clp->setPrimalObjectiveLimit(CoinGetRealOption(hProb, COIN_REAL_PRIMALOBJLIM));
	if (CoinGetOptionChanged(hProb, COIN_REAL_DUALOBJLIM))		pCoin->clp->setDualObjectiveLimit(CoinGetRealOption(hProb, COIN_REAL_DUALOBJLIM));
	if (CoinGetOptionChanged(hProb, COIN_REAL_PRIMALOBJTOL))	pCoin->clp->setPrimalTolerance(CoinGetRealOption(hProb, COIN_REAL_PRIMALOBJTOL));
	if (CoinGetOptionChanged(hProb, COIN_REAL_DUALOBJTOL))		pCoin->clp->setDualTolerance(CoinGetRealOption(hProb, COIN_REAL_DUALOBJTOL));

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
			pCoin->clp->crash(CoinGetRealOption(hProb, COIN_REAL_CRASHGAP),
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
	if (CoinGetOptionChanged(hProb, COIN_REAL_MIPFRACGAP))		pCoin->cbc->setAllowableFractionGap(CoinGetRealOption(hProb, COIN_REAL_MIPFRACGAP));
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
	PPROBLEM pProblem = pCoin->pProblem;
	int sos, count;
	int* which;
	int type;

	for (sos = 0; sos < pProblem->SosCount; sos++) {
		count = pProblem->SosBegin[sos+1] - pProblem->SosBegin[sos];
		which = &pProblem->SosIndex[pProblem->SosBegin[sos]];
		type = pProblem->SosType[sos];
		CbcObject *sosObject = new CbcSOS(pCoin->cbc, count, which, NULL, 0, type);
		pCoin->cbc->addObjects(1, &sosObject);
		delete sosObject;
	}
	return 1;
}


/************************************************************************/
/*  Optimization                                                        */
/************************************************************************/

int CbcRetrieveSolutionResults(PPROBLEM pProblem, HPROB hProb, PRESULT pResult)
{
	PCOIN pCoin = (PCOIN)hProb;
	const double* columnPrimal;
	const double* columnDual;
	const double* rowPrimal;
	const double* rowDual;

	if (!pProblem->SolveAsMIP) {
		pResult->SolutionStatus = pCoin->clp->status();
		pResult->ObjectValue    = pCoin->clp->objectiveValue();
		pResult->MipBestBound   = 0.0;
		pResult->IterCount      = pCoin->clp->numberIterations();
		pResult->MipNodeCount	  = 0;
		}
	else {
		pResult->SolutionStatus = pCoin->cbc->status();
		pResult->ObjectValue    = pCoin->cbc->getObjValue();
		pResult->MipBestBound   = pCoin->cbc->getBestPossibleObjValue();
		pResult->IterCount      = pCoin->cbc->getIterationCount();
		pResult->MipNodeCount   = pCoin->cbc->getNodeCount();
	}

	switch (pResult->SolutionStatus) {
		case 0:	strcpy(pResult->SolutionText, "Optimal solution found");		break;
		case 1:	strcpy(pResult->SolutionText, "Problem primal infeasible");	break;
		case 2:	strcpy(pResult->SolutionText, "Problem dual infeasible");		break;
		case 3:	strcpy(pResult->SolutionText, "Stopped on iterations");			break;
		case 4: strcpy(pResult->SolutionText, "Stopped due to errors");			break;
		case 5: strcpy(pResult->SolutionText, "Stopped by user");		break;
		default: 
			sprintf(pResult->SolutionText, "Unknown CBC solution status (%d)", pResult->SolutionStatus);
			break;
	}

	if (!pProblem->SolveAsMIP) {
		columnPrimal = pCoin->clp->primalColumnSolution();
		columnDual = pCoin->clp->dualColumnSolution();
		rowPrimal = pCoin->clp->primalRowSolution();
		rowDual = pCoin->clp->dualRowSolution();
		pResult->ColActivity = (double*) malloc(pProblem->ColCount * sizeof(double));
		pResult->ReducedCost = (double*) malloc(pProblem->ColCount * sizeof(double));
		pResult->SlackValues = (double*) malloc(pProblem->RowCount * sizeof(double));
		pResult->ShadowPrice = (double*) malloc(pProblem->RowCount * sizeof(double));
		if (!pResult->ColActivity ||
			!pResult->ReducedCost || 
			!pResult->SlackValues || 
			!pResult->ShadowPrice) {
			return SOLV_CALL_FAILED;
		}
		memcpy(pResult->ColActivity, columnPrimal, pProblem->ColCount * sizeof(double));
		memcpy(pResult->ReducedCost, columnDual, pProblem->ColCount * sizeof(double));
		memcpy(pResult->SlackValues, rowPrimal, pProblem->RowCount * sizeof(double));
		memcpy(pResult->ShadowPrice, rowDual, pProblem->RowCount * sizeof(double));
		}
	else {
		columnPrimal = pCoin->cbc->solver()->getColSolution();
		pResult->ColActivity = (double*) malloc(pProblem->ColCount * sizeof(double));
		if (!pResult->ColActivity) {
			return SOLV_CALL_FAILED;
		}
		memcpy(pResult->ColActivity, columnPrimal, pProblem->ColCount * sizeof(double));
	}
	return SOLV_CALL_SUCCESS;
}


extern int CbcOrClpRead_mode;


SOLVAPI int SOLVCALL CoinOptimizeProblem(HPROB hProb, int Method)
{		
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;

	if (!pProblem->SolveAsMIP) {
		coinSetClpOptions(hProb);
		if (CoinGetOptionChanged(hProb, COIN_INT_PRESOLVETYPE))
			pCoin->clp->initialSolve(*pCoin->clp_presolve);
		else {
			pCoin->clp->initialSolve();
		}
		pCoin->pResult->SolutionStatus = pCoin->clp->status();
		}
	else {
#ifdef NEW_STYLE_CBCMAIN
		if (CoinGetIntOption(hProb, COIN_INT_MIPUSECBCMAIN)) {
			if (!pCoin->CbcMain0Already) {
				CbcMain0(*pCoin->cbc);
				pCoin->CbcMain0Already = 1;
			}
			coinSetClpOptions(hProb);
			coinSetCbcOptions(hProb);
			//coinSetCglOptions(hProb);  BK: CbcMain1 should be calling the Cgl's automatically
			CbcOrClpRead_mode = 1;  // BK: Fix bug in CbcMain1, CbcOrClpRead_mode not initialized  (CpcSolver.cpp, stable 2.2)
			const int argc = 3;
			const char* argv[] = {"CoinMP", "-solve", "-quit"};
			CbcMain1(argc,argv,*pCoin->cbc);
			pCoin->pResult->SolutionStatus = pCoin->cbc->status();
			}
		else 
#endif
		{
			coinSetClpOptions(hProb);
			coinSetCbcOptions(hProb);
			coinSetCglOptions(hProb);

			pCoin->cbc->initialSolve();
			pCoin->cbc->branchAndBound();
			pCoin->pResult->SolutionStatus = pCoin->cbc->status();
		}
	}	
	CbcRetrieveSolutionResults(pProblem, hProb, pCoin->pResult);
	return pCoin->pResult->SolutionStatus;
}



/************************************************************************/
/*  Solution status                                                     */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionStatus(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionStatus;
}


  

SOLVAPI const char* SOLVCALL CoinGetSolutionText(HPROB hProb, int SolutionStatus)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->SolutionText;
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

	return pCoin->pResult->ObjectValue + pCoin->pProblem->ObjectConst;
}


SOLVAPI double SOLVCALL CoinGetMipBestBound(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->MipBestBound;
}



SOLVAPI int SOLVCALL CoinGetIterCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->IterCount;
}


SOLVAPI int SOLVCALL CoinGetMipNodeCount(HPROB hProb)
{
	PCOIN pCoin = (PCOIN)hProb;

	return pCoin->pResult->MipNodeCount;
}



/************************************************************************/
/*  Solution retrieval                                                  */
/************************************************************************/


SOLVAPI int SOLVCALL CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
											 double* SlackValues, double* ShadowPrice)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (Activity && pResult->ColActivity) {
		memcpy(Activity, pResult->ColActivity, pProblem->ColCount * sizeof(double));
	}
	if (ReducedCost && pResult->ReducedCost) {
		memcpy(ReducedCost, pResult->ReducedCost, pProblem->ColCount * sizeof(double));
	}
	if (SlackValues && pResult->SlackValues) {
		memcpy(SlackValues, pResult->SlackValues, pProblem->RowCount * sizeof(double));
	}
	if (ShadowPrice && pResult->ShadowPrice) {
		memcpy(ShadowPrice, pResult->ShadowPrice, pProblem->RowCount * sizeof(double));
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinGetSolutionRanges(HPROB hProb, double* ObjLoRange, double* ObjUpRange,
											 double* RhsLoRange, double* RhsUpRange)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (ObjLoRange && pResult->ObjLoRange) {
		memcpy(ObjLoRange, pResult->ObjLoRange, pProblem->ColCount * sizeof(double));
	}
	if (ObjUpRange && pResult->ObjUpRange) {
		memcpy(ObjUpRange, pResult->ObjUpRange, pProblem->ColCount * sizeof(double));
	}
	if (RhsLoRange && pResult->RhsLoRange) {
		memcpy(RhsLoRange, pResult->RhsLoRange, pProblem->RowCount * sizeof(double));
	}
	if (RhsUpRange && pResult->RhsUpRange) {
		memcpy(RhsUpRange, pResult->RhsUpRange, pProblem->RowCount * sizeof(double));
	}
	return SOLV_CALL_SUCCESS;
}



SOLVAPI int SOLVCALL CoinGetSolutionBasis(HPROB hProb, int* ColStatus, double* RowStatus)
{
	PCOIN pCoin = (PCOIN)hProb;
	PPROBLEM pProblem = pCoin->pProblem;
	PRESULT pResult = pCoin->pResult;

	if (ColStatus && pResult->ColStatus) {
		memcpy(ColStatus, pResult->ColStatus, pProblem->ColCount * sizeof(int));
	}
	if (RowStatus && pResult->RowStatus) {
		memcpy(RowStatus, pResult->RowStatus, pProblem->RowCount * sizeof(int));
	}
	return SOLV_CALL_SUCCESS;
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
			pCoin->osi->writeMps(WriteFilename, "", pCoin->pProblem->ObjectSense);   
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
	PCOIN pCoin = (PCOIN)hProb;

	strcpy(pCoin->pSolver->LogFilename, logFilename);
	return SOLV_CALL_SUCCESS;
}


SOLVAPI int SOLVCALL CoinCloseLogFile(HPROB hProb)
{
	return SOLV_CALL_SUCCESS;
}



/************************************************************************/
/*  Option Table                                                        */
/************************************************************************/

#define OPTIONCOUNT    68


SOLVOPTINFO OptionTable[OPTIONCOUNT] = {

	{"SolveMethod",            "SolveMethod",  GRP_SIMPLEX,        0,        0,      0,       5,  OPT_LIST,   0,   COIN_INT_SOLVEMETHOD},
      /*enum SolveType { useDual=0, usePrimal, usePrimalorSprint, useBarrier, useBarrierNoCross, automatic*/
	{"PresolveType",           "Presolve",     GRP_SIMPLEX,        0,        0,      0,       2,  OPT_LIST,   0,   COIN_INT_PRESOLVETYPE},
      /*enum PresolveType { presolveOn=0, presolveOff, presolveNumber }; */
	{"Scaling",                "Scaling",      GRP_SIMPLEX,        3,        3,      0,       4,  OPT_LIST,   0,   COIN_INT_SCALING}, 
      /* Sets or unsets scaling, 0 -off, 1 equilibrium, 2 geometric, 3, auto, 4 dynamic(later) */
	{"Perturbation",           "Perturb",      GRP_SIMPLEX,      100,      100,      0,     100,  OPT_INT,    0,   COIN_INT_PERTURBATION}, 
      /* 0 - Off, 50 - On, 100 - Only if clp stalls */
	{"PrimalColPivotAlg",      "PrimPivAlg",   GRP_SIMPLEX,        1,        1,      0,       4,  OPT_LIST,   0,   COIN_INT_PRIMALPIVOTALG}, 
      /*0 is exact devex, 1 full steepest, 2 is partial exact devex
        3 switches between 0 and 2 depending on factorization
        4 starts as partial dantzig/devex but then may switch between 0 and 2.*/
	{"DualRowPivotAlg",        "DualPivAlg",   GRP_SIMPLEX,        1,        1,      0,       3,  OPT_LIST,   0,   COIN_INT_DUALPIVOTALG}, 
      /*0 is uninitialized, 1 full, 2 is partial uninitialized,
        3 starts as 2 but may switch to 1.*/
	{"LogLevel",               "LogLevel",     GRP_LOGFILE,        1,        1,      0,       4,  OPT_LIST,   0,   COIN_INT_LOGLEVEL}, 
	{"MaxIterations",          "MaxIter",      GRP_LIMITS,  99999999, 99999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MAXITER}, 
	{"MaxSeconds",             "MaxSeconds",   GRP_LIMITS,      -1.0,     -1.0,   -1.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MAXSECONDS}, 

	{"CrashInd",               "CrashInd",     GRP_SIMPLEX,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_CRASHIND}, 
	{"CrashPivot",             "CrashPivot",   GRP_SIMPLEX,        0,        0,     -1,       2,  OPT_LIST,   0,   COIN_INT_CRASHPIVOT}, 
	{"CrashGap",               "CrashGap",     GRP_SIMPLEX,   1000.0,   1000.0,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_CRASHGAP}, 
	{"PrimalObjectLimit",      "PrimalObjLim", GRP_SIMPLEX,  MAXREAL,  MAXREAL,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_PRIMALOBJLIM}, 
	{"DualObjectLimit",        "DualObjLim",   GRP_SIMPLEX,  MAXREAL,  MAXREAL,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_DUALOBJLIM}, 
	{"PrimalTolerance",        "PrimalTol",    GRP_SIMPLEX,     1e-7,     1e-7,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_PRIMALOBJTOL}, 
	{"DualTolerance",          "DualTol",      GRP_SIMPLEX,     1e-7,     1e-7,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_DUALOBJTOL}, 

	{"MipMaxNodes",            "MipMaxNodes",  GRP_LIMITS,   9999999,  9999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXNODES}, 
	{"MipMaxSolutions",        "MipMaxSol",    GRP_LIMITS,   9999999,  9999999,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXSOL}, 
	{"MipMaxSeconds",          "MipMaxSec",    GRP_LIMITS,     1e100,    1e100,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPMAXSEC}, 

	{"MipFathomDiscipline",    "FathomDisc",   GRP_MIPSTRAT,       0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPFATHOMDISC}, 
	{"MipHotStart",            "HotStart",     GRP_MIPSTRAT,       0,        0,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPHOTSTART},
//	{"MipForcePriority",       "ForcePrior",   GRP_MIPSTRAT,      -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPFORCEPRIOR},
	{"MipMinimumDrop",         "MinimumDrop",  GRP_MIPSTRAT,    1e-4,     1e-4,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMINIMUMDROP},
	{"MipMaxCutPasses",        "MaxCutPass",   GRP_MIPSTRAT,       2,        2,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXCUTPASS},
	{"MipMaxCutPassAtRoot",    "MaxPassRoot",  GRP_MIPSTRAT,      50,       50,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPMAXPASSROOT},
	{"MipStrongBranching",     "StrongBranch", GRP_MIPSTRAT,       5,        5,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSTRONGBRANCH},
	{"MipScanGlobalCuts",      "ScanGlobCuts", GRP_MIPSTRAT,       1,        1,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSCANGLOBCUTS},

	{"MipIntegerTolerance",    "MipIntTol",    GRP_MIPTOL,      1e-6,     1e-6,    0.0,     1.0,  OPT_REAL,   0,   COIN_REAL_MIPINTTOL}, 
	{"MipInfeasWeight",        "MipInfWeight", GRP_MIPTOL,       0.0,      0.0,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPINFWEIGHT}, 
	{"MipCutoffIncrement",     "MipCutIncr",   GRP_MIPTOL,      1e-5,     1e-5,    0.0,     1.0,  OPT_REAL,   0,   COIN_REAL_MIPCUTOFF}, 
	{"MipAllowableGap",        "MipAbsGap",    GRP_MIPTOL,     1e-10,    1e-10,    0.0, MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPABSGAP}, 
	{"MipFractionalGap",       "MipFracGap",   GRP_MIPTOL,     1e-10,    1e-10,    0.0,     1.0,  OPT_REAL,   0,   COIN_REAL_MIPFRACGAP}, 

	/* Probing */
	{"MipCutProbing",          "CutProbing",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_PROBING},
	{"MipProbeFrequency",      "ProbeFreq",    GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_FREQ},
	{"MipProbeMode",           "ProbeMode",    GRP_MIPCUTS,        1,        1,      0,       2,  OPT_LIST,   0,   COIN_INT_MIPPROBE_MODE},
	{"MipProbeUseObjective",   "UseObject",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPPROBE_USEOBJ},
	{"MipProbeMaxPass",        "MaxPass",      GRP_MIPCUTS,        3,        3,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXPASS},
	{"MipProbeMaxProbe",       "MaxProbe",     GRP_MIPCUTS,      100,      100,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXPROBE},
	{"MipProbeMaxLook",        "MaxLook",      GRP_MIPCUTS,       50,       50,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPPROBE_MAXLOOK},
	{"MipProbeRowCuts",        "RowCuts",      GRP_MIPCUTS,        3,        3,      0,       3,  OPT_LIST,   0,   COIN_INT_MIPPROBE_ROWCUTS},

	/* Gomory cuts */
  	{"MipCutGomory",           "CutGomory",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_GOMORY},
	{"MipGomoryFrequency",     "GomoryFreq",   GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPGOMORY_FREQ},
	{"MipGomoryLimit",         "GomoryLimit",  GRP_MIPCUTS,       50,       50,      1,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPGOMORY_LIMIT},
	{"MipGomoryAway",          "GomoryAway",   GRP_MIPCUTS,     0.05,     0.05, 0.0001,     0.5,  OPT_REAL,   0,   COIN_REAL_MIPGOMORY_AWAY},

	/* Knapsack cuts */
	{"MipCutKnapsack",         "CutKnapsack",  GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_KNAPSACK},
	{"MipKnapsackFrequency",   "KrapsackFreq", GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPKNAPSACK_FREQ},
	{"MipKnapsackMaxIn",       "KnapsackMaxIn",GRP_MIPCUTS,       50,       50,      1,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPKNAPSACK_MAXIN},

	/* Oddhole cuts */
	{"MipCutOddHole",          "CutOddHole",   GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_ODDHOLE},
	{"MipOddHoleFrequency",    "OddHoleFreq",  GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPODDHOLE_FREQ},
	{"MipOddHoleMinViolation", "MinViolation", GRP_MIPCUTS,    0.001,    0.001,1.01e-8,     0.5,  OPT_REAL,   0,   COIN_REAL_MIPODDHOLE_MINVIOL},
	{"MipOddHoleMinViolPer",   "MinViolPer",   GRP_MIPCUTS,   0.0003,   0.0003,1.01e-8,    0.25,  OPT_REAL,   0,   COIN_REAL_MIPODDHOLE_MINVIOLPER},
	{"MipOddHoleMaxEntries",   "MaxEntries",   GRP_MIPCUTS,      100,      100,      3,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPODDHOLE_MAXENTRIES},

	/* Clique cuts */
	{"MipCutClique",           "CutClique",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_CLIQUE},
	{"MipCliqueFrequency",     "CliqueFreq",   GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_FREQ},
	{"MipCliquePacking",       "CliquePacking",GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_PACKING},
	{"MipCliqueStar",          "CliqueStar",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_STAR},
	{"MipCliqueStarMethod",    "StarMethod",   GRP_MIPCUTS,        2,        2,      0,       2,  OPT_LIST,   0,   COIN_INT_MIPCLIQUE_STARMETHOD},
	{"MipCliqueStarMaxLen",    "StarMaxLen",   GRP_MIPCUTS,       12,       12,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_STARMAXLEN},
	{"MipCliqueStarReport",    "StarReport",   GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_STARREPORT},
	{"MipCliqueRow",           "CliqueRow",    GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_ROW},
	{"MipCliqueRowMaxLen",     "ClqRowMaxLen", GRP_MIPCUTS,       12,       12,      0,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPCLIQUE_ROWMAXLEN},
	{"MipCliqueRowReport",     "ClqRowReport", GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCLIQUE_ROWREPORT},
	{"MipCliqueMinViolation",  "ClqMinViol",   GRP_MIPCUTS,     -1.0,     -1.0,-MAXREAL,MAXREAL,  OPT_REAL,   0,   COIN_REAL_MIPCLIQUE_MINVIOL},

	/* Lift and Project */
	{"MipCutLiftAndProject",   "CutLiftPro",   GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_LIFTPROJECT},
	{"MipLiftCoverFrequency",  "LiftProFreq",  GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPLIFTPRO_FREQ},
	{"MipLiftCoverBetaOne",    "LiftProBeta",  GRP_MIPCUTS,        1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPLIFTPRO_BETAONE},

	/* Simple Rounding */
	{"MipCutSimpleRounding",   "CutSimpRound", GRP_MIPCUTS,        0,        0,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPCUT_SIMPROUND},
	{"MipSimpleRoundFrequency","SimpRoundFreq",GRP_MIPCUTS,       -1,       -1,-MAXINT,  MAXINT,  OPT_INT,    0,   COIN_INT_MIPSIMPROUND_FREQ},

	{"MipUseCbcMain",          "UseCbcMain",   GRP_MIPSTRAT,       1,        1,      0,       1,  OPT_ONOFF,  0,   COIN_INT_MIPUSECBCMAIN}
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

