using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ManagedWindowsFormsApp
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            //CultureInfo newCultureInfo = new System.Globalization.CultureInfo("ko-KR");
            //CultureInfo newCultureInfo = new System.Globalization.CultureInfo("en-US");
            //Thread.CurrentThread.CurrentCulture = newCultureInfo;
            //Thread.CurrentThread.CurrentUICulture = newCultureInfo;
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
        }
    }
}
