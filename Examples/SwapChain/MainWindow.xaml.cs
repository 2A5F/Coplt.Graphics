﻿using System.Windows;
using System.Windows.Interop;
using Serilog;

namespace Examples;

/// <summary>
/// Interaction logic for MainWindow.xaml
/// </summary>
public partial class MainWindow : Window
{
    public readonly Example Example;

    #region Ctor

    public MainWindow()
    {
        InitializeComponent();
        Example = new(new WindowInteropHelper(this).EnsureHandle(), (uint)Width, (uint)Height);
        Title = "Swap Chain";
        Example.DoInitGraphics();
    }

    #endregion

    #region Misc

    protected override void OnClosed(EventArgs e)
    {
        Example.IsClosed = true;
    }

    protected override void OnRenderSizeChanged(SizeChangedInfo sizeInfo)
    {
        Log.Information("OnRenderSizeChanged {NewSize}", sizeInfo.NewSize);
        Example.SwapChain.Resize((uint)sizeInfo.NewSize.Width, (uint)sizeInfo.NewSize.Height);
    }

    #endregion
}
