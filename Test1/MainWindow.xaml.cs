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
    private IntPtr Handle;

    public MainWindow()
    {
        Graphics = GraphicsInstance.LoadD3d12();
        Graphics.SetLogger((level, _) => Log.IsEnabled(level.ToLogEventLevel()),
            (level, _, msg) => Log.Write(level.ToLogEventLevel(), "{Msg}", msg));
        Handle = new WindowInteropHelper(this).EnsureHandle();
        InitializeComponent();
    }

    private void OnLoaded(object sender, RoutedEventArgs e) { }
}
