﻿using System.Diagnostics;
using System.Reflection;
using Coplt.Graphics;
using Coplt.Graphics.Core;
using Serilog;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

namespace Examples;

public abstract class ExampleBase(IntPtr Handle, uint Width, uint Height)
{
    public uint Width = Width;
    public uint Height = Height;
    public float AspectRatio = (float)((double)Width / Height);
    public bool IsClosed = false;
    public GraphicsInstance Graphics = null!;
    public GpuDevice Device = null!;
    public GpuIsolate Isolate = null!;
    public GpuSwapChain Output = null!;
    public GpuRecord Record = null!;
    public IntPtr Handle = Handle;

    public abstract string Name { get; }

    #region InitGraphics

    protected virtual void InitGraphics() { }

    public void DoInitGraphics()
    {
        try
        {
            Graphics = GraphicsInstance.LoadD3d12(Debug: true);
            Graphics.SetLogger(
                (level, _) => Log.IsEnabled(level.ToLogEventLevel()),
                (level, scope, msg) => Log.Write(level.ToLogEventLevel(), "[{Scope}] {Msg}", scope, msg)
            );
            Device = Graphics.CreateDevice(new()
            {
                // DeviceType = DeviceTypeRequire.IntegratedGpu,
            }, Name: "Main Device");
            var Adapter = Device.Adapter;
            Log.Information(
                "Selected device: {@Info}",
                new
                {
                    Adapter.Name, Adapter.VendorId, Adapter.DeviceId, Adapter.Driver, Adapter.Backend,
                    Adapter.Features.ShaderModelLevel, Adapter.Features.UMA, Adapter.Features.CacheCoherentUMA
                }
            );
            Isolate = Device.MainIsolate;
            Output = Isolate.CreateSwapChainForHwnd(
                Handle, new()
                {
                    Width = Width,
                    Height = Height,
                    // VSync = true,
                    // Srgb = false,
                }
            );
            Record = Isolate.RentRecord();
            InitGraphics();
            new Thread(
                () =>
                {
                    Thread.CurrentThread.Name = "Render Thread";
                    LoadResources(Record).Wait();
                    var start_time = Stopwatch.GetTimestamp();
                    var last_time = start_time;
                    while (!IsClosed)
                    {
                        try
                        {
                            var now_time = Stopwatch.GetTimestamp();
                            var total_time = Stopwatch.GetElapsedTime(start_time, now_time);
                            var delta_time = Stopwatch.GetElapsedTime(last_time, now_time);
                            last_time = now_time;
                            var time = new Time
                            {
                                Total = total_time,
                                Delta = delta_time
                            };

                            Render(Record, time);
                            Record.PreparePresent(Output);
                            Isolate.Submit(Record);
                            Output.Present();
                        }
                        catch (Exception e)
                        {
                            Log.Error(e, "");
                            Environment.Exit(1);
                        }
                    }
                }
            ).Start();
        }
        catch (Exception e)
        {
            Log.Error(e, "");
            Environment.Exit(1);
        }
    }

    #endregion

    #region LoadShaderModule

    protected async Task<ShaderModule> LoadShaderModule(ShaderStage stage, string path)
    {
        var blob = await File.ReadAllBytesAsync(Path.Join(AppContext.BaseDirectory, path));
        return Device.CreateShaderModule(stage, blob);
    }
    protected Task<ShaderModule[]> LoadShaderModules(string name, ReadOnlySpan<ShaderStage> stages)
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

    #region LoadImage

    public static Task<Image<Rgba32>> LoadImage(string path) => LoadImage<Rgba32>(path);
    public static Task<Image<T>> LoadImage<T>(string path) where T : unmanaged, IPixel<T> => Image.LoadAsync<T>(
        Path.Join(Path.GetDirectoryName(Assembly.GetExecutingAssembly().Location), path)
    );

    #endregion

    #region LoadResources

    protected virtual Task LoadResources(GpuRecord cmd) => Task.CompletedTask;

    #endregion

    #region Render

    protected virtual void Render(GpuRecord cmd, Time time) { }

    #endregion
}
