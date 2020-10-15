using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AspNetCoreModule.Test.WebSocketClient;
using System.Threading;

namespace WebSocketClientTool
{
    public class Assert
    {
        public static void True(bool test, string description = "")
        {
            if (test)
            {
                Console.WriteLine("Pass");
            }
            else
            {
                Console.WriteLine("Fail");
                throw new System.Exception("Test fail");
            }
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            DoWebSocketTest();
        }

        public static void DoWebSocketTest()
        {
            using (WebSocketClientHelper websocketClient = new WebSocketClientHelper())
            {
                // Dotnet only
                // var frameReturned = websocketClient.Connect(new Uri("http://localhost:5000/websocket"), true, true);

                // IIS + ANCM
<<<<<<< HEAD
                var frameReturned = websocketClient.Connect(new Uri("http://localhost/PublishOutput/websocket"), true, true);

                // IISExpress + ANCM
                // var frameReturned = websocketClient.Connect(new Uri("http://localhost/PublishOutput/websocket"), true, true);
=======
                //var frameReturned = websocketClient.Connect(new Uri("http://localhost/PublishOutput/websocket"), true, true);

                // IISExpress + ANCM
                var frameReturned = websocketClient.Connect(new Uri("http://localhost/PublishOutput/websocket"), true, true);
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf

                // IIS only
                // var frameReturned = websocketClient.Connect(new Uri("http://localhost/websocket/EchoHandler.ashx"), true, true);

                //  Test close immediately
<<<<<<< HEAD
                /* Thread.Sleep(500);
=======
                Thread.Sleep(500);
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf
                var test = websocketClient.Connection.DataReceived[websocketClient.Connection.DataReceived.Count - 1];
                if (test.FrameType == FrameType.Close)
                {
                    websocketClient.Connection.Done = true;
                    websocketClient.Send(Frames.CLOSE_FRAME);
                    return;
<<<<<<< HEAD
                }  */            
=======
                }                
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf

                //var frameReturned = websocketClient.Connect(new Uri("http://localhost/websocket/EchoHandler.ashx"), true, true);
                Assert.True(frameReturned.Content.Contains("Connection: Upgrade"));
                Assert.True(frameReturned.Content.Contains("HTTP/1.1 101 Switching Protocols"));
                Thread.Sleep(500);
                VerifySendingWebSocketData(websocketClient, "a");
                Thread.Sleep(500);
                frameReturned = websocketClient.Close();
                Assert.True(frameReturned.FrameType == FrameType.Close, "Closing Handshake");
            }
        }

        private static bool VerifySendingWebSocketData(WebSocketClientHelper websocketClient, string testData)
        {
            bool result = false;
<<<<<<< HEAD
            //
            // send complete or partial text data and ping multiple times
            //
            websocketClient.SendTextData(testData, 0x01);  // 0x01: start of sending partial data
            websocketClient.SendPing();            
            websocketClient.SendTextData(testData, 0x80);  // 0x80: end of sending partial data            
            Thread.Sleep(3000);

            // Verify test result
            for (int i = 0; i < 3; i++)
            {
                if (DoVerifyDataSentAndReceived(websocketClient) == false)
                {
                    // retrying after 1 second sleeping
                    Thread.Sleep(1000);
                }
                else
                {
                    result = true;
                    break;
                }
            }
            return result;
        }

        private static bool VerifySendingWebSocketData2(WebSocketClientHelper websocketClient, string testData)
        {
            bool result = false;
=======
>>>>>>> bac425f0a67bcefec4eaf656b046c8d55cae79cf

            //
            // send complete or partial text data and ping multiple times
            //
            websocketClient.SendTextData(testData);
            websocketClient.SendPing();
            websocketClient.SendTextData(testData);
            websocketClient.SendPing();
            websocketClient.SendPing();
            websocketClient.SendTextData(testData, 0x01);  // 0x01: start of sending partial data
            websocketClient.SendPing();
            websocketClient.SendTextData(testData, 0x80);  // 0x80: end of sending partial data
            websocketClient.SendPing();
            websocketClient.SendPing();
            websocketClient.SendTextData(testData);
            websocketClient.SendTextData(testData);
            websocketClient.SendTextData(testData);
            websocketClient.SendPing();
            Thread.Sleep(3000);

            // Verify test result
            for (int i = 0; i < 3; i++)
            {
                if (DoVerifyDataSentAndReceived(websocketClient) == false)
                {
                    // retrying after 1 second sleeping
                    Thread.Sleep(1000);
                }
                else
                {
                    result = true;
                    break;
                }
            }
            return result;
        }

        private static bool DoVerifyDataSentAndReceived(WebSocketClientHelper websocketClient)
        {
            var result = true;
            var sentString = new StringBuilder();
            var recString = new StringBuilder();
            var pingString = new StringBuilder();
            var pongString = new StringBuilder();

            foreach (Frame frame in websocketClient.Connection.DataSent.ToArray())
            {
                if (frame.FrameType == FrameType.Continuation
                    || frame.FrameType == FrameType.SegmentedText
                        || frame.FrameType == FrameType.Text
                            || frame.FrameType == FrameType.ContinuationFrameEnd)
                {
                    sentString.Append(frame.Content);
                }

                if (frame.FrameType == FrameType.Ping)
                {
                    pingString.Append(frame.Content);
                }
            }

            foreach (Frame frame in websocketClient.Connection.DataReceived.ToArray())
            {
                if (frame.FrameType == FrameType.Continuation
                    || frame.FrameType == FrameType.SegmentedText
                        || frame.FrameType == FrameType.Text
                            || frame.FrameType == FrameType.ContinuationFrameEnd)
                {
                    recString.Append(frame.Content);
                }

                if (frame.FrameType == FrameType.Pong)
                {
                    pongString.Append(frame.Content);
                }
            }

            if (sentString.Length == recString.Length && pongString.Length == pingString.Length)
            {
                if (sentString.Length != recString.Length)
                {
                    result = false;
                    TestUtility.LogInformation("Same size of data sent(" + sentString.Length + ") and received(" + recString.Length + ")");
                }

                if (sentString.ToString() != recString.ToString())
                {
                    result = false;
                    TestUtility.LogInformation("Not matched string in sent and received");
                }
                if (pongString.Length != pingString.Length)
                {
                    result = false;
                    TestUtility.LogInformation("Ping received; Ping (" + pingString.Length + ") and Pong (" + pongString.Length + ")");
                }
                websocketClient.Connection.DataSent.Clear();
                websocketClient.Connection.DataReceived.Clear();
            }
            else
            {
                TestUtility.LogInformation("Retrying...  so far data sent(" + sentString.Length + ") and received(" + recString.Length + ")");
                result = false;
            }
            return result;
        }
    }
}
