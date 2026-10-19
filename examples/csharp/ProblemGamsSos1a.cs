using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemGamsSos1a
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 3;
            const int NUM_ROWS = 1;
            const int NUM_NZ = 3;
            const int NUM_RNG = 0;
            const double DBL_MAX = 1e37;
            const int NUM_SOS = 1;
            const int NUM_SOSNZ = 3;

	        string probname = "GamsSos1a";
	        int ncol = NUM_COLS;
	        int nrow = NUM_ROWS;
	        int nels = NUM_NZ;
	        int nrng = NUM_RNG;

	        string objectname = "z";
	        int objsens = CoinMP.SOLV_OBJSENS_MAX;
	        double objconst = 0.0;
	        double[] dobj = new double[NUM_COLS] {0.9, 1.0, 1.1};

	        double[] dclo = new double[NUM_COLS] {0, 0, 0};
	        double[] dcup = new double[NUM_COLS] {0.8, 0.6, 0.6};

	        //char[] rtyp = new char[NUM_ROWS]  {};
	        //double[] drhs = new double[NUM_ROWS] {};
	        //double[] drng = new double[NUM_ROWS] {};

            double[] drlo = {-DBL_MAX};
            double[] drup = {1.0};

	        int[] mbeg = new int[NUM_COLS+1] {0, 1, 2, 3};
	        int[] mcnt = new int[NUM_COLS] {1, 1, 1};
	        int[] midx = new int[NUM_NZ] {0, 0, 0};
	        double[] mval = new double[NUM_NZ] {1, 1, 1};

	        string[] colNames = new string[NUM_COLS] {"x1", "x2", "x3"};
	        string[] rowNames = new string[NUM_ROWS] {"xsum"};

	        int sosCount = 1;
	        int sosNZCount = 3;
	        int[] sosType = new int[NUM_SOS] {1};
	        int[] sosBegin = new int[NUM_SOS+1] {0, 3};
	        int[] sosIndex = new int[NUM_SOSNZ] {0, 1, 2};

	        double optimalValue = 0.72;

            solveProblem.RunProblemMip(probname, optimalValue, ncol, nrow, nels, nrng,
	            objsens, objconst, dobj, dclo, dcup, null, drlo, drup, mbeg,
                mcnt, midx, mval, colNames, rowNames, objectname, null, null,
                sosCount, sosNZCount, sosType, null, sosBegin, sosIndex, null,
                0, null, 0, null, null, null);
        }
    }
}
