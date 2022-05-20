using System;
using System.Globalization;
using System.Resources;
using System.Threading;

[assembly: NeutralResourcesLanguageAttribute("en-US")]

public class Example
{
   public static void Main()
   {
      string[] cultureNames = { "en-US", "ko-KR", "fr-FR", "ru-RU", "es-ES" };
      string ResourceClassName = "MyResource";
      ResourceManager rm = new ResourceManager(ResourceClassName,
                               typeof(Example).Assembly);

      for (int ctr = 0; ctr < cultureNames.Length; ctr++) {
         string cultureName = cultureNames[ctr];
         Console.WriteLine("Testing culture: " + cultureName);

         CultureInfo culture = CultureInfo.CreateSpecificCulture(cultureName);
         Thread.CurrentThread.CurrentCulture = culture;
         Thread.CurrentThread.CurrentUICulture = culture;

         Console.WriteLine("Current culture: {0}", culture.NativeName);
         string timeString = rm.GetString("TimeHeader");
         Console.WriteLine("{0} {1:T}\n", timeString, DateTime.Now);
      }
   }
}