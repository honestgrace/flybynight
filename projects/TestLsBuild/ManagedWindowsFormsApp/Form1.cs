using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Resources;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ManagedWindowsFormsApp
{
    public partial class Form1 : Form
    {
        static int _counter = 0;

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {

            LoadLocalizedResources();
        }

        private void LoadLocalizedResources()
        {
            //ResourceManager resourceManager = new ResourceManager(typeof(ManagedWindowsFormsApp.Resources.LocalizedResources));
            //Equivalent statement for above
            ResourceManager resourceManager = new ResourceManager("ManagedWindowsFormsApp.Resources.LocalizedResources", Assembly.GetExecutingAssembly());

            CultureInfo ko_culture = new CultureInfo("ko-KR");
            CultureInfo de_culture = new CultureInfo("de-DE");

            int maxNumber = 3;
            switch(_counter % maxNumber)
            {
                case 0:
                    label1.Text = resourceManager.GetString("label1Text", ko_culture);
                    break;
                case 1:
                    label1.Text = resourceManager.GetString("label1Text", de_culture);
                    break;
                default:
                    //label1.Text = resourceManager.GetString("label1Text");
                    label1.Text = ManagedWindowsFormsApp.Resources.LocalizedResources.label1Text;
                    break;
            }
        }

        private void Label1_Click(object sender, EventArgs e)
        {
            _counter++;
            LoadLocalizedResources();
        }
    }
}
