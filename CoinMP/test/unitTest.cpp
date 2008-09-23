
/* unitTest.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "CoinMP.h"



void GetAndCheckSolution(double optimalValue, HPROB hProb)
{
	int solutionStatus;
	char solutionText[200];
	double objectValue;

	solutionStatus = CoinGetSolutionStatus(hProb);
	CoinGetSolutionText(hProb,solutionStatus, solutionText);
	objectValue =  CoinGetObjectValue(hProb);
      
	fprintf(stdout, "\n---------------------------------------\n");
	fprintf(stdout, "Solution Result: %s\n", solutionText );
	fprintf(stdout, "Solution Status: %d\n", solutionStatus);
	fprintf(stdout, "Solution Value: %lg\n", objectValue);
      
	assert(solutionStatus==0);
	assert(strcmp(solutionText,"Optimal solution found")==0);
	if (optimalValue != 0.0) {
		assert( fabs(objectValue-optimalValue) < 0.001 );
	}
	fprintf(stdout, "---------------------------------------\n\n");
}



void RunTestProblem(char *problemName, double optimalValue, int colCount, int rowCount, 
	  int nonZeroCount, int rangeCount, int objectSense, double *objectCoeffs, double objectConst, 
	  double *rhsValues, double *rangeValues, char *columnType, char *rowType, int *matrixBegin, 
	  int *matrixCount, int *matrixIndex, double *matrixValues, double *lowerBounds, double *upperBounds, 
	  double *initValues, char *colNames, char *rowNames, char *objectName)
{
	HPROB hProb;
    
	fprintf(stdout, "Solve Problem: %s\n", problemName);
	hProb = CoinCreateProblem(problemName);  
	CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount,
					objectSense, objectCoeffs, objectConst, rhsValues, rangeValues,
					rowType, matrixBegin, matrixCount, matrixIndex, matrixValues, 
					lowerBounds, upperBounds, initValues, colNames, rowNames, objectName);
	if (columnType) {
		CoinLoadInteger(hProb, columnType);
	}
	CoinOptimizeProblem(hProb, 0);
	GetAndCheckSolution(optimalValue, hProb);
	CoinUnloadProblem(hProb);
}



/*************************************************************************
*   This problem is copied from coinutils\test\CoinPackedMatrixTest
*
*    3x1 +  x2         -  2x4 - x5               -    x8
*          2x2 + 1.1x3
*                   x3              +  x6               
*                       2.8x4             -1.2x7
*  5.6x1                      + x5               + 1.9x8
*
*************************************************************************/

void SolveProblemCoinTest(void)
{
	char *problemName = "CoinTest";
	int colCount=8;
	int rowCount=5;
	int nonZeroCount=14;
	int rangeCount = 0;

	char *objectName = "obj";
	int objectSense = SOLV_OBJSENS_MAX;
	double objectConst = 0.0;
	double objectCoeffs[8] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	double rhsValues[5] = { 14., 80., 50., 50., 50. };
	char rowType[5] = { 'L', 'L', 'L', 'L', 'L' };

	int matrixBegin[8+1]={0,2,4,6,8,10,11,12,14};
	int matrixCount[8]={2,2,2,2,2,1,1,2};
	int matrixIndex[14]={0,4,0,1,1,2,0,3,0,4,2,3,0,4};
	double matrixValues[14]={3., 5.6, 1., 2., 1.1, 1., -2., 2.8, -1., 1., 1., -1.2, -1., 1.9};
    
	double lowerBounds[8] = { 0., 0., 0., 0., 0., 0., 0.  };
	double upperBounds[8] = { 1000000., 1000000., 1000000., 1000000., 1000000., 1000000., 1000000.   };

	double initValues[8] = { 0., 0., 0., 0., 0., 0., 0., 0.  };

	char *colNames = "c1\0" "c2\0" "c3\0" "c4\0" "c5\0" "c6\0" "c7\0" "c8";
	char *rowNames = "r1\0" "r2\0" "r3\0" "r4\0" "r5";
    
	double optimalValue = 1428729.2857143;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectCoeffs, objectConst, 
	  rhsValues, NULL, NULL, rowType, matrixBegin, matrixCount, 
	  matrixIndex, matrixValues, lowerBounds, upperBounds, 
	  initValues, colNames, rowNames, objectName);
}


void SolveProblemBakery(void)
{
	char *problemName = "Bakery";
	int colCount = 2;
	int rowCount = 3;
	int nonZeroCount = 4 ;
	int rangeCount = 0;

	char *objectName = "Profit";
	int objectSense = SOLV_OBJSENS_MAX;
	double objectConst = - 4000.0 / 30.0;
	double objectCoeffs[2] = { 0.05 , 0.08 };

	double rhsValues[3] = { 1400 , 8000 , 5000 };
	char rowType[3] = { 'L', 'L' , 'L' };

	int matrixBegin[2+1] = { 0 , 2, 4 };
	int matrixCount[2] = { 2 , 2 };
	int matrixIndex[4] = { 0, 1, 0, 2};
	double matrixValues[4] = { 0.1, 1, 0.2, 1};

	double lowerBounds[2] = { 0, 0  };
	double upperBounds[2] = { 1000000, 1000000   };
    
	char *colNames = "Sun\0" "Moon";
	char *rowNames = "c1\0" "c2\0" "c3";
    
	double optimalValue = 506.66666667;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectCoeffs, objectConst, 
	  rhsValues, NULL, NULL, rowType, matrixBegin, matrixCount, 
	  matrixIndex, matrixValues, lowerBounds, upperBounds, 
	  NULL, colNames, rowNames, objectName);
}


void SolveProblemAfiro(void)
{
	char *probname = "Afiro";
	int ncol = 32;
	int nrow = 27;
	int nels = 83;
	int nrng = 0;

	char *objectname = "COST";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;

	double dobj[32]={0, -0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.32, 0, 0, 0, -0.6, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.48, 0, 0, 10};

	double drhs[27]={0, 0, 80, 0, 0, 0, 80, 0, 0, 0, 0, 0, 500, 0, 0, 44, 500, 0, 
		0, 0, 0, 0, 0, 0, 0, 310, 300};

	char rtyp[27]= {'E', 'E', 'L', 'L', 'E', 'E', 'L', 'L', 'L', 'L', 'E', 'E', 'L', 
		'L', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'};

	int mbeg[32+1]={0, 4, 6, 8, 10, 14, 18, 22, 26, 28, 30, 32, 34, 36, 38, 40, 
		44, 46, 48, 50, 52, 56, 60, 64, 68, 70, 72, 74, 76, 78, 80, 82, 83};

	int mcnt[32]={4, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4, 
		4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 1};

	int midx[83]={0, 1, 2, 23, 0, 3, 0, 21, 1, 25, 4, 5, 6, 24, 4, 5, 7, 24, 4, 5, 
		8, 24, 4, 5, 9, 24, 6, 20, 7, 20, 8, 20, 9, 20, 3, 4, 4, 22, 5, 26, 10, 11, 
		12, 21, 10, 13, 10, 23, 10, 20, 11, 25, 14, 15, 16, 22, 14, 15, 17, 22, 14, 
		15, 18, 22, 14, 15, 19, 22, 16, 20, 17, 20, 18, 20, 19, 20, 13, 15, 15, 24, 
		14, 26, 15};

	double mval[83]={-1, -1.06, 1, 0.301, 1, -1, 1, -1, 1, 1, -1, -1.06, 1, 0.301, 
		-1, -1.06, 1, 0.313, -1, -0.96, 1, 0.313, -1, -0.86, 1, 0.326, -1, 2.364, -1, 
		2.386, -1, 2.408, -1, 2.429, 1.4, 1, 1, -1, 1, 1, -1, -0.43, 1, 0.109, 1, -1, 
		1, -1, 1, -1, 1, 1, -0.43, 1, 1, 0.109, -0.43, 1, 1, 0.108, -0.39, 1, 1, 
		0.108, -0.37, 1, 1, 0.107, -1, 2.191, -1, 2.219, -1, 2.249, -1, 2.279, 1.4, 
		-1, 1, -1, 1, 1, 1};

	double dclo[32]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	double dcup[32]={1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037};

	char *colnames = "X01\0" "X02\0" "X03\0" "X04\0" "X06\0" "X07\0" "X08\0" "X09\0" 
		"X10\0" "X11\0" "X12\0" "X13\0" "X14\0" "X15\0" "X16\0" "X22\0" "X23\0" "X24\0" "X25\0" 
		"X26\0" "X28\0" "X29\0" "X30\0" "X31\0" "X32\0" "X33\0" "X34\0" "X35\0" "X36\0" "X37\0" 
		"X38\0" "X39";

	char *rownames = "R09\0" "R10\0" "X05\0" "X21\0" "R12\0" "R13\0" "X17\0" "X18\0" 
		"X19\0" "X20\0" "R19\0" "R20\0" "X27\0" "X44\0" "R22\0" "R23\0" "X40\0" "X41\0" "X42\0" 
		"X43\0" "X45\0" "X46\0" "X47\0" "X48\0" "X49\0" "X50\0" "X51";

	double optimalValue = -464.753142857;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, NULL, NULL, rtyp, mbeg, mcnt, 
	  midx, mval, dclo, dcup, NULL, colnames, rownames, objectname);
}


void SolveProblemP0033(void)
{
	char *probname = "P0033";
	int ncol = 33;
	int nrow = 15;
	int nels = 98;
	int nrng = 0;

	char *objectName = "OBJ";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;

	double dobj[33]={171, 171, 171, 171, 163, 162, 163, 69, 69, 183, 183, 183, 
		183, 49, 183, 258, 517, 250, 500, 250, 500, 159, 318, 159, 318, 159, 318, 159, 
		318, 114, 228, 159, 318};

	double drhs[15]={1, 1, 1, 1, -5, 2700, -2600, -100, -900, -1656, -335, -1026, 
		-5, -500, -270};

	char rtyp[15]= { 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L' };

	int mbeg[33+1]={0, 3, 6, 10, 14, 19, 24, 26, 31, 36, 38, 41, 45, 49, 53, 54, 
		55, 56, 58, 60, 61, 62, 66, 70, 73, 76, 80, 84, 87, 90, 93, 96, 97, 98};

	int mcnt[33]={3, 3, 4, 4, 5, 5, 2, 5, 5, 2, 3, 4, 4, 4, 1, 1, 1, 2, 2, 1, 1, 
		4, 4, 3, 3, 4, 4, 3, 3, 3, 3, 1, 1};

	int midx[98]={0, 8, 9, 0, 12, 13, 0, 5, 6, 9, 0, 5, 6, 7, 1, 5, 6, 10, 11, 1, 
		5, 6, 8, 9, 1, 14, 2, 5, 6, 10, 11, 2, 5, 6, 8, 9, 3, 4, 3, 10, 11, 3, 5, 6, 
		11, 3, 5, 6, 9, 5, 6, 8, 9, 3, 4, 4, 12, 13, 12, 13, 13, 13, 5, 6, 10, 11, 5, 
		6, 10, 11, 5, 6, 11, 5, 6, 11, 5, 6, 8, 9, 5, 6, 8, 9, 5, 6, 9, 5, 6, 9, 5, 6, 
		7, 5, 6, 7, 14, 14};

	double mval[98]={1, -300, -300, 1, -300, -300, 1, 300, -300, -300, 1, 300, 
		-300, -300, 1, 285, -285, -285, -285, 1, 285, -285, -285, -285, 1, -285, 1, 
		265, -265, -265, -265, 1, 265, -265, -265, -265, 1, -230, 1, -230, -230, 1, 
		230, -230, -230, 1, 230, -230, -230, 190, -190, -190, -190, 1, -200, -400, 
		-200, -200, -400, -400, -200, -400, 200, -200, -200, -200, 400, -400, -400, 
		-400, 200, -200, -200, 400, -400, -400, 200, -200, -200, -200, 400, -400, 
		-400, -400, 200, -200, -200, 400, -400, -400, 200, -200, -200, 400, -400, 
		-400, -200, -400};

	double dclo[33]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	double dcup[33]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	char *colnames = "C157\0" "C158\0" "C159\0" "C160\0" "C161\0" "C162\0" "C163\0" 
		"C164\0" "C165\0" "C166\0" "C167\0" "C168\0" "C169\0" "C170\0" "C171\0" "C172\0" 
		"C173\0" "C174\0" "C175\0" "C176\0" "C177\0" "C178\0" "C179\0" "C180\0" "C181\0" 
		"C182\0" "C183\0" "C184\0" "C185\0" "C186\0" "C187\0" "C188\0" "C189";

	char *rownames = "R114\0" "R115\0" "R116\0" "R117\0" "R118\0" "R119\0" "R120\0" 
		"R121\0" "R122\0" "R123\0" "R124\0" "R125\0" "R126\0" "R127\0" "R128";

	char ctyp[33] = { 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B' };

	double optimalValue = 3089.0;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, NULL, ctyp, rtyp, mbeg, mcnt, 
	  midx, mval, dclo, dcup, NULL, colnames, rownames, objectName);
}


void SolveProblemExmip1(void)
{
	char *probname = "Exmip1";
	int ncol = 8;
	int nrow = 5;
	int nels = 14;
	int nrng = 2;
	
	char *objectName = "z";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;
	double dobj[8]={1, 0, 0, 0, 2, 0, 0, -1};

	double drhs[5]={2.5, 2.1, 4, 1.8, 15};
	double drng[5]={0, 0, 0, -3.2, 12};
	char rtyp[5]= {'G', 'L', 'E', 'G', 'L'};

	int mbeg[8+1]={0, 2, 4, 6, 8, 10, 11, 12, 14};
	int mcnt[8]={2, 2, 2, 2, 2, 1, 1, 2};
	int midx[14]={0, 4, 0, 1, 1, 2, 0, 3, 0, 4, 2, 3, 0, 4};
	double mval[14]={3, 5.6, 1, 2, 1.1, 1, -2, 2.8, -1, 1, 1, -1.2, -1, 1.9};

	double dclo[8]={2.5, 0, 0, 0, 0.5, 0, 0, 0};
	double dcup[8]={1e+037, 4.1, 1e+037, 1e+037, 4, 1e+037, 1e+037, 4.3};

	char *colnames = "COL01\0" "COL02\0" "COL03\0" "COL04\0" "COL05\0" "COL06\0" 
		"COL07\0" "COL08";
	char *rownames = "ROW01\0" "ROW02\0" "ROW03\0" "ROW04\0" "ROW05";

	char ctyp[8] = { 'C', 'C', 'B', 'B', 'C', 'C', 'C', 'C'};

	double optimalValue = 3.23684210526;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, drng, ctyp, rtyp, mbeg, mcnt, 
	  midx, mval, dclo, dcup, NULL, colnames, rownames, objectName);
}



int main (int argc, char *argv[])
{
	char SolverName[100];
	double CoinVersion;

	CoinInitSolver("");
	CoinGetSolverName(SolverName);
	CoinVersion = CoinGetVersion();
	fprintf(stdout, "UnitTest: %s version %lg\n\n", SolverName, CoinVersion);
	SolveProblemCoinTest();
	SolveProblemBakery();
	SolveProblemAfiro();
	SolveProblemP0033();
	SolveProblemExmip1();
	fprintf(stdout, "All unit tests completed successfully\n" );
	CoinFreeSolver();
	return 0;
}
