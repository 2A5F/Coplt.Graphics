using System.Diagnostics;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Coplt.Graphics;
using Coplt.Mathematics;
using Serilog;

namespace Test1;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    private GraphicsInstance Graphics = null!;
    private GpuDevice Device = null!;
    private GpuOutput Output = null!;
    private IntPtr Handle;

    private bool IsClosed = false;

    public MainWindow()
    {
        InitializeComponent();
        InitGraphics();
    }

    protected override void OnClosed(EventArgs e)
    {
        IsClosed = true;
    }

    protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
    {
        Console.WriteLine($"OnRenderSizeChanged {sizeInfo.NewSize}");
        Output.Resize((uint)sizeInfo.NewSize.Width, (uint)sizeInfo.NewSize.Height);
    }

    void InitGraphics()
    {
        try
        {
            Graphics = GraphicsInstance.LoadD3d12();
            Graphics.SetLogger((level, _) => Log.IsEnabled(level.ToLogEventLevel()),
                (level, scope, msg) => Log.Write(level.ToLogEventLevel(), "[{Scope}] {Msg}", scope, msg));
            Device = Graphics.CreateDevice(Debug: true, Name: "Main Device");
            Handle = new WindowInteropHelper(this).EnsureHandle();
            Output = Device.MainQueue.CreateOutputForHwnd(new()
            {
                Width = (uint)Width,
                Height = (uint)Height,
                // VSync = true,
            }, Handle);
        }
        catch (Exception e)
        {
            Log.Error(e, "");
        }
        new Thread(() =>
        {
            Thread.CurrentThread.Name = "Render Thread";
            var cmd = Device.MainCommandList;
            LoadResources(cmd);
            while (!IsClosed)
            {
                try
                {
                    Render(cmd);
                    Output.Present();
                }
                catch (Exception e)
                {
                    Log.Error(e, "");
                }
            }
        }).Start();
    }

    void LoadResources(CommandList cmd)
    {
        // todo        
    }

    void Render(CommandList cmd)
    {
        cmd.ClearColor(Output, new float4(1, 1, 1, 1));
    }
}
