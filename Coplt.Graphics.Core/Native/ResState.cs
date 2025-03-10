using Coplt.Graphics.Core;
using Coplt.Graphics.Native;

namespace Coplt.Graphics.Native
{
    public partial struct FResState
    {
        /// <summary>
        /// 判断状态是否兼容
        /// </summary>
        public readonly bool IsCompatible(in FResState other, bool legacy, /* 顺序 */ bool b2a, out bool same)
        {
            if (legacy)
            {
                same = Legacy == other.Legacy;
                return Legacy.FromFFI().IsCompatible(other.Legacy.FromFFI());
            }
            if (!Layout.FromFFI().IsCompatible(other.Layout.FromFFI()) || CrossQueue != other.CrossQueue) goto no;
            if (Access != other.Access)
            {
                same = false;
                var sna = (Access.FromFFI() & ResAccess.NoAccess) != 0;
                var ona = (other.Access.FromFFI() & ResAccess.NoAccess) != 0;
                if (sna && ona) return true;
                if (sna || ona)
                {
                    if (b2a)
                    {
                        if (sna && !ona) return true;
                        else if (!sna && ona) return false;
                    }
                    else
                    {
                        if (!sna && ona) return true;
                        else if (sna && !ona) return false;
                    }
                }
                if (!Access.FromFFI().IsReadOnly() || !other.Access.FromFFI().IsReadOnly()) goto no;
            }
            else
            {
                same = Stages == other.Stages;
            }
            return true;
        no:
            same = false;
            return false;
        }

        /// <summary>
        /// 在兼容的时候合并，不兼容的话是未定义行为
        /// </summary>
        public readonly FResState Merge(in FResState other, bool legacy, /* 顺序 */ bool b2a)
        {
            if (legacy)
            {
                return new()
                {
                    Layout = Layout,
                    Access = Access,
                    Stages = Stages,
                    // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                    Legacy = Legacy | other.Legacy,
                    CrossQueue = CrossQueue,
                };
            }
            var sna = (Access.FromFFI() & ResAccess.NoAccess) != 0;
            var ona = (other.Access.FromFFI() & ResAccess.NoAccess) != 0;
            if (b2a)
            {
                var Layout = this.Layout.FromFFI().Merge(other.Layout.FromFFI(), sna).ToFFI();
                if (sna)
                    return new()
                    {
                        Layout = Layout,
                        // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                        Access = other.Access,
                        // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                        Stages = Stages | other.Stages,
                        Legacy = Legacy,
                        CrossQueue = CrossQueue,
                    };
            }
            else
            {
                var Layout = this.Layout.FromFFI().Merge(other.Layout.FromFFI(), ona).ToFFI();
                if (ona)
                    return new()
                    {
                        Layout = Layout,
                        // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                        Access = Access,
                        // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                        Stages = Stages | other.Stages,
                        Legacy = Legacy,
                        CrossQueue = CrossQueue,
                    };
            }
            return new()
            {
                Layout = this.Layout.FromFFI().Merge(other.Layout.FromFFI(), false).ToFFI(),
                // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                Access = Access | other.Access,
                // ReSharper disable once BitwiseOperatorOnEnumWithoutFlags
                Stages = Stages | other.Stages,
                Legacy = Legacy,
                CrossQueue = CrossQueue,
            };
        }

        public override string ToString() =>
            $"{{ {nameof(Layout)} = {Layout}, {nameof(Access)} = {Access}, {nameof(Stages)} = {Stages}, {nameof(Legacy)} = {Legacy}, {nameof(CrossQueue)} = {CrossQueue} }}";
    }
}

namespace Coplt.Graphics.Core
{
    public static partial class GraphicsExtensions
    {
        public static bool ContainsOnly(this ResAccess value, ResAccess other) => (value & ~other) == 0;
        public static bool Contains(this ResAccess value, ResAccess other) => (value & other) != 0;
        public static bool IsReadOnly(this ResAccess value) => value.ContainsOnly(
            ResAccess.IndexBuffer | ResAccess.ConstantBuffer | ResAccess.VertexBuffer | ResAccess.CopySrc |
            ResAccess.ResolveSrc | ResAccess.DepthStencilRead | ResAccess.IndirectOrPredicationBuffer |
            ResAccess.ShaderResource | ResAccess.ShadingRate
        );
    }
}
