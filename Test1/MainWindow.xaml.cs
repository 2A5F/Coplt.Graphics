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
using Serilog;

namespace Test1;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    private GraphicsInstance Graphics;
    private GpuDevice Device;
    private GpuOutput Output;
    private IntPtr Handle;

    public MainWindow()
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
            }, Handle);
        }
        catch (Exception e)
        {
            Log.Error(e, "");
        }
        InitializeComponent();
    }

    private void OnLoaded(object sender, RoutedEventArgs e) { }
}
