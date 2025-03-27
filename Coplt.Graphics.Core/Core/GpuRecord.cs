using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Core;

public sealed unsafe class GpuRecord : IsolateChild
{
    #region Fields

    internal FGpuRecordData* m_data;
    internal readonly Dictionary<object, uint> m_resources = new();
    internal int m_debug_scope_count;

    #endregion

    #region Props

    public new FGpuRecord* Ptr => (FGpuRecord*)m_ptr;
    internal ref FGpuRecordData Data => ref *m_data;
    public bool DebugEnabled { get; }

    #endregion

    #region Ctor

    internal GpuRecord(FGpuRecord* ptr, FGpuRecordData* data, string? name, GpuIsolate isolate) : base((FGpuObject*)ptr, name, isolate)
    {
        m_data = data;
        DebugEnabled = Isolate.Device.Instance.DebugEnabled;
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
        AssertEndOrCanEnd();
        Ptr->End().TryThrow();
    }

    public void EnsureEnded()
    {
        if (Data.Ended) return;
        AssertEndOrCanEnd();
        Ptr->End().TryThrow();
    }

    internal void AssertEndOrCanEnd()
    {
        if (Data.Ended) return;
        if (m_debug_scope_count != 0)
            throw new InvalidOperationException(
                "There is still Debug scope not ended, please check whether Dispose is missed."
            );
    }

    #endregion

    #region Payload

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
            Data.Resources.Add(resource.IntoCmd());
        }
        return new() { IndexPlusOne = slot + 1 };
    }

    #endregion

    #region AddRects

    internal uint AddRects(ReadOnlySpan<URect> Rects)
    {
        var len = Data.PayloadRect.LongLength;
        if (len >= uint.MaxValue) throw new IndexOutOfRangeException("Payload >= uint32 max are not supported");
        Data.PayloadRect.EnsureCap(len + (uint)Rects.Length);
        Data.PayloadRect.AddRange(MemoryMarshal.Cast<URect, FRect>(Rects));
        return (uint)len;
    }

    #endregion

    #region AddString

    internal uint AddString(string str)
    {
        var index = Data.Blob.LongLength;
        Data.Blob.EnsureCap((nuint)str.Length * 2 + 2);
        Data.Blob.AddRange(MemoryMarshal.AsBytes(str.AsSpan()));
        Data.Blob.AddRange([0, 0]);
        return (uint)index;
    }

    internal uint AddString(ReadOnlySpan<byte> str)
    {
        var index = Data.Blob.LongLength;
        Data.Blob.EnsureCap((nuint)str.Length + 1);
        Data.Blob.AddRange(str);
        Data.Blob.Add(0);
        return (uint)index;
    }

    #endregion

    #endregion

    #region Debug

    #region Label

    public void Label(string Label, Color? Color = null)
    {
        if (!DebugEnabled) return;
        var cmd = new FCmdLabel
        {
            Base = { Type = FCmdType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str16,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { Label = cmd });
    }

    public void Label(ReadOnlySpan<byte> Label, Color? Color = null)
    {
        if (!DebugEnabled) return;
        var cmd = new FCmdLabel
        {
            Base = { Type = FCmdType.Label },
            StringIndex = AddString(Label),
            StringLength = (uint)Label.Length,
            StrType = FStrType.Str8,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { Label = cmd });
    }

    #endregion

    #region Scope

    public DebugScope2 Scope(string Name, Color? Color = null)
    {
        if (!DebugEnabled) return new(this);
        var cmd = new FCmdBeginScope
        {
            Base = { Type = FCmdType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str16,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { BeginScope = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    public DebugScope2 Scope(ReadOnlySpan<byte> Name, Color? Color = null)
    {
        if (!DebugEnabled) return new(this);
        var cmd = new FCmdBeginScope
        {
            Base = { Type = FCmdType.BeginScope },
            StringIndex = AddString(Name),
            StringLength = (uint)Name.Length,
            StrType = FStrType.Str8,
        };
        if (Color is { } color)
        {
            var c255 = color.To255;
            cmd.HasColor = true;
            cmd.Color[0] = (byte)c255.R;
            cmd.Color[1] = (byte)c255.G;
            cmd.Color[2] = (byte)c255.B;
        }
        Data.Commands.Add(new() { BeginScope = cmd });
        m_debug_scope_count++;
        return new(this);
    }

    #endregion

    #endregion

    #region PreparePresent

    public void PreparePresent(GpuOutput2 Output)
    {
        AssertNotEnded();
        Output.AssertSameIsolate(Isolate);
        var cmd = new FCmdPreparePresent
        {
            Base = { Type = FCmdType.PreparePresent },
            Output = AddResource(Output),
        };
        Data.Commands.Add(new() { PreparePresent = cmd });
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

#region DebugScope

public struct DebugScope2(GpuRecord self) : IDisposable
{
    private bool m_disposed;
    public void Dispose()
    {
        if (m_disposed) throw new ObjectDisposedException(nameof(ComputeScope));
        m_disposed = true;
        if (!self.DebugEnabled) return;
        var cmd = new FCmdEndScope
        {
            Base = { Type = FCmdType.EndScope },
        };
        self.Data.Commands.Add(new() { EndScope = cmd });
        self.m_debug_scope_count--;
    }
}

#endregion
