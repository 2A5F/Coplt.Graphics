using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Native;

public unsafe partial struct FMessage
{
    public string GetString() => msg.str == null
        ? ""
        : type switch
        {
            FMessageType.CStr8 => Encoding.UTF8.GetString(MemoryMarshal.CreateReadOnlySpanFromNullTerminated(msg.str)),
            FMessageType.CStr16 => new string((char*)msg.str),
            FMessageType.Slice8 => Encoding.UTF8.GetString(new ReadOnlySpan<byte>(msg.str, len)),
            FMessageType.Slice16 => new string((char*)msg.str, 0, len),
            FMessageType.String8 => new String8(msg.string8).TakeString(),
            FMessageType.String16 => new String16(msg.string16).TakeString(),
            _ => throw new ArgumentOutOfRangeException()
        };
}
