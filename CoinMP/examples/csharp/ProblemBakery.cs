using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemBakery
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 2;
            const int NUM_ROWS = 3;
            const int NUM_NZ = 4;

            string problemName = "Bakery";
            int colCount = NUM_COLS;
            int rowCount = NUM_ROWS;
            int nonZeroCount = NUM_NZ;
            int rangeCount = 0;

            string objectName = "Profit";
            int objectSense = CoinMP.SOLV_OBJSENS_MAX;
            double objectConst = -4000.0 / 30.0;
            double[] objectCoeffs = new double[NUM_COLS] { 0.05, 0.08 };

            double[] lowerBounds = new double[NUM_COLS] { 0.0, 0.0 };
            double[] upperBounds = new double[NUM_COLS] { 1000000.0, 1000000.0 };

            char[] rowType = new char[NUM_ROWS] { 'L', 'L', 'L' };
            double[] rhsValues = new double[NUM_ROWS] { 1400, 8000, 5000 };
            double[] rangeValues = new double[NUM_ROWS] { 0.0, 0.0, 0.0 };

            int[] matrixBegin = new int[NUM_COLS + 1] { 0, 2, 4 };
            int[] matrixCount = new int[NUM_COLS] { 2, 2 };
            int[] matrixIndex = new int[NUM_NZ] { 0, 1, 0, 2 };
            double[] matrixValues = new double[NUM_NZ] { 0.1, 1.0, 0.2, 1.0 };

            string[] colNames = new string[NUM_COLS] { "Sun", "Moon" };
            string[] rowNames = new string[NUM_ROWS] { "c1", "c2", "c3" };

            double optimalValue = 506.66666667;

            solveProblem.RunProblem(problemName, optimalValue, colCount, rowCount,
                nonZeroCount, rangeCount, objectSense, objectConst, objectCoeffs,
                lowerBounds, upperBounds, rowType, rhsValues, null,
                matrixBegin, matrixCount, matrixIndex, matrixValues,
                colNames, rowNames, objectName, null, null);

        }

    }

}