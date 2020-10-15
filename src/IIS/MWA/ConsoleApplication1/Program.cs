using System;
using System.ServiceProcess;
using Microsoft.Web.Administration;

namespace ConsoleApplication1
{
    class Program
    {
        public static void Main()
        {
            string computerName = "jhkim-dev";
            ServiceController[] services = ServiceController.GetServices(computerName);
            Console.WriteLine(services.Length);

            using (ServerManager serverManager = new ServerManager($@"\\{computerName}\c$\windows\system32\inetsrv\config\applicationHost.config"))
            using (ServerManager serverManager2 = ServerManager.OpenRemote("jhkim-dev"))
            {
                ApplicationPoolCollection appPool = serverManager.ApplicationPools;
                Console.WriteLine(appPool.Count);

                ApplicationPoolCollection appPool2 = serverManager2.ApplicationPools;
                Console.WriteLine(appPool2.Count);
            }

            Console.ReadKey();

        }
    }
}