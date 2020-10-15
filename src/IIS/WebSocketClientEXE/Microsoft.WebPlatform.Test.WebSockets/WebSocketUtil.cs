using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Microsoft.WebPlatform.Test.WebSockets
{
    public static class WebSocketUtil
    {
        public static bool DisplayLog = true;

        public static string RandomString(long size)
        {
            var random = new Random((int)DateTime.Now.Ticks);

            var builder = new StringBuilder();
            char ch;
            for (int i = 0; i < size; i++)
            {
                ch = Convert.ToChar(Convert.ToInt32(Math.Floor(26 * random.NextDouble() + 65)));
                builder.Append(ch);
            }

            return builder.ToString();
        }

        public static void LogVerbose(string format, params object[] parameters)
        {
            if (DisplayLog)
            Console.WriteLine(format, parameters);
        }

        public static FrameType GetFrameType(byte[] inputData)
        {
            if(inputData.Length==0)
                return FrameType.NonControlFrame;

            byte firstByte = inputData[0];

            switch (firstByte)
            {
                case 0x80:
                    return FrameType.ContinuationFrameEnd;
                case 0:
                    return FrameType.Continuation;
                case 0x81:
                    return FrameType.Text;
                case 0x01:
                    return FrameType.SegmentedText;
                case 0x82:
                    return FrameType.Binary;
                case 0x02:
                    return FrameType.SegmentedBinary;
                case 0x88:
                    return FrameType.Close;
                case 0x89:
                    return FrameType.Ping;
                case 0x8A:
                    return FrameType.Pong;
            }
            return FrameType.NonControlFrame;
        }

        public static string GetFrameString(byte[] inputData)
        {
            string content;

            FrameType frameType = GetFrameType(inputData);

            if (frameType != FrameType.NonControlFrame && frameType != FrameType.ContinuationControlled)
            {
                int frameLength = inputData[1]; 
                int startingIndex = 2;
                int dataLength = 0;

                if (IsFrameMasked(inputData))
                {
                    frameLength = inputData[1]^128; 
                    
                    if (frameLength < WebSocketConstants.SMALL_LENGTH_FLAG)
                    {
                        startingIndex = 6;
                        dataLength = inputData[1] ^ 128; 
                    }
                    else if (frameLength == WebSocketConstants.SMALL_LENGTH_FLAG)
                    {
                        startingIndex = 8;
                        dataLength = (int)GetFrameSize(inputData, 2, 4); 
                    }
                    else if (frameLength == WebSocketConstants.LARGE_LENGTH_FLAG)
                    {
                        startingIndex = 14;
                        dataLength = (int)GetFrameSize(inputData, 2, 10); 
                    }
                }
                else
                {
                    if (frameLength < WebSocketConstants.SMALL_LENGTH_FLAG)
                    {
                        startingIndex = 2;
                        dataLength = inputData[1];
                    }
                    else if (frameLength == WebSocketConstants.SMALL_LENGTH_FLAG)
                    {
                        startingIndex = 4;
                        dataLength = (int)GetFrameSize(inputData, 2, 4);
                    }
                    else if (frameLength == WebSocketConstants.LARGE_LENGTH_FLAG)
                    {
                        startingIndex = 10;
                        dataLength = (int)GetFrameSize(inputData, 2, 10);
                    }
                }

                content = Encoding.UTF8.GetString(inputData, startingIndex, (inputData.Length - startingIndex < dataLength) ? inputData.Length - startingIndex : dataLength);
            }
            else
                content = Encoding.UTF8.GetString(inputData, 0, inputData.Length);

            return content;
        }

        public static uint GetFrameSize(byte[] inputData, int start, int length)
        {
            byte[] bytes = SubArray(inputData, 2, length - 2);

            if (BitConverter.IsLittleEndian)
                Array.Reverse(bytes);

            if (length > 4)
                return BitConverter.ToUInt32(bytes, 0);
            else
                return BitConverter.ToUInt16(bytes, 0);
        }

        public static byte[] GetFramedTextDataInBytes(string data)
        {
            return GetFramedDataInBytes(0x81, data);
        }
        public static byte[] GetFramedTextDataInBytes(string data, byte opCode)
        {
            return GetFramedDataInBytes(opCode, data);
        }

        public static byte[] GetFramedBinaryDataInBytes(string data)
        {
            return GetFramedDataInBytes(0x82, data);
        }

        private static byte[] GetFramedDataInBytes(byte dataType, string data)
        {
            var a = BitConverter.GetBytes(data.Length);
            var framelist = GetByteArrayFromNumber(dataType, data.Length);


            byte[] datalist = Encoding.UTF8.GetBytes(data);

            var frame = JoinTwoArrays(framelist, datalist);
            return frame;
        }


        public static byte[] GetByteArrayFromNumber(byte dataType, int number)
        {
            if (number < 126)
            {
                return new byte[] {dataType, (byte)(number | 128),0,0,0,0 };
            }
            else
            {
                byte lengthByte = WebSocketConstants.LARGE_LENGTH_BYTE;
                int lengthBits = 16;

                if (number < 65536)
                {
                    lengthByte = WebSocketConstants.SMALL_LENGTH_BYTE;
                    lengthBits = 4;
                }

                var framelist = new byte[] { dataType, lengthByte };
                string hexValue = (number).ToString("X");
                hexValue = PrependZeroes(hexValue, lengthBits - hexValue.Length);

                var sizeArray = JoinTwoArrays(StringToByteArray(hexValue), new byte[]{0,0,0,0});

                return JoinTwoArrays(framelist, sizeArray);
            }
        }

        public static string PrependZeroes(string hex, int zeroes)
        {
            for (int i = 0; i < zeroes; i++)
            {
                hex = "0" + hex;
            }
            return hex;
        }

        public static byte[] StringToByteArray(string hex)
        {
            return Enumerable.Range(0, hex.Length)
                             .Where(x => x % 2 == 0)
                             .Select(x => Convert.ToByte(hex.Substring(x, 2), 16))
                             .ToArray();
        }

        public static bool IsFrameMasked(byte[] inputData)
        {
            bool frameMasked = false;
            FrameType frameType = GetFrameType(inputData);

            if (frameType != FrameType.NonControlFrame && inputData[1] > 127)
                frameMasked = true;

            return frameMasked;
        }

        public static byte[] JoinTwoArrays(byte[] aArray, byte[] bArray)
        {
            var concat = new byte[aArray.Length + bArray.Length];

            Buffer.BlockCopy(aArray, 0, concat, 0, aArray.Length);
            Buffer.BlockCopy(bArray, 0, concat, aArray.Length, bArray.Length);

            return concat;

        }

        public static T[] SubArray<T>(this T[] data, int index, int length)
        {
            T[] result = new T[length];
            Array.Copy(data, index, result, 0, length);
            return result;
        }

        public static void LogMessage(string message)
        {
            Console.WriteLine(message);
        }

        public static void VerifyTrue(bool condition, string message)
        {
            if (condition)
            {
                Console.WriteLine("PASS: " + message);
            }
            else
            {
                Console.WriteLine("FAIL: " + message);
            }
        }

        public static string WebSocketUri = "http://localhost:8080";
        public static int WebSocketVersion { get { return 13; } }

        public static ConnectionManager GetEchoConnectionManager()
        {
            return new ConnectionManager(GetEchoUri(), true);
        }

        public static Uri GetEchoUri()
        {
            return new Uri(WebSocketUri);
        }
    }
}