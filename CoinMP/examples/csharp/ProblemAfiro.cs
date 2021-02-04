using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemAfiro
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 32;
            const int NUM_ROWS = 27;
            const int NUM_NZ = 83;
            const int NUM_RNG = 0;
            const double DBL_MAX = 1e+037;

	        string probname = "Afiro";
	        int ncol = NUM_COLS;
	        int nrow = NUM_ROWS;
	        int nels = NUM_NZ;
	        int nrng = NUM_RNG;

	        string objectname = "Cost";
	        int objsens = CoinMP.SOLV_OBJSENS_MIN;
	        double objconst = 0.0;

	        double[] dobj = new double[NUM_COLS] {0, -0.4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.32, 0, 0, 0, -0.6,
		                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -0.48, 0, 0, 10};

	        double[] dclo = new double[NUM_COLS] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		                                          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	        double[] dcup = new double[NUM_COLS] {DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX,
		                          DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX,
		                          DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX,
		                          DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX, DBL_MAX};

	        char[] rtyp = new char[NUM_ROWS] {'E', 'E', 'L', 'L', 'E', 'E', 'L', 'L', 'L', 'L', 'E', 'E', 'L',
		                                 'L', 'E', 'E', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'};

	        double[] drhs = new double[NUM_ROWS] {0, 0, 80, 0, 0, 0, 80, 0, 0, 0, 0, 0, 500, 0, 0, 44, 500, 0,
		                                          0, 0, 0, 0, 0, 0, 0, 310, 300};

	        int[] mbeg = new int[NUM_COLS+1] {0, 4, 6, 8, 10, 14, 18, 22, 26, 28, 30, 32, 34, 36, 38, 40,
		                              44, 46, 48, 50, 52, 56, 60, 64, 68, 70, 72, 74, 76, 78, 80, 82, 83};

	        int[] mcnt = new int[NUM_COLS] {4, 2, 2, 2, 4, 4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 4, 2, 2, 2, 2, 4,
		                                    4, 4, 4, 2, 2, 2, 2, 2, 2, 2, 1};

	        int[] midx = new int[NUM_NZ] {0, 1, 2, 23, 0, 3, 0, 21, 1, 25, 4, 5, 6, 24, 4, 5, 7, 24, 4, 5,
		                       8, 24, 4, 5, 9, 24, 6, 20, 7, 20, 8, 20, 9, 20, 3, 4, 4, 22, 5, 26, 10, 11,
		                       12, 21, 10, 13, 10, 23, 10, 20, 11, 25, 14, 15, 16, 22, 14, 15, 17, 22, 14,
		                       15, 18, 22, 14, 15, 19, 22, 16, 20, 17, 20, 18, 20, 19, 20, 13, 15, 15, 24,
		                       14, 26, 15};

            double[] mval = new double[NUM_NZ] {-1, -1.06, 1, 0.301, 1, -1, 1, -1, 1, 1, -1, -1.06, 1, 0.301,
		                      -1, -1.06, 1, 0.313, -1, -0.96, 1, 0.313, -1, -0.86, 1, 0.326, -1, 2.364, -1,
		                      2.386, -1, 2.408, -1, 2.429, 1.4, 1, 1, -1, 1, 1, -1, -0.43, 1, 0.109, 1, -1,
		                      1, -1, 1, -1, 1, 1, -0.43, 1, 1, 0.109, -0.43, 1, 1, 0.108, -0.39, 1, 1,
		                      0.108, -0.37, 1, 1, 0.107, -1, 2.191, -1, 2.219, -1, 2.249, -1, 2.279, 1.4,
		                      -1, 1, -1, 1, 1, 1};

	        string[] colNames = new string[NUM_COLS] {"x01", "x02", "x03", "x04", "x06", "x07", "x08", "x09",
		                         "x10", "x11", "x12", "x13", "x14", "x15", "x16", "x22", "x23", "x24", "x25",
		                         "x26", "x28", "x29", "x30", "x31", "x32", "x33", "x34", "x35", "x36", "x37",
		                         "x38", "x39"};

	        string[] rowNames = new string[NUM_ROWS] {"r09", "r10", "x05", "x21", "r12", "r13", "x17", "x18",
		                         "x19", "x20", "r19", "r20", "x27", "x44", "r22", "r23", "x40", "x41", "x42",
		                         "x43", "x45", "x46", "x47", "x48", "x49", "x50", "x51"};

	        double optimalValue = -464.753142857;

            solveProblem.RunProblem(probname, optimalValue, ncol, nrow, nels, nrng, objsens, objconst,
                           dobj, dclo, dcup, rtyp, drhs, null, mbeg, mcnt, midx, mval,
                           colNames, rowNames, objectname, null, null);
        }
    }
}
