using System.Diagnostics;
using System.IO;
using System.Reflection;
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
using Coplt.Graphics.Core;
using Coplt.Mathematics;
using Serilog;
using Path = System.IO.Path;

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

    private Shader Shader = null!;

    private bool IsClosed = false;

    #region Ctor

    public MainWindow()
    {
        InitializeComponent();
        InitGraphics();
    }

    #endregion

    #region Misc

    protected override void OnClosed(EventArgs e)
    {
        IsClosed = true;
    }

    protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
    {
        Log.Information("OnRenderSizeChanged {NewSize}", sizeInfo.NewSize);
        Output.Resize((uint)sizeInfo.NewSize.Width, (uint)sizeInfo.NewSize.Height);
    }

    #endregion

    #region InitGraphics

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
            LoadResources(cmd).Wait();
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

    #endregion

    #region LoadShaderModule

    async Task<ShaderModule> LoadShaderModule(ShaderStage stage, string path)
    {
        var blob = await File.ReadAllBytesAsync(Path.Join(AppContext.BaseDirectory, path));
        return Device.CreateShaderModule(stage, blob);
    }
    Task<ShaderModule[]> LoadShaderModules(string name, ReadOnlySpan<ShaderStage> stages)
    {
        var tasks = new Task<ShaderModule>[stages.Length];
        for (int i = 0; i < stages.Length; i++)
        {
            var stage = stages[i];
            tasks[i] = LoadShaderModule(stage, $"{name}.{stage}.dxil");
        }
        return Task.WhenAll(tasks);
    }

    #endregion

    #region LoadResources

    async Task LoadResources(CommandList cmd)
    {
        var modules = await LoadShaderModules("HelloTriangle", [ShaderStage.Vertex, ShaderStage.Pixel]);
        Shader = Device.CreateShader(modules, null, Device.CreateShaderInputLayout(["POSITION", "COLOR"]));

        // todo        
    }

    #endregion

    #region Render

    void Render(CommandList cmd)
    {
        cmd.ClearColor(Output, new float4(1, 1, 1, 1));
    }

    #endregion
}
