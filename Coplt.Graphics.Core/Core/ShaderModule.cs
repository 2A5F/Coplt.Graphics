using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public sealed unsafe partial class ShaderModule : DeviceChild
{
    #region Fields

    internal FShaderModuleData* m_data;
    internal readonly String8? m_entry_point;

    #endregion

    #region Props

    public new FShaderModule* Ptr => (FShaderModule*)m_ptr;
    public ref readonly FShaderModuleData Data => ref *m_data;
    public ShaderStage Stage => Data.Stage.FromFFI();
    public String8? EntryPoint => m_entry_point;
    public FSlice<byte> Blob => new(Data.Data, Data.Size);

    #endregion

    #region Ctor

    internal ShaderModule(FShaderModuleCreateResult result, String8? entry_point, string? name, GpuDevice device)
        : base((FGpuObject*)result.ShaderModule, name, device)
    {
        m_data = result.Data;
        m_entry_point = entry_point;
    }

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        m_data = null;
    }

    #endregion
}
