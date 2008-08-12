
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
	char *solutionText;
	double objectValue;

	solutionStatus = CoinGetSolutionStatus(hProb);
	solutionText = CoinGetSolutionText(hProb,solutionStatus);
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
	  double *initValues, char **colNames, char **rowNames)
{
    HPROB hProb;
    
	fprintf(stdout, "Solve Problem: %s\n", problemName);
    hProb = CoinCreateProblem(problemName);  
    CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount,
					objectSense, objectCoeffs, objectConst, rhsValues, rangeValues,
					rowType, matrixBegin, matrixCount, matrixIndex, matrixValues, 
					lowerBounds, upperBounds, initValues, colNames, rowNames );
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
    char * colNames[8] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8" };
    char * rowNames[5] = { "r1", "r2", "r3", "r4", "r5" } ;
    
	double optimalValue = 1428729.2857143;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectCoeffs, objectConst, 
      rhsValues, NULL, NULL, rowType, matrixBegin, matrixCount, 
	  matrixIndex, matrixValues, lowerBounds, upperBounds, 
	  initValues, colNames, rowNames);
}


void SolveProblemBakery(void)
{
	char *problemName = "Bakery";
    int colCount = 2;
    int rowCount = 3;
    int nonZeroCount = 4 ;
    int rangeCount = 0;

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
    char * colNames[2] = { "Sun", "Moon" };
    char * rowNames[3] = { "c1", "c2", "c3" } ;
    
	double optimalValue = 506.66666667;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectCoeffs, objectConst, 
      rhsValues, NULL, NULL, rowType, matrixBegin, matrixCount, 
      matrixIndex, matrixValues, lowerBounds, upperBounds, 
	  NULL, colNames, rowNames);
}


void SolveProblemAfiro(void)
{
	char *probname = "Afiro";
	int ncol = 32;
	int nrow = 27;
	int nels = 83;
	int nrng = 0;

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

	char *colname[32]={"X01", "X02", "X03", "X04", "X06", "X07", "X08", "X09", 
		"X10", "X11", "X12", "X13", "X14", "X15", "X16", "X22", "X23", "X24", "X25", 
		"X26", "X28", "X29", "X30", "X31", "X32", "X33", "X34", "X35", "X36", "X37", 
		"X38", "X39"};

	char *rowname[27]={"R09", "R10", "X05", "X21", "R12", "R13", "X17", "X18", 
		"X19", "X20", "R19", "R20", "X27", "X44", "R22", "R23", "X40", "X41", "X42", 
		"X43", "X45", "X46", "X47", "X48", "X49", "X50", "X51"};

	double optimalValue = -464.753142857;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, NULL, NULL, rtyp, mbeg, mcnt, 
      midx, mval, dclo, dcup, NULL, colname, rowname);
}


void SolveProblemP0033(void)
{
	char *probname = "P0033";
	int ncol = 33;
	int nrow = 15;
	int nels = 98;
	int nrng = 0;

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

	char *colname[33]={"C157", "C158", "C159", "C160", "C161", "C162", "C163", 
		"C164", "C165", "C166", "C167", "C168", "C169", "C170", "C171", "C172", 
		"C173", "C174", "C175", "C176", "C177", "C178", "C179", "C180", "C181", 
		"C182", "C183", "C184", "C185", "C186", "C187", "C188", "C189"};

	char *rowname[15]={"R114", "R115", "R116", "R117", "R118", "R119", "R120", 
		"R121", "R122", "R123", "R124", "R125", "R126", "R127", "R128"};

	char ctyp[33] = { 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B' };

	double optimalValue = 3089.0;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, NULL, ctyp, rtyp, mbeg, mcnt, 
      midx, mval, dclo, dcup, NULL, colname, rowname);
}


void SolveProblemExmip1(void)
{
	char *probname = "Exmip1";
	int ncol = 8;
	int nrow = 5;
	int nels = 14;
	int nrng = 2;
	
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

	char *colname[8]={"COL01", "COL02", "COL03", "COL04", "COL05", "COL06", 
		"COL07", "COL08"};
	char *rowname[5]={"ROW01", "ROW02", "ROW03", "ROW04", "ROW05"};

	char ctyp[8] = { 'C', 'C', 'B', 'B', 'C', 'C', 'C', 'C'};

	double optimalValue = 3.23684210526;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, dobj, objconst, drhs, drng, ctyp, rtyp, mbeg, mcnt, 
      midx, mval, dclo, dcup, NULL, colname, rowname);
}



int main (int argc, char *argv[])
{
    CoinInitSolver("");
	fprintf(stdout, "UnitTest: %s version %lg\n\n",CoinGetSolverName(), CoinGetVersion());
	SolveProblemCoinTest();
	SolveProblemBakery();
	SolveProblemAfiro();
	SolveProblemP0033();
	SolveProblemExmip1();
	fprintf(stdout, "All unit tests completed successfully\n" );
    CoinFreeSolver();
	return 0;
}
