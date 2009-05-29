using System.Windows.Forms;
using Coin.CoinMP;
    
//*************************************************************************
//*   This problem is copied from coinutils\test\CoinPackedMatrixTest
//*
//*    3x1 +  x2         -  2x4 - x5               -    x8
//*          2x2 + 1.1x3
//*                   x3              +  x6
//*                       2.8x4             -1.2x7
//*  5.6x1                      + x5               + 1.9x8
//*
//*************************************************************************

namespace CoinMPTest
{
    public static class ProblemCoinTest
    {
        public static void Solve(SolveProblem solveProblem)
        {

            const int NUM_COLS = 8;
            const int NUM_ROWS = 5;
            const int NUM_NZ = 14;

            string problemName = "CoinTest";

            int colCount = NUM_COLS;
            int rowCount = NUM_ROWS;
            int nonZeroCount = NUM_NZ;
            int rangeCount = 0;
 
	        string objectName = "obj";
	        int objectSense = CoinMP.SOLV_OBJSENS_MAX;
	        double objectConst = 0.0;
	        double[] objectCoeffs = new double[NUM_COLS] { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0 };

	        double[] lowerBounds = new double[NUM_COLS] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };
	        double[] upperBounds = new double[NUM_COLS] { 1000000.0, 1000000.0, 1000000.0, 1000000.0, 
                                                          1000000.0, 1000000.0, 1000000.0, 1000000.0  };

        	char[] rowType = new char[NUM_ROWS] { 'L', 'L', 'L', 'L', 'L' };
	        double[] rhsValues = new double[NUM_ROWS] { 14.0, 80.0, 50.0, 50.0, 50.0 };

	        int[] matrixBegin = new int[NUM_COLS+1] {0, 2, 4, 6, 8, 10, 11, 12, 14};
	        int[] matrixCount = new int[NUM_COLS] {2, 2, 2, 2, 2, 1, 1, 2};
	        int[] matrixIndex = new int[NUM_NZ] {0, 4, 0, 1, 1, 2, 0, 3, 0, 4, 2, 3, 0, 4};
	        double[] matrixValues = new double[NUM_NZ] {3.0, 5.6, 1.0, 2.0, 1.1, 1.0, -2.0, 2.8, 
                                                        -1.0, 1.0, 1.0, -1.2, -1.0, 1.9};
    
	        string[] colNames = new string[NUM_COLS] {"c1", "c2", "c3", "c4", "c5", "c6", "c7", "c8"};
	        string[] rowNames = new string[NUM_ROWS] {"r1", "r2", "r3", "r4", "r5"};

        	double[] initValues = new double[NUM_COLS] { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0  };

	        double optimalValue = 1428729.2857143;

            solveProblem.RunProblem(problemName, optimalValue, colCount, rowCount,
                nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs,
                lowerBounds, upperBounds, rowType, rhsValues, null,
                matrixBegin, matrixCount, matrixIndex, matrixValues,
                colNames, rowNames, objectName, initValues, null);

       }

    }

}