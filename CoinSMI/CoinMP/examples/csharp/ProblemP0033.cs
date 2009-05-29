using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public static class ProblemP0033
    {
        public static void Solve(SolveProblem solveProblem)
        {
            const int NUM_COLS = 33;
            const int NUM_ROWS = 15;
            const int NUM_NZ = 98;
            const int NUM_RNG = 0;

            string probname = "P0033";
            int ncol = NUM_COLS;
            int nrow = NUM_ROWS;
            int nels = NUM_NZ;
            int nrng = NUM_RNG;

            string objectname = "Obj";
            int objsens = CoinMP.SOLV_OBJSENS_MIN;
            double objconst = 0.0;

            double[] dobj = new double[NUM_COLS] {171, 171, 171, 171, 163, 162, 163, 69, 69, 183, 183, 183, 
		                        183, 49, 183, 258, 517, 250, 500, 250, 500, 159, 318, 159, 318, 159, 318, 159, 
		                        318, 114, 228, 159, 318};

            double[] dclo = new double[NUM_COLS] {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

            double[] dcup = new double[NUM_COLS] {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
		                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

            char[] rtyp = new char[NUM_ROWS] { 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L' };

            double[] drhs = new double[NUM_ROWS] {1, 1, 1, 1, -5, 2700, -2600, -100, -900, -1656, -335, -1026, 
		                        -5, -500, -270};

            int[] mbeg = new int[NUM_COLS + 1] {0, 3, 6, 10, 14, 19, 24, 26, 31, 36, 38, 41, 45, 49, 53, 54, 
		                        55, 56, 58, 60, 61, 62, 66, 70, 73, 76, 80, 84, 87, 90, 93, 96, 97, 98};

            int[] mcnt = new int[NUM_COLS] {3, 3, 4, 4, 5, 5, 2, 5, 5, 2, 3, 4, 4, 4, 1, 1, 1, 2, 2, 1, 1, 
		                        4, 4, 3, 3, 4, 4, 3, 3, 3, 3, 1, 1};

            int[] midx = new int[NUM_NZ] {0, 8, 9, 0, 12, 13, 0, 5, 6, 9, 0, 5, 6, 7, 1, 5, 6, 10, 11, 1, 
		                        5, 6, 8, 9, 1, 14, 2, 5, 6, 10, 11, 2, 5, 6, 8, 9, 3, 4, 3, 10, 11, 3, 5, 6, 
		                        11, 3, 5, 6, 9, 5, 6, 8, 9, 3, 4, 4, 12, 13, 12, 13, 13, 13, 5, 6, 10, 11, 5, 
		                        6, 10, 11, 5, 6, 11, 5, 6, 11, 5, 6, 8, 9, 5, 6, 8, 9, 5, 6, 9, 5, 6, 9, 5, 6, 
		                        7, 5, 6, 7, 14, 14};

            double[] mval = new double[NUM_NZ] {1, -300, -300, 1, -300, -300, 1, 300, -300, -300, 1, 300, 
		                        -300, -300, 1, 285, -285, -285, -285, 1, 285, -285, -285, -285, 1, -285, 1, 
		                        265, -265, -265, -265, 1, 265, -265, -265, -265, 1, -230, 1, -230, -230, 1, 
		                        230, -230, -230, 1, 230, -230, -230, 190, -190, -190, -190, 1, -200, -400, 
		                        -200, -200, -400, -400, -200, -400, 200, -200, -200, -200, 400, -400, -400, 
		                        -400, 200, -200, -200, 400, -400, -400, 200, -200, -200, -200, 400, -400, 
		                        -400, -400, 200, -200, -200, 400, -400, -400, 200, -200, -200, 400, -400, 
		                        -400, -200, -400};

            string[] colNames = new string[NUM_COLS] {"c157", "c158", "c159", "c160", "c161", "c162", "c163", 
		                        "c164", "c165", "c166", "c167", "c168", "c169", "c170", "c171", "c172", 
		                        "c173", "c174", "c175", "c176", "c177", "c178", "c179", "c180", "c181", 
		                        "c182", "c183", "c184", "c185", "c186", "c187", "c188", "c189"};

            string[] rowNames = new string[NUM_ROWS] {"r114", "r115", "r116", "r117", "r118", "r119", "r120", 
		                        "r121", "r122", "r123", "r124", "r125", "r126", "r127", "r128"};

            char[] ctyp = new char[NUM_COLS] { 'B', 'B', 'B', 'B', 'B', 'B', 'B',
		                        'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		                        'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 'B', 
		                        'B', 'B', 'B', 'B', 'B', 'B' };

            double optimalValue = 3089.0;

            solveProblem.RunProblem(probname, optimalValue, ncol, nrow, nels, nrng,
                objsens, objconst, dobj, dclo, dcup, rtyp, drhs, null, mbeg, mcnt,
                midx, mval, colNames, rowNames, objectname, null, ctyp);

        }
    }
}
