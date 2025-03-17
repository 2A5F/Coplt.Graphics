using System.Runtime.InteropServices;

namespace Coplt.Graphics.Native.D3d12
{
    public static unsafe partial class D3d12Native
    {
        [DllImport("Coplt.Graphics.Native.D3d12", CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        public static extern void Coplt_Graphics_D3d12_Create_Instance([NativeTypeName("Coplt::FResult *")] FResult* r, [NativeTypeName("const Coplt::FInstanceCreateOptions &")] FInstanceCreateOptions* options, [NativeTypeName("Coplt::FInstance **")] FInstance** @out);
    }
}
