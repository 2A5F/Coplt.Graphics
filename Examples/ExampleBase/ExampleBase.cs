﻿using System.Diagnostics;
using Coplt.Graphics;
using Coplt.Graphics.Core;
using Serilog;

namespace Examples;

public abstract class ExampleBase(IntPtr Handle, uint Width, uint Height)
{
    public bool IsClosed = false;
    public GraphicsInstance Graphics = null!;
    public GpuDevice Device = null!;
    public GpuOutput Output = null!;
    public IntPtr Handle = Handle;

    public abstract string Name { get; }

    #region InitGraphics

    protected virtual void InitGraphics() { }

    public void DoInitGraphics()
    {
        try
        {
            Graphics = GraphicsInstance.LoadD3d12();
            Graphics.SetLogger(
                (level, _) => Log.IsEnabled(level.ToLogEventLevel()),
                (level, scope, msg) => Log.Write(level.ToLogEventLevel(), "[{Scope}] {Msg}", scope, msg)
            );
            Device = Graphics.CreateDevice(Debug: true, Name: "Main Device");
            Output = Device.MainQueue.CreateOutputForHwnd(
                new()
                {
                    Width = Width,
                    Height = Height,
                    // VSync = true,
                }, Handle
            );
            InitGraphics();
        }
        catch (Exception e)
        {
            Log.Error(e, "");
        }
        new Thread(
            () =>
            {
                Thread.CurrentThread.Name = "Render Thread";
                var cmd = Device.MainCommandList;
                LoadResources(cmd).Wait();
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

                        Render(cmd, time);
                        Output.Present();
                    }
                    catch (Exception e)
                    {
                        Log.Error(e, "");
                    }
                }
            }
        ).Start();
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

    #region LoadResources

    protected abstract Task LoadResources(CommandList cmd);

    #endregion

    #region Render

    protected abstract void Render(CommandList cmd, Time time);

    #endregion
}
