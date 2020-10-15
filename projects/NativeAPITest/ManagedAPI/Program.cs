using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace Test
{
    class Program
    {

        static void Main(string[] args)
        {
            TestAssembly.Test();
            //TestX509Chain.Test();
            //TestCreateCertificate.CreateSelfSignedCertificate("foo");
        }
    }
}
