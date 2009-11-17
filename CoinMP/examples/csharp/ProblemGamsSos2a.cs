using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemGamsSos2a
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 7;
            const int NUM_ROWS = 5;
            const int NUM_NZ = 15;
            const int NUM_RNG = 0;
            const double DBL_MAX = 1e37;
            const int NUM_SOS = 1;
            const int NUM_SOSNZ = 3;

	        string probname = "GamsSos2a";
	        int ncol = NUM_COLS;
	        int nrow = NUM_ROWS;
	        int nels = NUM_NZ;
	        int nrng = NUM_RNG;
	
	        string objectname = "z";
	        int objsens = CoinMP.SOLV_OBJSENS_MIN;
	        double objconst = 0.0;
	        double[] dobj = new double[NUM_COLS] {0, 0, 0, 0, 0, 1, 1};

	        double[] dclo = new double[NUM_COLS] {0.0, 0, 0, -DBL_MAX, -DBL_MAX, 0, 0};
	        double[] dcup = new double[NUM_COLS] {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, 
                                                  DBL_MAX, DBL_MAX, DBL_MAX};

	        char[] rtyp = new char[NUM_ROWS]  {'E', 'E', 'E', 'G', 'G'};
	        double[] drhs = new double[NUM_ROWS] {1, 0, 0, -1.3, 1.3};

	        int[] mbeg = new int[NUM_COLS+1] {0, 3, 6, 9, 10, 13, 14, 15};
	        int[] mcnt = new int[NUM_COLS] {3, 3, 3, 1, 3, 1, 1};
	        int[] midx = new int[NUM_NZ] {0, 1, 2, 0, 1, 2, 0, 1, 2, 1, 2, 3, 4, 3, 4};
	        double[] mval = new double[NUM_NZ] {1, 1, 1, 1, 2, 2, 1, 3, 3, -1, -1, -1, 1, 1, 1};

	        string[] colNames = new string[NUM_COLS] {"w1", "w2", "w3", "x", "fx", "fplus", "fminus"};
	        string[] rowNames = new string[NUM_ROWS] {"wsum", "xdef", "fxdef", "gapplus", "gapminus"};

	        int sosCount = 1;
	        int sosNZCount = 3;
	        int[] sosType = new int[NUM_SOS] {2};
	        int[] sosBegin = new int[NUM_SOS+1] {0, 3};
	        int[] sosIndex = new int[NUM_SOSNZ] {0, 1, 2};

	        double optimalValue = 0.0;

            solveProblem.RunProblemMip(probname, optimalValue, ncol, nrow, nels, nrng, 
	            objsens, objconst, dobj, dclo, dcup, rtyp, drhs, null, mbeg,
                mcnt, midx, mval, colNames, rowNames, objectname, null, null,
                sosCount, sosNZCount, sosType, null, sosBegin, sosIndex, null,
                0, null, 0, null, null, null);
        }
    }
}
