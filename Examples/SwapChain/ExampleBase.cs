using System.Diagnostics;
using System.Reflection;
using Coplt.Graphics;
using Coplt.Graphics.Core;
using Serilog;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Examples;

public class Example(IntPtr Handle, uint Width, uint Height)
{
    public bool IsClosed = false;
    public GraphicsInstance Graphics = null!;
    public GpuDevice Device = null!;
    public GpuIsolate Isolate = null!;
    public GpuSwapChain SwapChain = null!;
    public IntPtr Handle = Handle;

    #region InitGraphics

    public void DoInitGraphics()
    {
        try
        {
            Graphics = GraphicsInstance.LoadD3d12(Debug: true);
            Graphics.SetLogger(
                (level, _) => Log.IsEnabled(level.ToLogEventLevel()),
                (level, scope, msg) => Log.Write(level.ToLogEventLevel(), "[{Scope}] {Msg}", scope, msg)
            );
            Device = Graphics.CreateDevice(Name: "Main Device");
            var Adapter = Device.Adapter;
            Log.Information(
                "Selected device: {@Info}",
                new
                {
                    Adapter.Name, Adapter.VendorId, Adapter.DeviceId, Adapter.Driver, Adapter.Backend,
                    Adapter.Features.ShaderModelLevel
                }
            );
            Isolate = Device.CreateIsolate(Name: "Main Isolate");
            SwapChain = Isolate.CreateSwapChainForHwnd(
                Handle, new()
                {
                    Width = Width,
                    Height = Height,
                    // VSync = true,
                }, Name: "Main Swap Chain"
            );
            new Thread(
                () =>
                {
                    Thread.CurrentThread.Name = "Render Thread";
                    while (!IsClosed)
                    {
                        try
                        {
                            SwapChain.Present();
                        }
                        catch (Exception e)
                        {
                            Log.Error(e, "");
                        }
                    }
                }
            ).Start();
        }
        catch (Exception e)
        {
            Log.Error(e, "");
        }
    }

    #endregion
}