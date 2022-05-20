using System;
using System.Collections;
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

            /*
             * https://docs.microsoft.com/en-us/openspecs/windows_protocols/ms-lcid/a9eac961-e77d-41a6-90a5-ce1a8b0cdb9c
             */
            ArrayList test = new ArrayList()
            {
                new CultureInfo("en-US"),           /* In English Os, .en-US.resx is compatible with .resx because .resx is language-neutral */
                new CultureInfo("de-DE"),
                new CultureInfo("ja-JP"),
                
                new CultureInfo("zh-Hans"),         /* compatible with zh-CHS, exceptional case */
                new CultureInfo("zh-Hant"),         /* compatible with zh-CHT, exceptional case */
                new CultureInfo("zh-CHS"),
                new CultureInfo("zh-CHT"),
                
                new CultureInfo("ko-KR"),           /* South Korea */
                new CultureInfo("ko-KP"),           /* North Korea */
                new CultureInfo("ko"),              /* Won't work if there is no ko */

                new CultureInfo("nb"),
                new CultureInfo("nb-NO"),           /* compatible with nb */
                new CultureInfo("no"),              /* not compatible with nb */
                
                //new CultureInfo("sr-Cyrl-CS"),    /* Not supported old name in .Net of former State Union of Serbia and Montenegro */
                new CultureInfo("sr-Cyrl-RS"),      /* Republic of Serbia */
                new CultureInfo("sr-Latn-ME")       /* Montenegro */
            };
            
            foreach (CultureInfo item in test)
            {
                try
                {
                    label1.Text += "\n" + resourceManager.GetString("label1Text", item);
                }
                catch (Exception ex)
                {
                    label1.Text += "\n" + item.DisplayName + ": " + ex.Message;
                }
            }            
        }

        private void Label1_Click(object sender, EventArgs e)
        {
            _counter++;
            LoadLocalizedResources();
        }
    }
}
