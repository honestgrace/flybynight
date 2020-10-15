/*

 NOTE:
 1. C:\SharedFolder is hard-coded. The value should be updated if it is required and the directory should be created giving the full access permission to users and everyone.
 2. This test module should be deployed to bin directory and web.config on the root application directory should be updated as the following:

 <configuration>
    <system.webServer>
      <modules>
        <add name="TestManagedModule" type="MyHttpModuleProject.TestManagedModule" />
      </modules>
    </system.webServer>
</configuration>

 */

using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Web;

namespace MyHttpModuleProject
{
    public class TestManagedModule : IHttpModule
    {
        static int staticId;
        
        static TestManagedModule()
        {
            staticId = new Random().Next();
        }

        private int processId
        {
            get
            {
                return Process.GetCurrentProcess().Id;
            }
        }

        private int InstanceId;
        public TestManagedModule()
        {
            this.InstanceId = new Random().Next();
            //Comment("TestManagedModule");
        }

        public void Comment(string text)
        {
            File.AppendAllText(@"C:\SharedFolder\MyHttpModule.log",
                "p:" + processId.ToString().PadRight(8) +
                "u:" + Environment.UserName.PadRight(16) +
                "d:" + Assembly.GetExecutingAssembly().GetName().Name.PadRight(32) +
                "s:" + staticId.ToString().PadRight(16) +
                "i:" + InstanceId.ToString().PadRight(16) +
                text + Environment.NewLine);
        }

        void IHttpModule.Init(HttpApplication app)
        {
            app.BeginRequest += app_BeginRequest;
            app.EndRequest += app_EndRequest;
            //Comment("Init");
        }

        public void Dispose()
        {
            //throw new NotImplementedException();
            //Comment("Dispose");
        }

        private void app_BeginRequest(object sender, EventArgs e)
        {
            var app = (HttpApplication)sender;
            var headerValue = app.Request.Headers.AllKeys.Contains("X-Original-URL") ? app.Request.Headers["X-Original-URL"] : "-----------";

            Comment("BeginRequest".PadRight(16) + app.Request.Url.AbsoluteUri.PadRight(32) + app.Request.RawUrl.PadRight(32) + headerValue);
        }

        void app_EndRequest(object sender, EventArgs e)
        {
            var app = (HttpApplication)sender;
            var headerValue = app.Request.Headers.AllKeys.Contains("X-Original-URL") ? app.Request.Headers["X-Original-URL"] : "-----------";
            Comment("EndRequest".PadRight(16) + app.Request.Url.AbsoluteUri.PadRight(32) + app.Request.RawUrl.PadRight(32) + headerValue);
        }
    }
}
