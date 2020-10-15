using System;
using System.Text;
using System.Threading;
using System.Reflection;
using AspnetCoreModule.Tests.Utility;
using AspnetCoreModule.Tests.Utility.WebSocketClient;

namespace Test
{
    public class WebsocketTest 
    {
        private ConnectionManager _cm;

        public void Setup()
        {
            TestUtility.LogMessage("Setup() called");
            _cm = WebSocketClientUtility.GetEchoConnectionManager();
            _cm.InitiateWithAlwaysReading();
            LogAndVerifyOpeningHandshake(_cm); 
        }

        public void Restart()
        {
            LogAndVerifyClosingHandshake(_cm);
            Setup();
            Thread.Sleep(3000);
        }
        public void Message(string message = "partial     ")
        {
            _cm.SendTextData(message + " start  ", 0x01);
            _cm.SendTextData(message + " end    ", 0x80);
            Thread.Sleep(3000);
        }

        public void Message2(string message="MyMessage    ")
        {
            _cm.SendTextData(message);
            _cm.SendTextData(message);
            Thread.Sleep(3000);
        }
        
        public void Ping(int count = 1)
        {
            for (int i = 0; i < count; i++)
            {
                _cm.SendPing();
            }
            Thread.Sleep(3000);
        }

        public void Repro(int count = 1)
        {
            int totalByteSent = 0;
            int dataLength = "pragment1pragment2".Length;
            for (int i = 0; i < count; i++)
            {
                _cm.SendTextData("pragment1", 0x01);
                _cm.SendPing();
                _cm.SendTextData("pragment2", 0x80);
                _cm.SendPing();
                totalByteSent += dataLength;
                Thread.Sleep(100);
            }
            Thread.Sleep(5000);
            Console.WriteLine("totalByteSent: " + totalByteSent);
        }

        public void ReproNoPing(int count = 1)
        {
            int totalByteSent = 0;
            int dataLength = "pragment1pragment2".Length;
            for (int i = 0; i < count; i++)
            {
                _cm.SendTextData("pragment1", 0x01);
                 //_cm.SendPing();
                _cm.SendTextData("pragment2", 0x80);
                totalByteSent += dataLength;
            }

            Console.WriteLine("totalByteSent: " + totalByteSent);
        }

        public void Repro3(int count = 1, string message = "MyMessage    ")
        {
            for (int j = 0; j < count; j++)
            {
                _cm.SendTextData(message);
                for (int i = 0; i < count; i++)
                {
                    _cm.SendPing();
                }
                _cm.SendTextData(message);
                Thread.Sleep(1000);
            }
            Thread.Sleep(3000);
        }
        
        public void Test()
        {
            TestUtility.LogMessage("Test() called");
            //int sleepTime = 3000;

            //_cm.SendTextData(TestUtility.RandomString(10485760));
            //sleepTime = 10000;
            //int times = 1;

            //Repro();

            //Console.WriteLine("Start Sleeping... " + sleepTime + " ms");
            //_cm.SendTextData(TestUtility.RandomString(1024 * times));
            //Thread.Sleep(sleepTime);
            //sleepTime = 3000;
            //Console.WriteLine("End Sleeping... " + sleepTime + " ms");

            Thread.Sleep(3000);

            while (true)
            {
                Thread.Sleep(3000);
            }

            TestUtility.LogMessage("Verifying");
            VerifyDataSentAndReceived(_cm);

            TestUtility.LogMessage("Closing WebSocket Connection");
            LogAndVerifyClosingHandshake(_cm);

            Thread.Sleep(100);
        }

        private void LogAndVerifyOpeningHandshake(ConnectionManager cm)
        {
            cm.SendWebSocketRequest(WebSocketClientUtility.WebSocketVersion);
            Thread.Sleep(5000);
            
            Frame openingFrame;

            if (!cm.IsAlwaysReading)
                openingFrame = cm.ReadData();
            else
                openingFrame = cm.Client.DataReceived[0];

            TestUtility.LogMessage(openingFrame.Content);
            TestUtility.VerifyTrue(
                ((openingFrame.Content.IndexOf("Connection: Upgrade", System.StringComparison.OrdinalIgnoreCase) >= 0)
                    && (openingFrame.Content.IndexOf("Upgrade: Websocket", System.StringComparison.OrdinalIgnoreCase) >= 0)
                        && openingFrame.Content.Contains("HTTP/1.1 101 Switching Protocols")
                    ),
                "Opening handshake");
            Thread.Sleep(1000);
        }

        private void LogAndVerifyClosingHandshake(ConnectionManager cm)
        {
            cm.CloseConnection();
            Thread.Sleep(500);

            Frame closeFrame;

            if (!cm.IsAlwaysReading)
                closeFrame = cm.ReadData();
            else
                closeFrame = cm.Client.DataReceived[cm.Client.DataReceived.Count - 1];


            TestUtility.VerifyTrue(closeFrame.FrameType == FrameType.Close, "Closing Handshake");
        }

        private void VerifyDataSentAndReceived(ConnectionManager cm)
        {
            var sentString = new StringBuilder();
            var recString = new StringBuilder();

            foreach (Frame frame in cm.Client.DataSent.ToArray())
            {
                if (frame.FrameType == FrameType.Continuation
                    || frame.FrameType == FrameType.SegmentedText
                        || frame.FrameType == FrameType.Text
                            || frame.FrameType == FrameType.ContinuationFrameEnd)
                    sentString.Append(frame.Content);
            }

            foreach (Frame frame in cm.Client.DataReceived.ToArray())
            {
                if (frame.FrameType == FrameType.Continuation
                    || frame.FrameType == FrameType.SegmentedText
                        || frame.FrameType == FrameType.Text
                            || frame.FrameType == FrameType.ContinuationFrameEnd)
                    recString.Append(frame.Content);
            }

            TestUtility.VerifyTrue(sentString.Length == recString.Length, "Same size of data sent(" + sentString.Length + ") and received(" + recString.Length  + ")");
            TestUtility.VerifyTrue(sentString.ToString() == recString.ToString(), "Same string in sent and received");
            cm.Client.DataSent.Clear();
            cm.Client.DataReceived.Clear();
            //TestUtility.LogMessage(sentString.ToString());
            //TestUtility.LogMessage(recString.ToString());
        }
    }

    class TestcaseMain
    {
        [STAThread()]
        public static void Main(string[] args)
        {
            try
            {
                if (args.Length == 1)
                {
                    WebSocketClientUtility.WebSocketUri = args[0];
                    WebsocketTest test = new WebsocketTest();
                    test.Setup();
                    test.Test();
                }
                else
                {
                    Console.WriteLine("Usage: Ex. TestWebSocket.exe http://localhost:8080");
                }
            }
            catch (Exception err)
            {
                Console.WriteLine(err.Message);
            }
        }
    }
}