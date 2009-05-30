using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Coin.CoinMP;

namespace CoinMPTest
{
    public partial class formCoinMP : Form
    {
 
        public formCoinMP()
        {
            int result;
            int length;
            double version;
            StringBuilder solverName = new StringBuilder(100);

            InitializeComponent();

            result = CoinMP.CoinInitSolver("");
            length = CoinMP.CoinGetSolverNameBuf(solverName, solverName.Capacity);
            version = CoinMP.CoinGetVersion();

            LogHandler.EmptyText(txtLog);
            LogHandler.WriteLine(txtLog, "Solver: " + solverName);
            LogHandler.WriteLine(txtLog, "Version: " + version);
            
        }


        private void btnRunTest_Click(object sender, EventArgs e)
        {
            SolveProblem solveProblem = new SolveProblem(txtLog, txtLog);

            ProblemBakery.Solve(solveProblem);
            ProblemCoinTest.Solve(solveProblem);
            ProblemAfiro.Solve(solveProblem);
            ProblemP0033.Solve(solveProblem);
            ProblemExMip1.Solve(solveProblem);
        }

        private void btnClear_Click(object sender, EventArgs e)
        {
            LogHandler.EmptyText(txtLog);
        }
    }
}
