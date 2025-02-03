using System.Runtime.InteropServices;

namespace Coplt.Graphics.Native.D3d12
{
    public static unsafe partial class D3d12Native
    {
        [DllImport("Coplt.Graphics.Native.D3d12", CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        [return: NativeTypeName("Coplt::FInstance *")]
        public static extern FInstance* Coplt_Graphics_D3d12_Create_Instance();
    }
}
