using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Microsoft.WebPlatform.Test.WebSockets
{
    public class Frame
    {
        public Frame(byte[] data)
        {
            Data = data;
            FrameType = WebSocketUtil.GetFrameType(Data);
            Content = WebSocketUtil.GetFrameString(Data);
            IsMasked = WebSocketUtil.IsFrameMasked(Data);
        }

        public FrameType FrameType { get; set; }
        public byte[] Data { get; private set; }
        public string Content { get; private set; }
        public bool IsMasked { get; private set; }

        override public string ToString()
        {
            return FrameType + ": " + Content;
        }
    }
}
