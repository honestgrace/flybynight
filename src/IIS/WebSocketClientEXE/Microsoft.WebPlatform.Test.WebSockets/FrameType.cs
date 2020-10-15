using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Microsoft.WebPlatform.Test.WebSockets
{
    //*  %x0 denotes a continuation frame
    //*  %x1 denotes a text frame
    //*  %x2 denotes a binary frame
    //*  %x3-7 are reserved for further non-control frames
    //*  %x8 denotes a connection close
    //*  %x9 denotes a ping
    //*  %xA denotes a pong
    public enum FrameType
    {
        NonControlFrame,
        Ping,
        Pong,
        Text,
        SegmentedText,
        Binary,
        SegmentedBinary,
        Continuation,
        ContinuationControlled,
        ContinuationFrameEnd,
        Close,
    }
}
