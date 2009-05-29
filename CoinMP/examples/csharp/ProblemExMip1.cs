using System.Windows.Forms;
using Coin.CoinMP;

// exmip1.mps in Mps/Sample

namespace CoinMPTest
{
    public static class ProblemExMip1
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 8;
            const int NUM_ROWS = 5;
            const int NUM_NZ = 14;
            const int NUM_RNG = 2;
            const double DBL_MAX = 1e37;

	        string probname = "Exmip1";
	        int ncol = NUM_COLS;
	        int nrow = NUM_ROWS;
	        int nels = NUM_NZ;
	        int nrng = NUM_RNG;
	
	        string objectname = "z";
	        int objsens = CoinMP.SOLV_OBJSENS_MIN;
	        double objconst = 0.0;
	        double[] dobj = new double[NUM_COLS] {1, 0, 0, 0, 2, 0, 0, -1};

	        double[] dclo = new double[NUM_COLS] {2.5, 0, 0, 0, 0.5, 0, 0, 0};
	        double[] dcup = new double[NUM_COLS] {DBL_MAX, 4.1, DBL_MAX, DBL_MAX, 4, DBL_MAX, DBL_MAX, 4.3};

	        //char[] rtyp = new char[NUM_ROWS]  {'G', 'L', 'E', 'R', 'R'};
	        //double[] drhs = new double[NUM_ROWS] {2.5, 2.1, 4, 1.8, 15};
	        //double[] drng = new double[NUM_ROWS] {0, 0, 0, -3.2, 12};

            double[] drlo = {2.5, -DBL_MAX, -DBL_MAX, 1.8, 3.0};
            double[] drup = {DBL_MAX, 2.1, 4.0, 5.0, 15.0};

	        int[] mbeg = new int[NUM_COLS+1] {0, 2, 4, 6, 8, 10, 11, 12, 14};
	        int[] mcnt = new int[NUM_COLS] {2, 2, 2, 2, 2, 1, 1, 2};
	        int[] midx = new int[NUM_NZ] {0, 4, 0, 1, 1, 2, 0, 3, 0, 4, 2, 3, 0, 4};
	        double[] mval = new double[NUM_NZ] {3, 5.6, 1, 2, 1.1, 1, -2, 2.8, -1, 1, 1, -1.2, -1, 1.9};

	        string[] colNames = new string[NUM_COLS] {"col01", "col02", "col03", "col04", "col05", "col06", 
		                                              "col07", "col08"};
	        string[] rowNames = new string[NUM_ROWS] {"row01", "row02", "row03", "row04", "row05"};

            char[] ctyp = new char[NUM_COLS] { 'C', 'C', 'B', 'B', 'C', 'C', 'C', 'C' };

	        double optimalValue = 3.23684210526;

            solveProblem.RunProblem(probname, optimalValue, ncol, nrow, nels, nrng, 
	            objsens, objconst, dobj, dclo, dcup, drlo, drup, mbeg,
                mcnt, midx, mval, colNames, rowNames, objectname, null, ctyp);
        }
    }
}
