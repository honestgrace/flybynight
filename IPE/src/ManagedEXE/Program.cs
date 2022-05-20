﻿using ManagedExe.Properties;
using System;
using System.Globalization;
using System.Resources;

namespace ManagedExe
{
    public class Class1
    {
        public Class1()
        {
            ResourceManager res_mng = new ResourceManager(
                typeof(Resources));
            Console.WriteLine("Current UI culture " + CultureInfo.CurrentUICulture.EnglishName);
            Console.WriteLine("Current culture " + CultureInfo.CurrentCulture);
            Console.WriteLine("Current CurrentThread culture " + System.Threading.Thread.CurrentThread.CurrentUICulture);
            string data = res_mng.GetString("String1", CultureInfo.CurrentUICulture);
            Console.WriteLine(data);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            new Class1();
        }
    }
}