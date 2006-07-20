/************************************************************************/
/*                                                                      */
/*  COINMP.DLL                                        Maximal Software  */
/*                                                                      */
/*  File         :  'coinmp.cpp'                                        */
/*                                                                      */
/*  Author       :  Bjarni Kristjansson                                 */
/*                                                                      */
/*  Copyright (c) 2005-2006                     Bjarni Kristjansson     */
/*                                                                      */
/************************************************************************/


#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include <cfloat>

#include "CoinHelperFunctions.hpp"
#include "CoinMessageHandler.hpp"
#include "ClpSimplex.hpp"
#include "ClpPrimalColumnSteepest.hpp"
#include "ClpDualRowSteepest.hpp"
#include "ClpEventHandler.hpp"
#include "CbcEventHandler.hpp"
#include "OsiSolverInterface.hpp"
#include "OsiClpSolverInterface.hpp"
#include "CbcModel.hpp"

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
#define SOLVER_LIB
#include "coinmp.h"



/**************************************************************************/


class CBMessageHandler : public CoinMessageHandler {
public: 
   void setCallback(MSGLOGCALLBACK callback);
	virtual int print();
private:
	MSGLOGCALLBACK callback_;
};


void CBMessageHandler::setCallback(MSGLOGCALLBACK callback)
{
  callback_ = callback;
}


int CBMessageHandler::print()
{
	callback_((char *)messageBuffer());
	return CoinMessageHandler::print();
}



/**************************************************************************/


class CBIterHandler : public ClpEventHandler {

public: 
   void setIterCallback(ITERCALLBACK callback);
   void setNodeCallback(NODECALLBACK callback);

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
	NODECALLBACK nodeCallback_;
};


void CBIterHandler::setIterCallback(ITERCALLBACK callback)
{
  iterCallback_ = callback;
}


void CBIterHandler::setNodeCallback(NODECALLBACK callback)
{
  nodeCallback_ = callback;
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
	if ((whichEvent == node) ||
		 (whichEvent == treeStatus) ||
		 (whichEvent == solution)) {
		numIter = model_->getIterationCount();
		objValue = model_->getObjValue();
		cancelAsap = nodeCallback_(numIter, 0, 0.0, objValue, 0);
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
	iterhandler->nodeCallback_ = this->nodeCallback_;
	return iterhandler;
}



/**************************************************************************/


class CBNodeHandler : public CbcEventHandler {

public: 
   void setCallback(NODECALLBACK callback);

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
	NODECALLBACK callback_;
};


void CBNodeHandler::setCallback(NODECALLBACK callback)
{
  callback_ = callback;
}


CBNodeHandler::CbcAction CBNodeHandler::event(CbcEvent whichEvent)
{
	int numIter;
	double objValue;
	int cancelAsap;

	if (whichEvent==node) {
		numIter = model_->getIterationCount();
		objValue = model_->getObjValue();
		cancelAsap = callback_(numIter, 0, 0.0, objValue, 0);
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
	nodehandler->callback_ = this->callback_;
	return nodehandler;
}



/**************************************************************************/

typedef struct {
				ClpSimplex *clp;
				CbcModel *cbc;
				OsiClpSolverInterface *osi;

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

				int SolutionStatus;
				int RowCount, ColCount;
				double *RowLower;
				double *RowUpper;
				char SolutionText[100];
				char *IsInt;
				int SolveAsMIP;
				MSGLOGCALLBACK  MessageLogCallback;
				ITERCALLBACK    IterationCallback;
				NODECALLBACK    MipNodeCallback;
				} COININFO, *PCOIN;
 


PCOIN global_pCoin;



SOLVAPI int CoinInitSolver(char *LicenseStr)
{
	return SOLV_SUCCESS;
}


SOLVAPI int CoinFreeSolver(void)
{
	return SOLV_SUCCESS;
}



SOLVAPI char*  CoinGetSolverName(void)
{
	return "CoinMP";
}


SOLVAPI char*  CoinGetVersionStr(void)
{
	return "1.0";
}


SOLVAPI double CoinGetVersion(void)
{
	return 1.0;
}


SOLVAPI int    CoinGetFeatures(void)
{
	return 0;
}



SOLVAPI double CoinGetRealMax(void)
{
	return COIN_DBL_MAX;
}



SOLVAPI HPROB CoinCreateProblem(char *ProblemName)
{
   PCOIN pCoin;

   pCoin = (PCOIN) malloc(sizeof(COININFO));
   global_pCoin = pCoin;
   pCoin->clp = new ClpSimplex();
	pCoin->osi = new OsiClpSolverInterface(pCoin->clp);
	pCoin->cbc = NULL;  /* ERRORFIX 2/22/05: Crashes if not NULL when trying to set message handler */
	pCoin->msghandler = NULL;
	pCoin->iterhandler = NULL;
	pCoin->nodehandler = NULL;
	pCoin->ColCount = 0;
	pCoin->RowCount = 0;
	pCoin->RowLower = NULL;
	pCoin->RowUpper = NULL;
	pCoin->IsInt = 0;
	pCoin->SolveAsMIP = 0;
   return (HPROB)pCoin;
}



SOLVAPI int CoinLoadProblem(HPROB hProb, int ColCount, int RowCount, int NonZeroCount, 
											int RangeCount, int ObjectSense, double* ObjectCoeffs, 
											double* RHSValues, double* RangeValues, char* RowType, 
											int* MatrixBegin, int* MatrixCount, int* MatrixIndex, 
											double* MatrixValues, double* LowerBounds, double* UpperBounds, 
											double* InitValues, char** ColNames, char** RowNames)
{
   PCOIN pCoin = (PCOIN)hProb;
   int i;

	pCoin->ColCount = ColCount;
	pCoin->RowCount = RowCount;

	pCoin->clp->setOptimizationDirection(ObjectSense);

	pCoin->RowLower = (double *)malloc(RowCount*sizeof(double));
	pCoin->RowUpper = (double *)malloc(RowCount*sizeof(double));
	if (!pCoin->RowLower && !pCoin->RowUpper) {
		return SOLV_FAILED;
	}
   for( i = 0; i < RowCount; i++ ){
		switch (RowType[i]) {
			case 'L':
				pCoin->RowLower[i] = -COIN_DBL_MAX;
				pCoin->RowUpper[i] = RHSValues[i];
				break;

			case 'G':
				pCoin->RowLower[i] = RHSValues[i];
				pCoin->RowUpper[i] = COIN_DBL_MAX;
				break;

			case 'E':
				pCoin->RowLower[i] = RHSValues[i];
				pCoin->RowUpper[i] = RHSValues[i];
				break;

			case 'R':
				if (RangeValues[i] < 0.0)
					pCoin->RowLower[i] = RHSValues[i]+RangeValues[i];
				else {
					pCoin->RowLower[i] = RHSValues[i]-RangeValues[i];
				}
				pCoin->RowUpper[i] = RHSValues[i];
				break;
		}
	}
   pCoin->clp->loadProblem(ColCount, RowCount, MatrixBegin, MatrixIndex, MatrixValues,
							LowerBounds, UpperBounds, ObjectCoeffs, pCoin->RowLower, pCoin->RowUpper);


	std::vector<std::string> rowNameList;
	rowNameList.reserve(RowCount);
	for (i = 0; i < RowCount; i++) {
		rowNameList.push_back(RowNames[i]);
	}
	std::vector<std::string> colNameList;
	colNameList.reserve(ColCount);
	for (i = 0; i < ColCount; i++) {
		colNameList.push_back(ColNames[i]);
	}
	pCoin->clp->copyNames(rowNameList, colNameList);

   return SOLV_SUCCESS;
}


SOLVAPI int CoinLoadInteger(HPROB hProb, char* ColumnType)
{   
	PCOIN pCoin = (PCOIN)hProb;
	int i;

	pCoin->IsInt = (char *)malloc(pCoin->ColCount*sizeof(char));
	if (!pCoin->IsInt) {
		return SOLV_FAILED;
	}
	for (i = 0; i < pCoin->ColCount; i++ ) {
		if ( ColumnType[i] == 'B' || ColumnType[i] == 'I' ) {
			pCoin->IsInt[i] = 1;
			pCoin->SolveAsMIP = 1;
			//pCoin->cbc->solver()->setInteger(i);
		}
		else {
			pCoin->IsInt[i] = 0;
			//pCoin->cbc->solver()->setContinuous(i);
		}
	}
	if (pCoin->SolveAsMIP) {
		//pCoin->clp->copyInIntegerInformation(pCoin->IsInt);
		pCoin->cbc = new CbcModel(*pCoin->osi);
		for (i = 0; i < pCoin->ColCount; i++ ) {
			if (pCoin->IsInt[i]) {
				pCoin->cbc->solver()->setInteger(i);
			}
		}
	}
	//pCoin->cbc->solver()->copyInIntegerInformation(pCoin->IsInt);

	return SOLV_SUCCESS;
}



SOLVAPI int CoinLoadPriority(HPROB hProb, int PriorCount, int *PriorIndex, 
									  int *PriorValues, int *BranchDir)
{
	return SOLV_FAILED;
}


SOLVAPI int CoinLoadSos(HPROB hProb, int SosCount, int SosNZCount, 
								char *SosType, int *SosPrior, int *SosBegin,   
								int *SosIndex, double *SosRef)
{
	return SOLV_FAILED;
}


SOLVAPI int CoinLoadQuadratic(HPROB hProb, int *QuadBegin, int *QuadCount, 
										int *QuadIndex, double *QuadValues)
{
	return SOLV_FAILED;
}


SOLVAPI int CoinLoadNonLinear(HPROB hProb, int NlpTreeCount, int NlpLineCount, 
										int *NlpBegin, int *NlpOper, int *NlpArg1, 
										int *NlpArg2, int *NlpIndex1, int *NlpIndex2, 
										double *NlpValue1, double *NlpValue2)
{
	return SOLV_FAILED;
}



SOLVAPI int CoinUnloadProb(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;
	
	if (pCoin) {
		delete pCoin->clp;
		if (pCoin->RowLower) free(pCoin->RowLower);
		if (pCoin->RowUpper) free(pCoin->RowUpper);
		if (pCoin->IsInt) free(pCoin->IsInt);
	}
	free(pCoin);
	pCoin = NULL;
	return SOLV_SUCCESS;
}



/****************************************************************/


int CoinWriteMsgLog(char *FormatStr, ...)
{
   va_list pVa;
   char strbuf[256];

   va_start(pVa,FormatStr);
   vsprintf(strbuf,FormatStr,pVa);
	global_pCoin->MessageLogCallback(strbuf);
	return SOLV_SUCCESS;
}


int CoinIterLogCallback(int IterCount, double ObjectValue, int IsFeasible, double InfeasValue)
{
	if (!global_pCoin->SolveAsMIP) {
		if (((IterCount < 100) && ((IterCount % 10) == 0)) ||
			 ((IterCount % 100) == 0)) {
			if (!IsFeasible)
				CoinWriteMsgLog("Iteration: %5d  %16.8lg  %16.8lg",IterCount, ObjectValue, InfeasValue);
			else {
				CoinWriteMsgLog("Iteration: %5d  %16.8lg",IterCount, ObjectValue);
			}
		}
	}
	global_pCoin->IterationCallback(IterCount, ObjectValue, IsFeasible, InfeasValue);
	return SOLV_SUCCESS;
}


int CoinNodeLogCallback(int IterCount, int NodeCount, double BestBound, double BestObject, int IsMipImproved)
{
	if ((NodeCount > 0) && (((NodeCount % 100) == 0) || (IsMipImproved))) {
		CoinWriteMsgLog("Node: %5d  %s  %16.8lg  %16.8lg", 
		                   NodeCount, (IsMipImproved) ? "*" : " ", BestBound, BestObject);
	}
   global_pCoin->MipNodeCallback(IterCount, NodeCount, BestBound, BestObject, IsMipImproved);
	return SOLV_SUCCESS;
}




SOLVAPI int CoinSetMsgLogCallback(HPROB hProb, MSGLOGCALLBACK MsgLogCallback)
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
	return SOLV_SUCCESS;
}



SOLVAPI int CoinSetIterCallback(HPROB hProb, ITERCALLBACK IterCallback)
{
   PCOIN pCoin = (PCOIN)hProb;

   pCoin->IterationCallback = IterCallback;
	delete pCoin->iterhandler;
	pCoin->iterhandler = new CBIterHandler(pCoin->clp);
	pCoin->iterhandler->setIterCallback(IterCallback);
	if (pCoin->clp) pCoin->clp->passInEventHandler(pCoin->iterhandler);
	return SOLV_SUCCESS;
}


SOLVAPI int CoinSetMipNodeCallback(HPROB hProb, NODECALLBACK NodeCallback)
{
   PCOIN pCoin = (PCOIN)hProb;

   pCoin->MipNodeCallback = NodeCallback;
	delete pCoin->nodehandler;

	//JPF: pCoin->nodehandler = new CBNodeHandler(pCoin->clp);
	pCoin->nodehandler = new CBNodeHandler(pCoin->cbc);

	pCoin->nodehandler->setCallback(NodeCallback);
	//if (pCoin->iterhandler) pCoin->iterhandler->setNodeCallback(NodeCallback);
	if (pCoin->cbc) pCoin->cbc->passInEventHandler(pCoin->nodehandler);
	return SOLV_SUCCESS;
}



/****************************************************************/

SOLVAPI int CoinOptimizeProblem(HPROB hProb, int Method)
{		
   PCOIN pCoin = (PCOIN)hProb;
   ClpSolve clp_options;
	ClpSolve::SolveType method;
	ClpSolve::PresolveType presolve;

	pCoin->clp->scaling(CoinGetIntOption(hProb,COIN_INT_SCALING));
	pCoin->clp->setPerturbation(CoinGetIntOption(hProb, COIN_INT_PERTURBATION));

	pCoin->clp->setMaximumIterations(CoinGetIntOption(hProb, COIN_INT_MAXITER));

	pCoin->clp->setPrimalObjectiveLimit(CoinGetIntOption(hProb, COIN_REAL_PRIMALOBJLIM));
	pCoin->clp->setDualObjectiveLimit(CoinGetIntOption(hProb, COIN_REAL_DUALOBJLIM));
	pCoin->clp->setPrimalTolerance(CoinGetIntOption(hProb, COIN_REAL_PRIMALOBJTOL));
	pCoin->clp->setDualTolerance(CoinGetIntOption(hProb, COIN_REAL_DUALOBJTOL));

	/* check if it has been changed, leave alone otherwise */
   ClpPrimalColumnSteepest primalSteepest(CoinGetIntOption(hProb, COIN_INT_PRIMALPIVOTALG));
   pCoin->clp->setPrimalColumnPivotAlgorithm(primalSteepest);

   ClpDualRowSteepest dualSteepest(CoinGetIntOption(hProb, COIN_INT_DUALPIVOTALG));
   pCoin->clp->setDualRowPivotAlgorithm(dualSteepest);

	if (CoinGetIntOption(hProb, COIN_INT_CRASHIND)) {
		pCoin->clp->crash(CoinGetIntOption(hProb, COIN_REAL_CRASHGAP),
								CoinGetIntOption(hProb, COIN_INT_CRASHPIVOT));
	}
	switch (CoinGetIntOption(hProb,COIN_INT_SOLVEMETHOD)) {
		case 0: method = ClpSolve::useDual;				 break;
		case 1: method = ClpSolve::usePrimal;			 break;
		case 2: method = ClpSolve::usePrimalorSprint; break;
		case 3: method = ClpSolve::useBarrier;			 break;
		case 4: method = ClpSolve::useBarrierNoCross; break;
		case 5: method = ClpSolve::automatic;			 break;
		default: method = ClpSolve::usePrimal;
	}
	pCoin->clp->setSolveType(method);   //ClpSolve::usePrimal

	switch (CoinGetIntOption(hProb,COIN_INT_PRESOLVETYPE)) {
		case 0: presolve = ClpSolve::presolveOn;		 break;
		case 1: presolve = ClpSolve::presolveOff;		 break;
		case 2: presolve = ClpSolve::presolveNumber;	 break;
		default: presolve = ClpSolve::presolveOn;
	}
	clp_options.setPresolveType(presolve);   //ClpSolve::presolveOn

	if (!pCoin->SolveAsMIP) {
		pCoin->clp->initialSolve(clp_options);
		pCoin->SolutionStatus = pCoin->clp->status();
		}
	else {
		pCoin->cbc->setMaximumNodes(CoinGetIntOption(hProb, COIN_INT_MIPMAXNODES));
		pCoin->cbc->setMaximumSolutions(CoinGetIntOption(hProb, COIN_INT_MIPMAXSOL));
		pCoin->cbc->setDblParam(CbcModel::CbcMaximumSeconds,CoinGetRealOption(hProb, COIN_REAL_MIPMAXSEC));

		pCoin->cbc->setIntParam(CbcModel::CbcFathomDiscipline,CoinGetIntOption(hProb, COIN_INT_MIPFATHOMDISC));

		// JPF commented: pCoin->cbc->setHotstartStrategy(CoinGetIntOption(hProb, COIN_INT_MIPHOTSTART));
		//		pCoin->cbc->setForcePriority(CoinGetIntOption(hProb, COIN_INT_MIPFORCEPRIOR));

		pCoin->cbc->setMinimumDrop(CoinGetIntOption(hProb, COIN_INT_MIPMINIMUMDROP));
		pCoin->cbc->setMaximumCutPassesAtRoot(CoinGetIntOption(hProb, COIN_INT_MIPMAXPASSROOT));
		pCoin->cbc->setMaximumCutPasses(CoinGetIntOption(hProb, COIN_INT_MIPMAXCUTPASS));
		pCoin->cbc->setNumberStrong(CoinGetIntOption(hProb, COIN_INT_MIPSTRONGBRANCH));
		pCoin->cbc->setHowOftenGlobalScan(CoinGetIntOption(hProb, COIN_INT_MIPSCANGLOBCUTS));

		pCoin->cbc->setIntegerTolerance(CoinGetRealOption(hProb, COIN_REAL_MIPINTTOL));
		pCoin->cbc->setInfeasibilityWeight(CoinGetRealOption(hProb, COIN_REAL_MIPINFWEIGHT));
		pCoin->cbc->setDblParam(CbcModel::CbcCutoffIncrement,CoinGetRealOption(hProb, COIN_REAL_MIPCUTOFF));
		pCoin->cbc->setAllowableGap(CoinGetRealOption(hProb, COIN_REAL_MIPABSGAP));

		/* see CbcModel.hpp has commments on calling cuts */

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_PROBING)) {
			pCoin->probing = new CglProbing();
			pCoin->probing->setMode(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MODE));
			pCoin->probing->setUsingObjective(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_USEOBJ) ? true : false);
			pCoin->probing->setMaxPass(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXPASS));
			pCoin->probing->setMaxProbe(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXPROBE));
			pCoin->probing->setMaxLook(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_MAXLOOK));
			pCoin->probing->setRowCuts(CoinGetIntOption(hProb, COIN_INT_MIPPROBE_ROWCUTS));
			pCoin->cbc->addCutGenerator(pCoin->probing,CoinGetIntOption(hProb, COIN_INT_MIPPROBE_FREQ),"Probing");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_GOMORY)) {
			pCoin->gomory = new CglGomory();
			pCoin->gomory->setLimit(CoinGetIntOption(hProb, COIN_INT_MIPGOMORY_LIMIT));
			pCoin->gomory->setAway(CoinGetRealOption(hProb, COIN_REAL_MIPGOMORY_AWAY));
			pCoin->cbc->addCutGenerator(pCoin->gomory,CoinGetIntOption(hProb, COIN_INT_MIPGOMORY_FREQ),"Gomory");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_KNAPSACK)) {
			pCoin->knapsack = new CglKnapsackCover();
			pCoin->knapsack->setMaxInKnapsack(CoinGetIntOption(hProb, COIN_INT_MIPKNAPSACK_MAXIN));
			pCoin->cbc->addCutGenerator(pCoin->knapsack,CoinGetIntOption(hProb, COIN_INT_MIPKNAPSACK_FREQ),"Knapsack");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_ODDHOLE)) {
			pCoin->oddhole= new CglOddHole();
			pCoin->oddhole->setMinimumViolation(CoinGetRealOption(hProb, COIN_REAL_MIPODDHOLE_MINVIOL));
			pCoin->oddhole->setMinimumViolationPer(CoinGetRealOption(hProb, COIN_REAL_MIPODDHOLE_MINVIOLPER));
			pCoin->oddhole->setMaximumEntries(CoinGetIntOption(hProb, COIN_INT_MIPODDHOLE_MAXENTRIES));
			pCoin->cbc->addCutGenerator(pCoin->oddhole,CoinGetIntOption(hProb, COIN_INT_MIPODDHOLE_FREQ),"OddHole");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_CLIQUE)) {
			pCoin->clique= new CglClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_PACKING) ? true : false);
			pCoin->clique->setDoStarClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STAR) ? true : false);
			pCoin->clique->setStarCliqueNextNodeMethod((CglClique::scl_next_node_method)CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARMETHOD));
			pCoin->clique->setStarCliqueCandidateLengthThreshold(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARMAXLEN));
			pCoin->clique->setStarCliqueReport(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_STARREPORT) ? true : false);
			pCoin->clique->setDoRowClique(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROW) ? true : false);
			pCoin->clique->setRowCliqueCandidateLengthThreshold(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROWMAXLEN));
			pCoin->clique->setRowCliqueReport(CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_ROWREPORT) ? true : false);
			pCoin->clique->setMinViolation(CoinGetRealOption(hProb, COIN_REAL_MIPCLIQUE_MINVIOL));
			pCoin->cbc->addCutGenerator(pCoin->clique,CoinGetIntOption(hProb, COIN_INT_MIPCLIQUE_FREQ),"Clique");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_LIFTPROJECT)) {
			pCoin->liftpro = new CglLiftAndProject();
			pCoin->liftpro->setBeta(CoinGetIntOption(hProb, COIN_INT_MIPLIFTPRO_BETAONE) ? 1 : -1);
			pCoin->cbc->addCutGenerator(pCoin->liftpro,CoinGetIntOption(hProb, COIN_INT_MIPLIFTPRO_FREQ),"LiftProject");
		}

		if (CoinGetIntOption(hProb, COIN_INT_MIPCUT_SIMPROUND)) {
			pCoin->rounding = new CglSimpleRounding();
			pCoin->cbc->addCutGenerator(pCoin->rounding,CoinGetIntOption(hProb, COIN_INT_MIPSIMPROUND_FREQ),"Rounding");
		}

		pCoin->cbc->initialSolve();
		pCoin->cbc->branchAndBound();
		pCoin->SolutionStatus = pCoin->cbc->status();
	}
	
	return pCoin->SolutionStatus;
}



SOLVAPI int CoinGetSolutionStatus(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	return pCoin->clp->status();
}



SOLVAPI char * CoinGetSolutionText(HPROB hProb, int SolutionStatus)
{
   PCOIN pCoin = (PCOIN)hProb;

	switch (SolutionStatus) {
		case 0:	strcpy(pCoin->SolutionText, "Optimal solution found");		break;
		case 1:	strcpy(pCoin->SolutionText, "Problem primal infeasible");	break;
		case 2:  strcpy(pCoin->SolutionText, "Problem dual infeasible");		break;
		case 3:  strcpy(pCoin->SolutionText, "Stopped on iterations");			break;
		case 4: 	strcpy(pCoin->SolutionText, "Stopped due to errors");			break;
		case 5: 	strcpy(pCoin->SolutionText, "Stopped by user");		break;
	}
	return pCoin->SolutionText;
}



SOLVAPI double CoinGetObjectValue(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return pCoin->clp->objectiveValue();
	else {
		return pCoin->cbc->getObjValue();
	}
}


SOLVAPI double CoinGetMipBestBound(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

   return 0;
}


SOLVAPI int CoinGetIterCount(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

   return pCoin->clp->numberIterations();
}


SOLVAPI int CoinGetNodeCount(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	if (!pCoin->SolveAsMIP) 
		return 0;
	else {
		return pCoin->cbc->getNodeCount();
	}
}


/****************************************************************/


SOLVAPI int CoinGetSolutionValues(HPROB hProb, double* Activity, double* ReducedCost, 
											 double* SlackValues, double* ShadowPrice)
{
   PCOIN pCoin = (PCOIN)hProb;
	const double *columnPrimal;
	const double *columnDual;
	const double *rowPrimal;
	const double *rowDual;
	int i;

	if (pCoin->SolveAsMIP) {
		if (Activity) {
			columnPrimal = pCoin->cbc->solver()->getColSolution();
			for (i = 0; i < pCoin->ColCount; i++) {
				Activity[i] = columnPrimal[i];
			}
			return SOLV_SUCCESS;
		}
		return SOLV_FAILED;
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
   return SOLV_SUCCESS;
}



SOLVAPI int CoinGetSolutionRanges(HPROB hProb, double *ObjLoRange, double *ObjUpRange,
											 double *RhsLoRange, double *RhsUpRange)
{
	return SOLV_FAILED;
}



SOLVAPI int CoinGetSolutionBasis(HPROB hProb, int *ColStatus, double *RowStatus)
{
	return SOLV_FAILED;
}



/****************************************************************/


SOLVAPI int CoinReadFile(HPROB hProb, int FileType, char* ReadFilename)
{
   PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		pCoin->clp->readMps(ReadFilename, 1, 0);    break;

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_FAILED;
	}
	return SOLV_SUCCESS;
}



SOLVAPI int CoinWriteFile(HPROB hProb, int FileType, char* WriteFilename)
{
   PCOIN pCoin = (PCOIN)hProb;

	switch (FileType) {
		case SOLV_FILE_MPS:		pCoin->clp->writeMps(WriteFilename);    break;

		case SOLV_FILE_LP: 
		case SOLV_FILE_BASIS: 
		case SOLV_FILE_IIS: 

		default:
			return SOLV_FAILED;

	}
	return SOLV_SUCCESS;
}



SOLVAPI int CoinOpenLogFile(HPROB hProb, char* logFilename)
{
   PCOIN pCoin = (PCOIN)hProb;

	return SOLV_SUCCESS;
}


SOLVAPI int CoinCloseLogFile(HPROB hProb)
{
   PCOIN pCoin = (PCOIN)hProb;

	return SOLV_SUCCESS;
}


/****************************************************************/
/****************************************************************/


#define MAXINT          2100000000L
#define MAXREAL         COIN_DBL_MAX

#define ROUND(x)       (((x)>0)?((long)((x)+0.5)):((long)((x)-0.5)))


#define OPT_NONE				0
#define OPT_ONOFF				1
#define OPT_LIST				2
#define OPT_INT				3
#define OPT_REAL				4
#define OPT_STRING			5

#define GRP_NONE           0
#define GRP_OTHER          0

#define GRP_SIMPLEX			1
#define GRP_PREPROC			2
#define GRP_LOGFILE			3
#define GRP_LIMITS			4
#define GRP_MIPSTRAT			5
#define GRP_MIPCUTS        6
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


#define OPTIONCOUNT    66



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

	};




SOLVAPI int CoinGetOptionCount(HPROB hProb)
{
	return OPTIONCOUNT;
}


SOLVAPI int CoinGetOptionInfo(HPROB hProb, int OptionNr, int *OptionID, int *GroupType,
										int *OptionType, char *OptionName, char *ShortName)
{
	if ((OptionNr < 0) && (OptionNr >= OPTIONCOUNT)) {
		return SOLV_FAILED;
	}
	if (OptionID)   *OptionID = OptionTable[OptionNr].OptionID;
	if (OptionType) *OptionType = OptionTable[OptionNr].OptionType;
	if (OptionName) strcpy(OptionName, OptionTable[OptionNr].OptionName);
	if (ShortName)  strcpy(ShortName, OptionTable[OptionNr].ShortName);
	return SOLV_SUCCESS;
}


SOLVAPI int CoinGetIntOptionMinMax(HPROB hProb, int OptionNr, int *MinValue, int *MaxValue)
{
	if ((OptionNr < 0) && (OptionNr >= OPTIONCOUNT)) {
		return SOLV_FAILED;
	}
	if (MinValue)   *MinValue = ROUND(OptionTable[OptionNr].MinValue);
	if (MaxValue)   *MaxValue = ROUND(OptionTable[OptionNr].MaxValue);
	return SOLV_SUCCESS;
}


SOLVAPI int CoinGetRealOptionMinMax(HPROB hProb, int OptionNr, double *MinValue, double *MaxValue)
{
	if ((OptionNr < 0) && (OptionNr >= OPTIONCOUNT)) {
		return SOLV_FAILED;
	}
	if (MinValue)   *MinValue = OptionTable[OptionNr].MinValue;
	if (MaxValue)   *MaxValue = OptionTable[OptionNr].MaxValue;
	return SOLV_SUCCESS;
}




/****************************************************************/



int LocateOptionID(int OptionID)
{
	int i;

	for (i = 0; i < OPTIONCOUNT; i++) {
		if (OptionID == OptionTable[i].OptionID) {
			return i;
		}
	}
	return -1;
}



SOLVAPI int CoinGetIntOption(HPROB hProb,int OptionID)
{   
	PCOIN pCoin = (PCOIN)hProb;
	int OptionNr;

	OptionNr = LocateOptionID(OptionID);
	if (OptionNr < 0) {
		return 0;
	}
	if (OptionTable[OptionNr].OptionType != OPT_REAL)
		return OptionTable[OptionNr].CurrentValue;
	else {
		return 0;
	}
}
  	


SOLVAPI int CoinSetIntOption(HPROB hProb,int OptionID, int IntValue)
{
   PCOIN pCoin = (PCOIN)hProb;
	int OptionNr;

	OptionNr = LocateOptionID(OptionID);
	if (OptionNr < 0) {
		return SOLV_FAILED;
	}
	if (OptionTable[OptionNr].OptionType != OPT_REAL) {
		OptionTable[OptionNr].CurrentValue = IntValue;
	   return SOLV_SUCCESS;
	}
	return SOLV_FAILED;
}



SOLVAPI double CoinGetRealOption(HPROB hProb,int OptionID)
{
   PCOIN pCoin = (PCOIN)hProb;
	int OptionNr;

	OptionNr = LocateOptionID(OptionID);
	if (OptionNr < 0) {
		return 0.0;
	}
	if (OptionTable[OptionNr].OptionType == OPT_REAL) {
		return OptionTable[OptionNr].CurrentValue;
	}
	return 0.0;
}



SOLVAPI int CoinSetRealOption(HPROB hProb,int OptionID, double RealValue)
{
   PCOIN pCoin = (PCOIN)hProb;
	int OptionNr;

	OptionNr = LocateOptionID(OptionID);
	if (OptionNr < 0) {
		return SOLV_FAILED;
	}
	if (OptionTable[OptionNr].OptionType == OPT_REAL) {
		OptionTable[OptionNr].CurrentValue = RealValue;
	   return SOLV_SUCCESS;
	}
	return SOLV_FAILED;
}


SOLVAPI char*  CoinGetStringOption(HPROB hProb, int OptionID)
{
   PCOIN pCoin = (PCOIN)hProb;

   return NULL;
}


SOLVAPI int    CoinSetStringOption(HPROB hProb, int OptionID, char *StringValue)
{
   PCOIN pCoin = (PCOIN)hProb;

   return SOLV_FAILED;
}


/****************************************************************/


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

