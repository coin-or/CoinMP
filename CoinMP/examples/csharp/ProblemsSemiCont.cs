using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemSemiCont
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 4;
            const int NUM_ROWS = 3;
            const int NUM_NZ = 6;
            const int NUM_RNG = 0;
            const double DBL_MAX = 1e37;
            const int NUM_SEMI = 1;

	        string probname = "SemiCont";
	        int ncol = NUM_COLS;
	        int nrow = NUM_ROWS;
	        int nels = NUM_NZ;
	        int nrng = NUM_RNG;
	
	        string objectname = "z";
	        int objsens = CoinMP.SOLV_OBJSENS_MIN;
	        double objconst = 0.0;
	        double[] dobj = new double[NUM_COLS] {0.0, 1.0, 1.0, 0.0};

	        double[] dclo = new double[NUM_COLS] {2.8, 0.0, 0.0, 0.0};
	        double[] dcup = new double[NUM_COLS] {10.0, DBL_MAX, DBL_MAX, DBL_MAX};

	        char[] rtyp = new char[NUM_ROWS]  {'L', 'G', 'E'};
	        double[] drhs = new double[NUM_ROWS] {8.9, 8.9, 10.0};

	        int[] mbeg = new int[NUM_COLS+1] {0, 1, 2, 3, 6};
	        int[] mcnt = new int[NUM_COLS] {1, 1, 1, 3};
	        int[] midx = new int[NUM_NZ] {2, 0, 1, 0, 1, 2};
	        double[] mval = new double[NUM_NZ] {1, -1, 1, 1, 1, 1};

	        string[] colNames = new string[NUM_COLS] {"s", "pup", "plo", "x"};
	        string[] rowNames = new string[NUM_ROWS] {"bigx", "smallx", "f"};

			int semiCount = 1;
			int[] semiIndex = new int[NUM_SEMI] {0};

	        double optimalValue = 1.1;

            solveProblem.RunProblemMip(probname, optimalValue, ncol, nrow, nels, nrng, 
	            objsens, objconst, dobj, dclo, dcup, rtyp, drhs, null, mbeg,
                mcnt, midx, mval, colNames, rowNames, objectname, null, null,
                0, 0, null, null, null, null, null, semiCount, semiIndex, 
                0, null, null, null);
        }
    }
}
