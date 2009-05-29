using System.Windows.Forms;

namespace CoinMPTest
{
    public class LogHandler
    {
        TextBox _txtLog = null;

        public LogHandler()
        {
            _txtLog = null;
        }


        public LogHandler(TextBox txtLog)
        {
            _txtLog = txtLog;
        }

        public TextBox txtLog
        {
            get { return _txtLog; }
            set { _txtLog = value; }
        }


        public void WriteText(string text)
        {
            if (_txtLog != null)
            {
                _txtLog.Text += text.ToString();
            }
        }


        public void WriteLine(string text)
        {
            if (_txtLog != null)
            {
                _txtLog.Text += text.ToString() + "\r\n";
            }
        }

        public void NewLine()
        {
            if (_txtLog != null)
            {
                _txtLog.Text += "\r\n";
            }
        }

        public void EmptyText()
        {
            if (_txtLog != null)
            {
                _txtLog.Text = "";
            }
        }

        public static void WriteText(TextBox txtLog, string text)
        {
            if (txtLog != null) {
                txtLog.Text += text.ToString();
            }
        }

        public static void WriteLine(TextBox txtLog, string text)
        {
            if (txtLog != null) {
                txtLog.Text += text.ToString() + "\r\n";
            }
        }

        public static void NewLine(TextBox txtLog)
        {
            if (txtLog != null) {
                txtLog.Text += "\r\n";
            }
        }

        public static void EmptyText(TextBox txtLog)
        {
            if (txtLog != null) {
                txtLog.Text = "";
            }
        }
        
    }
}