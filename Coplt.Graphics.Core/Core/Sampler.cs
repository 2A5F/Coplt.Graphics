using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.States;

namespace Coplt.Graphics.States
{
    public enum FilterMode : byte
    {
        Point,
        Linear,
    }

    public enum AddressMode : byte
    {
        Repeat,
        Mirror,
        Clamp,
        Border,
        MirrorOnce,
    }

    public record struct SamplerInfo()
    {
        public Color BorderColor;
        public uint MaxAnisotropy;
        public float MipLodBias;
        public float MinLod;
        public float MaxLod = 3.402823466E38f;
        public CmpFunc Cmp;
        public FilterMode Mag;
        public FilterMode Min;
        public FilterMode Mipmap;
        public AddressMode U;
        public AddressMode V;
        public AddressMode W;

        public static readonly SamplerInfo PointRepeat = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Point,
        };

        public static readonly SamplerInfo LinearRepeat = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Point,
        };

        public static readonly SamplerInfo TrilinearRepeat = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Linear,
        };

        public static readonly SamplerInfo PointClamp = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Point,
            U = AddressMode.Clamp,
            V = AddressMode.Clamp,
            W = AddressMode.Clamp,
        };

        public static readonly SamplerInfo LinearClamp = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Point,
            U = AddressMode.Clamp,
            V = AddressMode.Clamp,
            W = AddressMode.Clamp,
        };

        public static readonly SamplerInfo TrilinearClamp = new()
        {
            Mag = FilterMode.Linear,
            Min = FilterMode.Linear,
            Mipmap = FilterMode.Linear,
            U = AddressMode.Clamp,
            V = AddressMode.Clamp,
            W = AddressMode.Clamp,
        };
    }
}

namespace Coplt.Graphics.Core
{
    [Dropping(Unmanaged = true)]
    public sealed unsafe partial class Sampler : DeviceChild
    {
        #region Fields

        internal FSamplerInfo* m_info;

        #endregion

        #region Props

        public new FGpuSampler* Ptr => (FGpuSampler*)m_ptr;
        public ref readonly SamplerInfo Info => ref *(SamplerInfo*)m_info;

        #endregion

        #region Ctor

        internal Sampler(FGpuSampler* ptr, string? name, GpuDevice device) : base((FGpuObject*)ptr, name, device)
        {
            m_info = ptr->Info();
        }

        #endregion

        #region Drop

        [Drop]
        private void Drop()
        {
            m_info = null;
        }

        #endregion
    }
}
