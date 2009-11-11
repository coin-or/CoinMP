
/* example.c */

// $Id$

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "CoinMP.h"


int SOLVCALL MsgLogCallback(char* MessageStr)
{
	fprintf(stdout, "%s", MessageStr);
	return 0;
}

int SOLVCALL IterCallback(int    IterCount, 
			double ObjectValue,
			int    IsFeasible, 
			double InfeasValue)
{
	fprintf(stdout, "ITER: iter=%d, obj=%lg, feas=%d, infeas=%lg\n",
		IterCount, ObjectValue, IsFeasible, InfeasValue);
	return 0;
}

int SOLVCALL MipNodeCallback(int    IterCount, 
				int	  MipNodeCount,
				double BestBound,
				double BestInteger,
				int    IsMipImproved)
{
	fprintf(stdout, "NODE: iter=%d, node=%d, bound=%lg, best=%lg, %s\n",
		IterCount, MipNodeCount, BestBound, BestInteger, IsMipImproved ? "Improved" : "");
	return 0;
}


void GetAndCheckSolution(double optimalValue, HPROB hProb)
{
	int solutionStatus;
	const char* solutionText;
	double objectValue;
	int i;
	int colCount;
	double* xValues;
	const char* ColName;
	const char* problemName;

	problemName = CoinGetProblemName(hProb);
	solutionStatus = CoinGetSolutionStatus(hProb);
	solutionText = CoinGetSolutionText(hProb,solutionStatus);
	objectValue =  CoinGetObjectValue(hProb);
      
	fprintf(stdout, "\n---------------------------------------\n");
	fprintf(stdout, "Problem Name:    %s\n", problemName);
	fprintf(stdout, "Solution Result: %s\n", solutionText );
	fprintf(stdout, "Solution Status: %d\n", solutionStatus);
	fprintf(stdout, "Optimal Value:   %lg\n", objectValue);
	fprintf(stdout, "---------------------------------------\n");

	colCount = CoinGetColCount(hProb);
	xValues = (double* )malloc(colCount * sizeof(double));
	CoinGetSolutionValues(hProb, xValues, NULL, NULL, NULL);
	for (i = 0; i < colCount; i++) {
		if (xValues[i] != 0.0) {
			ColName = CoinGetColName(hProb, i);
			fprintf(stdout, "%s = %lg\n", ColName, xValues[i]);
		}
	}
	fprintf(stdout, "---------------------------------------\n\n");
	assert(solutionStatus==0);
	assert(strcmp(solutionText,"Optimal solution found")==0);
	if (optimalValue != 0.0) {
		assert( fabs(objectValue-optimalValue) < 0.001 );
	}
}



void RunTestProblem(char* problemName, double optimalValue, int colCount, int rowCount, 
	  int nonZeroCount, int rangeCount, int objectSense, double objectConst, double* objectCoeffs, 
	  double* lowerBounds, double* upperBounds, char* rowType, double* rhsValues, double* rangeValues, 
	  int* matrixBegin, int* matrixCount, int* matrixIndex, double* matrixValues, char** colNames, 
	  char** rowNames, char* objectName, double* initValues, char* columnType)
{
	HPROB hProb;
	int result;
	char filename[260];
    
	fprintf(stdout, "Solve Problem: %s (obj=%lg)\n", problemName, optimalValue);
	hProb = CoinCreateProblem(problemName);  
	result = CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount,
					objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds, 
					rowType, rhsValues, rangeValues, matrixBegin, matrixCount, 
					matrixIndex, matrixValues, colNames, rowNames, objectName);
	if (columnType) {
		result = CoinLoadInteger(hProb, columnType);
	}
	result = CoinCheckProblem(hProb);
	if (result != SOLV_CALL_SUCCESS) {
		fprintf(stdout, "Check Problem failed (result = %d)\n", result);
	}
	//result = CoinSetMsgLogCallback(hProb, &MsgLogCallback);
	if (columnType == NULL)
		result = CoinSetIterCallback(hProb, &IterCallback);
	else {
		result = CoinSetMipNodeCallback(hProb, &MipNodeCallback);
	}
	result = CoinOptimizeProblem(hProb, 0);
	strcpy(filename, problemName);
	strcat(filename, ".mps");
	result = CoinWriteFile(hProb, SOLV_FILE_MPS, filename);
	GetAndCheckSolution(optimalValue, hProb);
	CoinUnloadProblem(hProb);
}


void RunTestProblemBuf(char* problemName, double optimalValue, int colCount, int rowCount, 
	  int nonZeroCount, int rangeCount, int objectSense, double objectConst, double* objectCoeffs, 
	  double* lowerBounds, double* upperBounds, char* rowType, double* rhsValues, double* rangeValues, 
	  int* matrixBegin, int* matrixCount, int* matrixIndex, double* matrixValues, char* colNamesBuf, 
	  char* rowNamesBuf, char* objectName, double* initValues, char* columnType)
{
	HPROB hProb;
	int result;
	char filename[260];
    
	fprintf(stdout, "Solve Problem: %s (obj=%lg)\n", problemName, optimalValue);
	hProb = CoinCreateProblem(problemName);  
	result = CoinLoadProblemBuf(hProb, colCount, rowCount, nonZeroCount, rangeCount,
					objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds, 
					rowType, rhsValues, rangeValues, matrixBegin, matrixCount, 
					matrixIndex, matrixValues, colNamesBuf, rowNamesBuf, objectName);
	if (columnType) {
		result = CoinLoadInteger(hProb, columnType);
	}
	result = CoinCheckProblem(hProb);
	if (result != SOLV_CALL_SUCCESS) {
		fprintf(stdout, "Check Problem failed (result = %d)\n", result);
	}
	//result = CoinSetMsgLogCallback(hProb, &MsgLogCallback);
	if (columnType == NULL)
		result = CoinSetIterCallback(hProb, &IterCallback);
	else {
		result = CoinSetMipNodeCallback(hProb, &MipNodeCallback);
	}
	result = CoinOptimizeProblem(hProb, 0);
	strcpy(filename, problemName);
	strcat(filename, ".mps");
	result = CoinWriteFile(hProb, SOLV_FILE_MPS, filename);
	GetAndCheckSolution(optimalValue, hProb);
	CoinUnloadProblem(hProb);
}


void RunSosTestProblem(char* problemName, double optimalValue, int colCount, int rowCount, 
	  int nonZeroCount, int rangeCount, int objectSense, double objectConst, double* objectCoeffs, 
	  double* lowerBounds, double* upperBounds, char* rowType, double* rhsValues, double* rangeValues, 
	  int* matrixBegin, int* matrixCount, int* matrixIndex, double* matrixValues, char** colNames, 
	  char** rowNames, char* objectName, double* initValues, char* columnType, int sosCount, 
	  int sosNZCount, int* sosType, int* sosPrior, int* sosBegin, int* sosIndex, double* sosRef)
{
	HPROB hProb;
	int result;
	char filename[260];
    
	fprintf(stdout, "Solve Problem: %s (obj=%lg)\n", problemName, optimalValue);
	hProb = CoinCreateProblem(problemName);
	result = CoinLoadProblem(hProb, colCount, rowCount, nonZeroCount, rangeCount,
					objectSense, objectConst, objectCoeffs, lowerBounds, upperBounds, 
					rowType, rhsValues, rangeValues, matrixBegin, matrixCount, 
					matrixIndex, matrixValues, colNames, rowNames, objectName);
	if (columnType) {
		result = CoinLoadInteger(hProb, columnType);
	}
	result = CoinLoadSos(hProb, sosCount, sosNZCount, sosType, sosPrior, sosBegin, sosIndex, sosRef);
	result = CoinCheckProblem(hProb);
	if (result != SOLV_CALL_SUCCESS) {
		fprintf(stdout, "Check Problem failed (result = %d)\n", result);
	}
	//result = CoinSetMsgLogCallback(hProb, &MsgLogCallback);
	if ((columnType == NULL) && (sosCount == 0))
		result = CoinSetIterCallback(hProb, &IterCallback);
	else {
		result = CoinSetMipNodeCallback(hProb, &MipNodeCallback);
	}
	strcpy(filename, problemName);
	strcat(filename, ".mps");
	result = CoinWriteFile(hProb, SOLV_FILE_MPS, filename);
	result = CoinOptimizeProblem(hProb, 0);
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
	char* problemName = "CoinTest";
	int colCount=8;
	int rowCount=5;
	int nonZeroCount=14;
	int rangeCount = 0;

	char* objectName = "obj";
	int objectSense = SOLV_OBJSENS_MAX;
	double objectConst = 0.0;
	double objectCoeffs[8] = { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	double lowerBounds[8] = { 0., 0., 0., 0., 0., 0., 0. };
	double upperBounds[8] = { 1000000., 1000000., 1000000., 1000000., 1000000., 1000000., 1000000., 1000000. };

	char rowType[5] = { 'L', 'L', 'L', 'L', 'L' };
	double rhsValues[5] = { 14., 80., 50., 50., 50. };

	int matrixBegin[8+1]={0,2,4,6,8,10,11,12,14};
	int matrixCount[8]={2,2,2,2,2,1,1,2};
	int matrixIndex[14]={0,4,0,1,1,2,0,3,0,4,2,3,0,4};
	double matrixValues[14]={3., 5.6, 1., 2., 1.1, 1., -2., 2.8, -1., 1., 1., -1.2, -1., 1.9};
    
	char* colNames[8] = {"c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8"};
	char* rowNames[5] = {"r1", "r2", "r3", "r4", "r5"};

	//char* colNamesBuf = "c1\0" "c2\0" "c3\0" "c4\0" "c5\0" "c6\0" "c7\0" "c8";
	//char* rowNamesBuf = "r1\0" "r2\0" "r3\0" "r4\0" "r5";
    
	double initValues[8] = { 0., 0., 0., 0., 0., 0., 0., 0.  };

	double optimalValue = 1428729.2857143;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs, 
	  lowerBounds, upperBounds, rowType, rhsValues, NULL, 
	  matrixBegin, matrixCount, matrixIndex, matrixValues, 
	  colNames, rowNames, objectName, initValues, NULL);
}


void SolveProblemBakery(void)
{
	char* problemName = "Bakery";
	int colCount = 2;
	int rowCount = 3;
	int nonZeroCount = 4 ;
	int rangeCount = 0;

	char* objectName = "Profit";
	int objectSense = SOLV_OBJSENS_MAX;
	double objectConst = - 4000.0 / 30.0;
	double objectCoeffs[2] = { 0.05 , 0.08 };

	double lowerBounds[2] = { 0, 0 };
	double upperBounds[2] = { 1000000, 1000000 };

	char rowType[3] = { 'L', 'L' , 'L' };
	double rhsValues[3] = { 1400 , 8000 , 5000 };

	int matrixBegin[2+1] = { 0 , 2, 4 };
	int matrixCount[2] = { 2 , 2 };
	int matrixIndex[4] = { 0, 1, 0, 2};
	double matrixValues[4] = { 0.1, 1, 0.2, 1};

	char* colNames[2] = {"Sun", "Moon"};
	char* rowNames[3] = {"c1", "c2", "c3"};

	//char* colNamesBuf = "Sun\0" "Moon";
	//char* rowNamesBuf = "c1\0" "c2\0" "c3";
    
	double optimalValue = 506.66666667;

	RunTestProblem(problemName, optimalValue, colCount, rowCount, 
	  nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs, 
	  lowerBounds, upperBounds, rowType, rhsValues, NULL, 
	  matrixBegin, matrixCount, matrixIndex, matrixValues, 
	  colNames, rowNames, objectName, NULL, NULL);
}


void SolveProblemAfiro(void)
{
	char* probname = "Afiro";
	int ncol = 32;
	int nrow = 27;
	int nels = 83;
	int nrng = 0;

	char* objectname = "Cost";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;

	double dobj[32]={0, -0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.32, 0, 0, 0, -0.6, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.48, 0, 0, 10};

	double dclo[32]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	double dcup[32]={1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 
		1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037, 1e+037};

	char rtyp[27]= {'E', 'E', 'L', 'L', 'E', 'E', 'L', 'L', 'L', 'L', 'E', 'E', 'L', 
		'L', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'};

	double drhs[27]={0, 0, 80, 0, 0, 0, 80, 0, 0, 0, 0, 0, 500, 0, 0, 44, 500, 0, 
		0, 0, 0, 0, 0, 0, 0, 310, 300};

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

	char* colnames[32] = {"x01", "x02", "x03", "x04", "x06", "x07", "x08", "x09", 
		"x10", "x11", "x12", "x13", "x14", "x15", "x16", "x22", "x23", "x24", "x25", 
		"x26", "x28", "x29", "x30", "x31", "x32", "x33", "x34", "x35", "x36", "x37", 
		"x38", "x39"};

	char* rownames[27] = {"r09", "r10", "x05", "x21", "r12", "r13", "x17", "x18", 
		"x19", "x20", "r19", "r20", "x27", "x44", "r22", "r23", "x40", "x41", "x42", 
		"x43", "x45", "x46", "x47", "x48", "x49", "x50", "x51"};

	/*char* colnamesBuf = "x01\0" "x02\0" "x03\0" "x04\0" "x06\0" "x07\0" "x08\0" "x09\0" 
		"x10\0" "x11\0" "x12\0" "x13\0" "x14\0" "x15\0" "x16\0" "x22\0" "x23\0" "x24\0" "x25\0" 
		"x26\0" "x28\0" "x29\0" "x30\0" "x31\0" "x32\0" "x33\0" "x34\0" "x35\0" "x36\0" "x37\0" 
		"x38\0" "x39";*/

	/*char* rownamesBuf = "r09\0" "r10\0" "x05\0" "x21\0" "r12\0" "r13\0" "x17\0" "x18\0" 
		"x19\0" "x20\0" "r19\0" "r20\0" "x27\0" "x44\0" "r22\0" "r23\0" "x40\0" "x41\0" "x42\0" 
		"x43\0" "x45\0" "x46\0" "x47\0" "x48\0" "x49\0" "x50\0" "x51";*/

	double optimalValue = -464.753142857;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, objconst, dobj, dclo, dcup, rtyp, drhs, NULL, mbeg, 
	  mcnt, midx, mval, colnames, rownames, objectname, NULL, NULL);
}


void SolveProblemP0033(void)
{
	char* probname = "P0033";
	int ncol = 33;
	int nrow = 15;
	int nels = 98;
	int nrng = 0;

	char* objectname = "Obj";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;

	double dobj[33]={171, 171, 171, 171, 163, 162, 163, 69, 69, 183, 183, 183, 
		183, 49, 183, 258, 517, 250, 500, 250, 500, 159, 318, 159, 318, 159, 318, 159, 
		318, 114, 228, 159, 318};

	double dclo[33]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	double dcup[33]={1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

	char rtyp[15]= { 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L' };

	double drhs[15]={1, 1, 1, 1, -5, 2700, -2600, -100, -900, -1656, -335, -1026, 
		-5, -500, -270};

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

	char* colnames[33] = {"c157", "c158", "c159", "c160", "c161", "c162", "c163", 
		"c164", "c165", "c166", "c167", "c168", "c169", "c170", "c171", "c172", 
		"c173", "c174", "c175", "c176", "c177", "c178", "c179", "c180", "c181", 
		"c182", "c183", "c184", "c185", "c186", "c187", "c188", "c189"};

	char* rownames[15] = {"r114", "r115", "r116", "r117", "r118", "r119", "r120", 
		"r121", "r122", "r123", "r124", "r125", "r126", "r127", "r128"};

	/*char* colnamesBuf = "c157\0" "c158\0" "c159\0" "c160\0" "c161\0" "c162\0" "c163\0" 
		"c164\0" "c165\0" "c166\0" "c167\0" "c168\0" "c169\0" "c170\0" "c171\0" "c172\0" 
		"c173\0" "c174\0" "c175\0" "c176\0" "c177\0" "c178\0" "c179\0" "c180\0" "c181\0" 
		"c182\0" "c183\0" "c184\0" "c185\0" "c186\0" "c187\0" "c188\0" "c189";*/

	/*char* rownamesBuf = "r114\0" "r115\0" "r116\0" "r117\0" "r118\0" "r119\0" "r120\0" 
		"r121\0" "r122\0" "r123\0" "r124\0" "r125\0" "r126\0" "r127\0" "r128";*/

	char ctyp[33] = { 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		'B', 'B', 'B', 'B', 'B', 'B' };

	double optimalValue = 3089.0;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, objconst, dobj, dclo, dcup, rtyp, drhs, NULL, mbeg, 
	  mcnt, midx, mval, colnames, rownames, objectname, NULL, ctyp);
}


void SolveProblemExmip1(void)
{
	char* probname = "Exmip1";
	int ncol = 8;
	int nrow = 5;
	int nels = 14;
	int nrng = 2;
	
	char* objectname = "z";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;
	double dobj[8]={1, 0, 0, 0, 2, 0, 0, -1};

	double dclo[8]={2.5, 0, 0, 0, 0.5, 0, 0, 0};
	double dcup[8]={1e+037, 4.1, 1e+037, 1e+037, 4, 1e+037, 1e+037, 4.3};

	char rtyp[5]= {'G', 'L', 'E', 'G', 'L'};
	double drhs[5]={2.5, 2.1, 4, 1.8, 15};
	double drng[5]={0, 0, 0, -3.2, 12};

	int mbeg[8+1]={0, 2, 4, 6, 8, 10, 11, 12, 14};
	int mcnt[8]={2, 2, 2, 2, 2, 1, 1, 2};
	int midx[14]={0, 4, 0, 1, 1, 2, 0, 3, 0, 4, 2, 3, 0, 4};
	double mval[14]={3, 5.6, 1, 2, 1.1, 1, -2, 2.8, -1, 1, 1, -1.2, -1, 1.9};

	char* colnames[8] = {"col01", "col02", "col03", "col04", "col05", "col06", 
		"col07", "col08"};
	char* rownames[5] = {"row01", "row02", "row03", "row04", "row05"};

	/*char* colnamesBuf = "col01\0" "col02\0" "col03\0" "col04\0" "col05\0" "col06\0" 
		"col07\0" "col08";*/
	/*char* rownamesBuf = "row01\0" "row02\0" "row03\0" "row04\0" "row05";*/

	char ctyp[8] = { 'C', 'C', 'B', 'B', 'C', 'C', 'C', 'C'};

	double optimalValue = 3.23684210526;

	RunTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, objconst, dobj, dclo, dcup, rtyp, drhs, drng, mbeg, 
	  mcnt, midx, mval, colnames, rownames, objectname, NULL, ctyp);
}



void SolveProblemGamsSos1a(void)
{
	char* probname = "GamsSos1a";
	int ncol = 3;
	int nrow = 1;
	int nels = 3;
	int nrng = 0;
	
	char* objectname = "z";
	int objsens = SOLV_OBJSENS_MAX;
	double objconst = 0.0;
	double dobj[3]={0.9, 1.0, 1.1};

	double dclo[3]={0, 0, 0};
	double dcup[3]={0.8, 0.6, 0.6};

	char* rtyp = NULL;
	double drlo[1]={-1e+37};
	double drup[1]={1.0};

	int mbeg[3+1]={0, 1, 2, 3};
	int mcnt[3]={1, 1, 1};
	int midx[3]={0, 0, 0};
	double mval[3]={1, 1, 1};

	char* colnames[3] = {"x1", "x2", "x3"};
	char* rownames[1] = {"xsum"};

	//char* colnamesBuf = "x1\0" "x2\0" "x3";
	//char* rownamesBuf = "xsum";

	int sosCount = 1;
	int sosNZCount = 3;
	int sosType[1] = {1};
	int sosBegin[1+1] = {0, 3};
	int sosIndex[3] = {0, 1, 2};

	double optimalValue = 0.72;

	RunSosTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, objconst, dobj, dclo, dcup, NULL, drlo, drup, mbeg, 
	  mcnt, midx, mval, colnames, rownames, objectname, NULL, NULL,
	  sosCount, sosNZCount, sosType, NULL, sosBegin, sosIndex, NULL);
}


void SolveProblemGamsSos2a(void)
{
	char* probname = "GamsSos2a";
	int ncol = 7;
	int nrow = 5;
	int nels = 15;
	int nrng = 0;
	
	char* objectname = "z";
	int objsens = SOLV_OBJSENS_MIN;
	double objconst = 0.0;
	double dobj[7]={0, 0, 0, 0, 0, 1, 1};

	double dclo[7]={0.0, 0, 0, -1e+37, -1e+37, 0, 0};
	double dcup[7]={1e+37, 1e+37, 1e+37, 1e+37, 1e+37, 1e+37, 1e+37};

	char rtyp[5]= {'E', 'E', 'E', 'G', 'G'};
	double drhs[5]={1, 0, 0, -1.3, 1.3};

	int mbeg[7+1]={0, 3, 6, 9, 10, 13, 14, 15};
	int mcnt[7]={3, 3, 3, 1, 3, 1, 1};
	int midx[15]={0, 1, 2, 0, 1, 2, 0, 1, 2, 1, 2, 3, 4, 3, 4};
	double mval[15]={1, 1, 1, 1, 2, 2, 1, 3, 3, -1, -1, -1, 1, 1, 1};

	char* colnames[7] = {"w1", "w2", "w3", "x", "fx", "fplus", "fminus"};
	char* rownames[5] = {"wsum", "xdef", "fxdef", "gapplus", "gapminus"};

	//char* colnamesBuf = "w1\0" "w2\0" "w3\0" "x\0" "fx\0" "fplus\0" "fminus";
	//char* rownamesBuf = "wsum\0" "xdef\0" "fxdef\0" "gapplus\0" "gapminus";

	int sosCount = 1;
	int sosNZCount = 3;
	int sosType[1] = {2};
	int sosBegin[1+1] = {0, 3};
	int sosIndex[3] = {0, 1, 2};

	double optimalValue = 0.0;

	RunSosTestProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	  objsens, objconst, dobj, dclo, dcup, rtyp, drhs, NULL, mbeg, 
	  mcnt, midx, mval, colnames, rownames, objectname, NULL, NULL,
	  sosCount, sosNZCount, sosType, NULL, sosBegin, sosIndex, NULL);
}



int main (int argc, char* argv[])
{
	const char* SolverName;
	double CoinVersion;

	CoinInitSolver("");
	SolverName = CoinGetSolverName();
	CoinVersion = CoinGetVersion();
	fprintf(stdout, "UnitTest: %s version %lg\n\n", SolverName, CoinVersion);
	SolveProblemCoinTest();
	SolveProblemBakery();
	SolveProblemAfiro();
	SolveProblemP0033();
	SolveProblemExmip1();
	SolveProblemGamsSos1a();
	SolveProblemGamsSos2a();
	fprintf(stdout, "All unit tests completed successfully\n" );
	CoinFreeSolver();
	return 0;
}
