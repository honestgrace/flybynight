using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
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

            label1.Text = resourceManager.GetString("label1Text");
        }
    }
}
