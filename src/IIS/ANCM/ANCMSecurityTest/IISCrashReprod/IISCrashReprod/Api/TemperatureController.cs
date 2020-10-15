using System.Text;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Extensions.Logging;

namespace IISCrashReprod.Api
{
    [Route("api/[controller]")]
    public class TemperatureController : Controller
    {
        private readonly ILogger<TemperatureController> _logger;

        public TemperatureController(ILogger<TemperatureController> logger)
        {
            _logger = logger;
        }

        // POST api/Temperature
        [HttpPost]
        public IActionResult Post([FromBody]TemperatureRecord[] temps)
        {
            if (!ModelState.IsValid)
            {
                _logger.LogWarning("Received POST with invalid ModelState");
                return BadRequest();
            }

            return NoContent();
        }
    }

    public class TemperatureRecord
    {
        public string SN { get; set; }

        public double Temperature { get; set; }
    }
}
