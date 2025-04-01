using System.Buffers;
using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public record struct ShaderBindingSetItem(uint Index, View View);

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderBinding
{
   
}
