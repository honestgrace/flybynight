using System;
using System.Collections.Generic;
using System.Reflection;
using System.Text;

namespace Test
{
    class TestAssembly
    {
        static IEnumerable<AssemblyName> GetInstalledVersions(string name)
        {
            int result;

            IAssemblyName assemblyName;
            result = Utils.CreateAssemblyNameObject(out assemblyName, name, CreateAssemblyNameObjectFlags.CANOF_DEFAULT, IntPtr.Zero);
            if ((result != 0) || (assemblyName == null))
                throw new Exception("CreateAssemblyNameObject failed.");

            IAssemblyEnum enumerator;
            result = Utils.CreateAssemblyEnum(out enumerator, IntPtr.Zero, assemblyName, AssemblyCacheFlags.GAC, IntPtr.Zero);
            if ((result != 0) || (enumerator == null))
                throw new Exception("CreateAssemblyEnum failed.");

            while ((enumerator.GetNextAssembly(IntPtr.Zero, out assemblyName, 0) == 0) && (assemblyName != null))
            {
                StringBuilder displayName = new StringBuilder(1024);
                int displayNameLength = displayName.Capacity;
                assemblyName.GetDisplayName(displayName, ref displayNameLength, (int)AssemblyNameDisplayFlags.ALL);
                yield return new AssemblyName(displayName.ToString());
            }

        }

        public static void Test()
        {
            //ProcessAsUser.Launch("notepad");
            foreach (AssemblyName assemblyName in GetInstalledVersions("System.Data"))
            {
                Console.WriteLine("{0} V{1}, {2}", assemblyName.Name, assemblyName.Version.ToString(), assemblyName.ProcessorArchitecture);
            }
        }
    }
}
