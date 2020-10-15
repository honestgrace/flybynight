using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Management.Automation;
using System.Text;
using System.Threading.Tasks;

namespace TestPowershell
{
<<<<<<< HEAD
    [Cmdlet(VerbsCommon.Get, "JHKIMName")]    
=======
    class Pet
    {
        public string Name { get; set; }
        public int Age { get; set; }

        public static void OrderByEx1()
        {
            Pet[] pets = { new Pet { Name="Barley", Age=8 },
                   new Pet { Name="Boots", Age=4 },
                   new Pet { Name="Whiskers", Age=1 } };

            //IEnumerable<Pet> query = pets.OrderBy(item => item.Age);
            IEnumerable<Pet> query = pets.OrderBy(item => Guid.NewGuid());

            foreach (Pet pet in query)
            {
                Console.WriteLine("{0} - {1}", pet.Name, pet.Age);
            }
        }
    }


    [Cmdlet(VerbsCommon.Get, "RandomName")]    
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf
    public class GetRandomName : Cmdlet
    {
        public GetRandomName()
        {
            Name = "bogus";
        }
        
        [Parameter(
            Position=0, 
            Mandatory=false, 
            ValueFromPipeline = true)
        ]
        [ValidateNotNullOrEmpty]
        [ValidateSet("foo", "bar", IgnoreCase = true)]
        public string Name { get; set; }

        [Parameter(
            Position = 1,
            Mandatory = true,
            ValueFromPipeline = true)
        ]
        public string MiddleName { get; set; }

        protected override void ProcessRecord()
        {
<<<<<<< HEAD
=======
            Pet.OrderByEx1();

>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf
            WriteVerbose(Name);
            var result = new
            {
                Name = Name,
                MiddleName = MiddleName,
                LastName = _names.Where(n => n.Length == Name.Length)
                                .OrderBy(n => Guid.NewGuid())
                                .First()
            };            
            WriteObject(result);
            base.ProcessRecord();
        }

        private static readonly string[] _names = new string[]
        {
            "0", "1", "2",
            "01", "12", "23",
            "012", "123", "234",
            "0123", "1234", "2345",
            "01234", "12345", "23456",
            "012345", "123456", "234567",
            "0123456", "1234567", "2345678",
            "01234567", "12345678", "23456789",
            "012345678", "123456789", "234567890",
            "0123456789", "1234567890", "2345678901",
        };
    }
}
