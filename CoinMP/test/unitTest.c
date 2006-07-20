
/* unitTest.c */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "CoinMP.h"

int
main (int argc, char *argv[])
{
  {
  /*************************************************************************
  *   Setup data to represent this matrix by rows
  *
  *    3x1 +  x2         -  2x4 - x5               -    x8
  *          2x2 + 1.1x3
  *                   x3              +  x6               
  *                       2.8x4             -1.2x7
  *  5.6x1                      + x5               + 1.9x8
  *
    *************************************************************************/
    /*By columns */
    int nRows=5;
    int nCols=8;
    int numels=14;
    double elemBase[14]={3., 5.6, 1., 2., 1.1, 1., -2., 2.8, -1., 1., 1., -1.2, -1., 1.9};
    int indBase[14]={0,4,0,1,1,2,0,3,0,4,2,3,0,4};
    int startsBase[8+1]={0,2,4,6,8,10,11,12,14};
    int lenBase[8]={2,2,2,2,2,1,1,2};
    
    
    int rangeCount = 0;
    int objectSense = -1;
    double objectCoeffs[8] = { 1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 };
    double rhsValues[5] = { 14., 80., 50., 50., 50. };
    double rangeValues[5] = { 0., 0., 0., 0., 0. };
    char rowType[5] = { 'L', 'L', 'L', 'L', 'L' };
    
    double lowerBounds[8] = { 0., 0., 0., 0., 0., 0., 0.  };
    double upperBounds[8] = { 1000000., 1000000., 1000000., 1000000., 1000000., 1000000., 1000000.   };
    double initValues[8] = { 0., 0., 0., 0., 0., 0., 0., 0.  };
    char * colNames[8] = { "c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8" };
    char * rowNames[5] = { "r1", "r2", "r3", "r4", "r5" } ;
    
    HPROB hProb;
    
    CoinInitSolver("");
    hProb = CoinCreateProblem("CoinMPTest");
    
    CoinLoadProblem(hProb, nCols, nRows,
      numels, rangeCount,
      objectSense, objectCoeffs,
      rhsValues, rangeValues,
      rowType, startsBase,
      lenBase, indBase,
      elemBase, lowerBounds,
      upperBounds, initValues,
      colNames, rowNames );
     CoinOptimizeProblem(hProb, 0);
    
    {
      int solutionStatus = CoinGetSolutionStatus(hProb);
      char * solutionText = CoinGetSolutionText(hProb,solutionStatus);
      double objectValue =  CoinGetObjectValue(hProb);
      
      fprintf(stdout, "Solution Status: %s\n", solutionText );
      fprintf(stdout, "Solution Status: %d\n", solutionStatus);
      fprintf(stdout, "Solution Value: %lg\n", objectValue);
      
      assert(solutionStatus==0);
      assert(strcmp(solutionText,"Optimal solution found")==0);
      assert( fabs(objectValue-1428729.2857143) < 0.001 );
    }
    
    CoinUnloadProb(hProb);
    CoinFreeSolver();
  }

  /* EURO 2006 Testcase */
  {
    HPROB hProb;
    
    int colCount = 2;
    int rowCount = 3;
    int nonZeroCount = 4 ;
    int rangeCount = 0;
    int objectSense = -1;
    double objectCoeffs[2] = { 0.05 , 0.03 };
    double rhsValues[3] = { 1400 , 8000 , 5000 };
    double rangeValues[2] = { 0 , 0 };
    char rowType[2] = { 'L', 'L' };
    int matrixBegin[3] = { 0 , 2, 4 };
    int matrixCount[2] = { 2 , 2 };
    int matrixIndex[4] = { 0, 1, 0, 2};
    double matrixValues[4] = { 0.1, 1, 0.2, 1};
    double lowerBounds[2] = { 0, 0  };
    double upperBounds[2] = { 1000000, 1000000   };
    double initValues[2] = { 0, 0  };
    char * colNames[2] = { "Sun", "Moon" };
    char * rowNames[3] = { "c1", "c2", "c3" } ;
    
    CoinInitSolver("");
    hProb = CoinCreateProblem("CoinMPTest");
    
    CoinLoadProblem(hProb, colCount, rowCount,
      nonZeroCount, rangeCount,
      objectSense, objectCoeffs,
      rhsValues, rangeValues,
      rowType, matrixBegin,
      matrixCount, matrixIndex,
      matrixValues, lowerBounds,
      upperBounds, initValues,
      colNames, rowNames );
    
    CoinOptimizeProblem(hProb, 0);
    
    {
      int solutionStatus = CoinGetSolutionStatus(hProb);
      char * solutionText = CoinGetSolutionText(hProb,solutionStatus);
      double objectValue =  CoinGetObjectValue(hProb);
      
      fprintf(stdout, "Solution Status: %s\n", solutionText );
      fprintf(stdout, "Solution Status: %d\n", solutionStatus);
      fprintf(stdout, "Solution Value: %lg\n", objectValue);
      
      assert(solutionStatus==0);
    }
    
    CoinUnloadProb(hProb);
    CoinFreeSolver();
  }

  fprintf(stdout, "All tests completed successfully\n" );

}