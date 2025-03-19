using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public sealed unsafe class GpuRecord : IsolateChild
{
    #region Fields

    internal FGpuRecordData* m_data;
    internal readonly Dictionary<object, uint> m_resources = new();

    #endregion

    #region Props

    public new FGpuRecord* Ptr => (FGpuRecord*)m_ptr;
    internal ref FGpuRecordData Data => ref *m_data;

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

    #region Reset

    internal void Reset()
    {
        m_resources.Clear();
    }

    #endregion

    #region End

    public void AssertNotEnded()
    {
        if (Data.Ended) throw new InvalidOperationException("Gpu record is already ended");
    }

    public void End()
    {
        AssertNotEnded();
        Ptr->End().TryThrow();
    }

    public void EnsureEnded()
    {
        if (Data.Ended) return;
        Ptr->End().TryThrow();
    }

    #endregion

    #region AddResource

    internal FCmdResRef AddResource(IGpuResource resource)
    {
        if (resource.Isolate != Isolate)
            throw new ArgumentException($"Resource {resource} does not belong to isolate {Isolate}");
        ref var slot = ref CollectionsMarshal.GetValueRefOrAddDefault(m_resources, m_resources, out var exists);
        if (!exists)
        {
            var len = Data.Resources.LongLength;
            if (len + 1 >= uint.MaxValue) throw new IndexOutOfRangeException("Resources >= uint32 max are not supported");
            slot = (uint)len;
            if (resource is GpuOutput) Data.OutputCount++;
            Data.Resources.Add(resource.IntoCmd());
        }
        return new() { IndexPlusOne = slot + 1 };
    }

    #endregion

    #region AddRects

    internal uint AddRects(ReadOnlySpan<URect> Rects)
    {
        var len = Data.Payload.LongLength;
        if (len >= uint.MaxValue) throw new IndexOutOfRangeException("Payload >= uint32 max are not supported");
        Data.Payload.EnsureCap(len + (uint)Rects.Length);
        foreach (var rect in Rects)
        {
            Data.Payload.Add(
                new()
                {
                    Rect = new()
                    {
                        Left = rect.Left,
                        Top = rect.Top,
                        Right = rect.Right,
                        Bottom = rect.Bottom,
                    }
                }
            );
        }
        return (uint)len;
    }

    #endregion

    #region ClearColor

    public void ClearColor(IRtv Image, ReadOnlySpan<URect> Rects = default) =>
        ClearColor(Image, new(0, 0, 0, 1), Rects);

    public void ClearColor(IRtv Image, Color Color, ReadOnlySpan<URect> Rects = default)
    {
        AssertNotEnded();
        Image.AssertSameIsolate(Isolate);
        if (!Image.TryRtv()) throw new ArgumentException($"Resource {Image} cannot be used as Rtv");
        var cmd = new FCmdClearColor
        {
            Base = { Type = FCmdType.ClearColor },
            RectCount = (uint)Rects.Length,
            RectIndex = Rects.Length <= 0 ? 0 : AddRects(Rects),
            Image = AddResource(Image.Resource),
            Color = Unsafe.BitCast<Color, FCmdClearColor._Color_e__FixedBuffer>(Color),
        };
        Data.Commands.Add(new() { ClearColor = cmd });
    }

    #endregion
}
