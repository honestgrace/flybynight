using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Collections;

namespace Microsoft.WebPlatform.Test.WebSockets
{
    public class ConnectionManager
    {
        public Client Client { get; set; }
        public bool StoreData { get; set; }
        public bool IsAlwaysReading { get; private set; }
        public Uri Address { get; set; }
        public byte[][] HandShakeRequest { get; set; }
        public WebSocketState WebSocketState { get; set; }

        public ConnectionManager(Uri address)
        {
            StoreData = true;
            Address = address;
        }

        public ConnectionManager(Uri address, bool storeData)
        {
            Address = address;
            StoreData = storeData;
        }

        public void Initiate()
        {
            string host = Address.DnsSafeHost;
            int port = Address.Port;
            
            Client = new Client();

            WebSocketUtil.LogVerbose("Connecting to {0} on {1}", host, port);

            Client.TcpClient = new MyTcpClient(host, port);
            Client.Stream = Client.TcpClient.GetStream();
            IsAlwaysReading = false;

            if (StoreData)
            {
                Client.DataSent = new List<Frame>();
                Client.DataReceived = new List<Frame>();
            }
        }

        public void InitiateWithAlwaysReading()
        {
            Initiate();
            Client.Stream.BeginRead(Client.InputData, 0, Client.InputData.Length, ReadDataCallback, Client);
            IsAlwaysReading = true;
        }

        public void SendWebSocketRequest(int websocketVersion)
        {
            HandShakeRequest = Frames.GetHandShakeFrame(Address.AbsoluteUri, websocketVersion);
           
            byte[] outputData = null;
            int offset = 0;
            while (offset < HandShakeRequest.Length)
            {
                outputData = HandShakeRequest[offset++];

                var result = Client.Stream.BeginWrite(outputData, 0, outputData.Length, WriteCallback, Client);
                
                //jhkim debug
                //result.AsyncWaitHandle.WaitOne();

                WebSocketUtil.LogVerbose("Client {0:D3}: Write {1} bytes: {2} ", Client.Id, outputData.Length,
                    Encoding.UTF8.GetString(outputData, 0, outputData.Length));

                //result.AsyncWaitHandle.Close();
            }
        }

        public void SendWebSocketRequest(int websocketVersion, string AffinityCookie)
        {
            HandShakeRequest = Frames.GetHandShakeFrameWithAffinityCookie(Address.AbsoluteUri, websocketVersion, AffinityCookie);

            byte[] outputData = null;
            int offset = 0;
            while (offset < HandShakeRequest.Length)
            {
                outputData = HandShakeRequest[offset++];

                Client.Stream.BeginWrite(outputData, 0, outputData.Length, WriteCallback, Client);
                WebSocketUtil.LogVerbose("Client {0:D3}: Write {1} bytes: {2} ", Client.Id, outputData.Length,
                    Encoding.UTF8.GetString(outputData, 0, outputData.Length));
            }
        }
        
        public void ReadDataCallback(IAsyncResult result)
        {
            Client client = (Client) result.AsyncState;
            
            if (client.IsDisposed)
                return;

            int bytesRead = client.Stream.EndRead(result); // wait until the buffer is filled 
            int bytesReadIntotal = bytesRead;
            ArrayList InputDataArray = new ArrayList();
            byte[] tempBuffer = null;

            if (bytesRead > 0)
            {
                tempBuffer = WebSocketUtil.SubArray(Client.InputData, 0, bytesRead);

                // start looping if there is still remaining data
                if (client.TcpClient.GetStream().DataAvailable)
                {
                    // add the first buffer to the arrayList
                    InputDataArray.Add(tempBuffer);

                    // start looping appending to the arrayList
                    while (client.TcpClient.GetStream().DataAvailable)
                    {
                        bytesRead = client.TcpClient.GetStream().Read(Client.InputData, 0, Client.InputData.Length);
                        tempBuffer = WebSocketUtil.SubArray(Client.InputData, 0, bytesRead);
                        InputDataArray.Add(tempBuffer);
                        bytesReadIntotal += bytesRead;
                        WebSocketUtil.LogVerbose("Looping: Client {0:D3}: bytesReadHere {1} ", Client.Id, bytesRead);
                    }

                    // create a single byte array with the arrayList
                    tempBuffer = new byte[bytesReadIntotal];
                    int arrayIndex = 0;
                    foreach (byte[] item in InputDataArray.ToArray())
                    {
                        for (int i = 0; i < item.Length; i++)
                        {
                            tempBuffer[arrayIndex] = item[i];
                            arrayIndex++;
                        }
                    }
                }
                
                // Create frame with the tempBuffer
                Frame frame = new Frame(tempBuffer);
                WebSocketUtil.LogVerbose("Client {0:D3}: Read Type {1} : {2} ", Client.Id, frame.FrameType, bytesReadIntotal);
                ProcessReceivedData(frame);

                // Send Pong if the frame was Ping
                if (frame.FrameType == FrameType.Ping)
                    SendPong(frame);

                if (client.IsDisposed)
                    return;

                // Start the Async Read to handle the next frame comming from server
                client.Stream.BeginRead(client.InputData, 0, client.InputData.Length, ReadDataCallback, client);
            }
            else
            {
                client.Dispose();
            }
        }

        public Frame ReadData()
        {
            Frame frame = new Frame(new byte[] { });
            
            IAsyncResult result = Client.Stream.BeginRead(Client.InputData, 0, Client.InputData.Length, null, Client);
            
            if (result != null)
            {
                int bytesRead = Client.Stream.EndRead(result);
                if (bytesRead > 0)
                {
                    frame = new Frame(WebSocketUtil.SubArray(Client.InputData, 0, bytesRead));

                    ProcessReceivedData(frame);

                    WebSocketUtil.LogVerbose("Client {0:D3}: Read Type {1} : {2} ", Client.Id, frame.FrameType, frame.Content.Length);
                }

            }

            return frame;
        }

        public void SendTextData(string data)
        {
            Send(WebSocketUtil.GetFramedTextDataInBytes(data));
        }

        public void SendTextData(string data, byte opCode)
        {
            Send(WebSocketUtil.GetFramedTextDataInBytes(data, opCode));
        }

        public void SendHello()
        {
            Send(Frames.HELLO);
        }

        public void SendPing()
        {
            Send(Frames.PING);
        }

        public void SendPong()
        {
            Send(Frames.PONG);
        }

        public void SendPong(Frame receivedPing)
        {
            var pong = new byte[receivedPing.Data.Length+4];
            for (int i = 1; i < receivedPing.Data.Length; i++)
            {   
                if(i<2)
                    pong[i] = receivedPing.Data[i];
                else
                    pong[i+4] = receivedPing.Data[i];
            }

            pong[0] = 0x8A;
            pong[1] = (byte)((int)pong[1] | 128);

            Send(pong);
        }

        public void CloseConnection()
        {
            Client.Done = true;
            Send(Frames.CLOSE_FRAME);
        }

        public static void WriteCallback(IAsyncResult result)
        {
            var client = result.AsyncState as Client;
            if (client.IsDisposed)
                return;

            client.Stream.EndWrite(result);
        }

        override public string ToString()
        {
            return Client.Id + ": " + WebSocketState.ToString();
        }

        #region Private Methods

        public Frame Send(byte[] outputData)
        {
            var frame = new Frame(outputData);
            ProcessSentData(frame);
            if (Client.TcpClient.Connected)
            {
                var result = Client.Stream.BeginWrite(outputData, 0, outputData.Length, WriteCallback, Client);
                WebSocketUtil.LogVerbose("Client {0:D3}: Write Type {1} : {2} ", Client.Id, frame.FrameType,
                    frame.Content.Length);
            }
            else
            {
                Console.WriteLine("Connection is disconnected");
            }

            return frame;
        }

        private void ProcessSentData(Frame frame)
        {
            ProcessData(frame, true);
        }

        private void ProcessReceivedData(Frame frame)
        {
            if (frame.Content.Contains("Connection: Upgrade")
                && frame.Content.Contains("Upgrade: Websocket")
                && frame.Content.Contains("HTTP/1.1 101 Switching Protocols"))
                WebSocketState = WebSocketState.ConnectionOpen;
            if (frame.FrameType == FrameType.Close)
                WebSocketState = WebSocketState.ConnectionClosed;
            ProcessData(frame, false);
        }

        private void ProcessData(Frame frame, bool isSentData)
        {
            if (isSentData && StoreData)
                StoreDataSent(frame);
            else if (StoreData)
                StoreDataReceived(frame);
        }

        private void StoreDataReceived(Frame frame)
        {
            Client.DataReceived.Add(frame);
            Client.TotalDataReceived += frame.Content.Length;
         }

        private void StoreDataSent(Frame frame)
        {
            Client.DataSent.Add(frame);
            Client.TotalDataSent += frame.Content.Length;
        }

        #endregion
    }
}
