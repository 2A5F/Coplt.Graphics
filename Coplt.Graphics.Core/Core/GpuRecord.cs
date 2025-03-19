using Coplt.Dropping;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public sealed unsafe class GpuRecord : IsolateChild
{
    #region Fields

    internal FGpuRecordData* m_data;

    #endregion

    #region Props

    public ref readonly FGpuRecordData Data => ref *m_data;
    internal ref FGpuRecordData MutData => ref *m_data;

    #endregion

    #region Ctor

    internal GpuRecord(FGpuRecord* ptr, FGpuRecordData* data, string? name, GpuIsolate isolate) : base((FGpuObject*)ptr, name, isolate)
    {
        m_data = data;
    }

    #endregion

    #region Dispose

    protected override void Dispose(bool disposing)
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        var p = (FGpuRecord*)ptr;
        Isolate.Ptr->ReturnRecords(1, &p).TryThrow();
        m_data = null;
    }

    #endregion
}
