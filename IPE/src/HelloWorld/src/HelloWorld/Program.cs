using System;
using System.Globalization;
using System.Resources;
namespace HelloWorld
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("");

            ResourceManager resourceManager = new ResourceManager(typeof(HelloWorld.Properties.Resources));
            
            string helloWorld = resourceManager.GetString("HelloWord");
            Console.WriteLine("Ex. 1: " + helloWorld);

            helloWorld = resourceManager.GetString("HelloWord", new CultureInfo("fr-FR"));
            Console.WriteLine("Ex. 2: " + helloWorld);

            helloWorld = resourceManager.GetString("HelloWord", new CultureInfo("de-DE"));
            Console.WriteLine("Ex. 3: " + helloWorld);
            
            helloWorld = resourceManager.GetString("HelloWord", new CultureInfo("el-GR"));
            Console.WriteLine("Ex. 4: " + helloWorld);

            helloWorld = resourceManager.GetString("HelloWord", new CultureInfo("de"));
            Console.WriteLine("Quiz: " + helloWorld);
            Console.WriteLine("");
        }
    }
}
