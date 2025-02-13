using System.Collections.Concurrent;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using Coplt.Dropping;
using Coplt.Graphics.Native;
using Coplt.Graphics.Native.D3d12;
using Coplt.Graphics.Utilities;

namespace Coplt.Graphics.Core;

[Dropping(Unmanaged = true)]
public unsafe partial class GraphicsInstance
{
    #region Static

    public static event Action<Exception>? OnUnhandledException;

    internal static void EmitOnUnhandledException(Exception e) => OnUnhandledException?.Invoke(e);

    #endregion

    #region Fields

    internal FInstance* m_ptr;
    internal readonly ConcurrentDictionary<string, String8> m_string_cache = new();
    internal readonly ConcurrentDictionary<string, uint> m_slot_id_cache = new(StringComparer.OrdinalIgnoreCase);
    internal uint m_slot_id_inc;

    #endregion

    #region Props

    public FInstance* Ptr => m_ptr;

    #endregion

    #region Ctor

    public GraphicsInstance(FInstance* ptr) => m_ptr = ptr;

    #endregion

    #region Drop

    [Drop]
    private void Drop()
    {
        if (ExchangeUtils.ExchangePtr(ref m_ptr, null, out var ptr) is null) return;
        ptr->Release();
    }

    #endregion

    #region Load

    public static GraphicsInstance LoadD3d12() => new(D3d12Native.Coplt_Graphics_D3d12_Create_Instance());

    #endregion

    #region Logger

    #region Unsafe

    public void SetLogger(
        delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, FMessage*, void> pf_logger
    )
    {
        FLogger logger = new() { p_object = null, pf_enable = null, pf_logger = pf_logger, pf_drop = null };
        m_ptr->SetLogger(&logger).TryThrow();
    }

    public void SetLogger(
        delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, B8> pf_enable,
        delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, FMessage*, void> pf_logger
    )
    {
        FLogger logger = new() { p_object = null, pf_enable = pf_enable, pf_logger = pf_logger, pf_drop = null };
        m_ptr->SetLogger(&logger).TryThrow();
    }
    public void SetLogger(
        void* p_object,
        delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, B8> pf_enable,
        delegate* unmanaged[Cdecl]<void*, FLogLevel, FLogType, FMessage*, void> pf_logger,
        delegate* unmanaged[Cdecl]<void*, void> pf_drop
    )
    {
        FLogger logger = new() { p_object = p_object, pf_enable = pf_enable, pf_logger = pf_logger, pf_drop = pf_drop };
        m_ptr->SetLogger(&logger).TryThrow();
    }

    #endregion

    #region Proxy0

    public void SetLogger(Action<FLogLevel, FLogType, string> logger)
    {
        var obj = new LoggerProxy0(logger);
        var handle = GCHandle.Alloc(obj);
        var ptr = (void*)GCHandle.ToIntPtr(handle);
        FLogger f_logger = new()
        {
            p_object = ptr,
            pf_enable = null,
            pf_logger = &LoggerProxy0.Log,
            pf_drop = &Utils.NativeDropGCHandle,
        };
        m_ptr->SetLogger(&f_logger).TryThrow();
    }

    private sealed class LoggerProxy0(Action<FLogLevel, FLogType, string> logger)
    {
        private readonly Action<FLogLevel, FLogType, string> logger = logger;

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
        public static void Log(void* obj, FLogLevel level, FLogType type, FMessage* msg)
        {
            try
            {
                var str = msg->GetString();
                var self = (LoggerProxy0)GCHandle.FromIntPtr((nint)obj).Target!;
                self.logger(level, type, str);
            }
            catch (Exception e)
            {
                try
                {
                    OnUnhandledException?.Invoke(e);
                }
                catch
                {
                    // ignored
                }
            }
        }
    }

    #endregion

    #region Proxy1

    public void SetLogger(Func<FLogLevel, FLogType, bool> enable, Action<FLogLevel, FLogType, string> logger)
    {
        var obj = new LoggerProxy1(enable, logger);
        var handle = GCHandle.Alloc(obj);
        var ptr = (void*)GCHandle.ToIntPtr(handle);
        FLogger f_logger = new()
        {
            p_object = ptr,
            pf_enable = &LoggerProxy1.Enable,
            pf_logger = &LoggerProxy1.Log,
            pf_drop = &Utils.NativeDropGCHandle,
        };
        m_ptr->SetLogger(&f_logger).TryThrow();
    }

    private sealed class LoggerProxy1(
        Func<FLogLevel, FLogType, bool> enable,
        Action<FLogLevel, FLogType, string> logger)
    {
        private readonly Func<FLogLevel, FLogType, bool> enable = enable;
        private readonly Action<FLogLevel, FLogType, string> logger = logger;

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
        public static B8 Enable(void* obj, FLogLevel level, FLogType type)
        {
            try
            {
                var self = (LoggerProxy1)GCHandle.FromIntPtr((nint)obj).Target!;
                return self.enable(level, type);
            }
            catch (Exception e)
            {
                try
                {
                    OnUnhandledException?.Invoke(e);
                }
                catch
                {
                    // ignored
                }
            }
            return false;
        }

        [UnmanagedCallersOnly(CallConvs = [typeof(CallConvCdecl)])]
        public static void Log(void* obj, FLogLevel level, FLogType type, FMessage* msg)
        {
            try
            {
                var str = msg->GetString();
                var self = (LoggerProxy1)GCHandle.FromIntPtr((nint)obj).Target!;
                self.logger(level, type, str);
            }
            catch (Exception e)
            {
                try
                {
                    OnUnhandledException?.Invoke(e);
                }
                catch
                {
                    // ignored
                }
            }
        }
    }

    #endregion

    #endregion

    #region String

    public String8 CreateString8(ReadOnlySpan<byte> str, string? managed = null)
    {
        fixed (byte* p_str = str)
        {
            FString8* ptr;
            m_ptr->CreateString8(p_str, (nuint)str.Length, &ptr).TryThrow();
            return new(ptr, managed);
        }
    }

    public String16 CreateString16(ReadOnlySpan<char> str, string? managed = null)
    {
        fixed (char* p_str = str)
        {
            FString16* ptr;
            m_ptr->CreateString16(p_str, (nuint)str.Length, &ptr).TryThrow();
            return new(ptr, managed);
        }
    }

    public String8 CreateString8(string str) => CreateString8(Encoding.UTF8.GetBytes(str), str);
    public String16 CreateString16(string str) => CreateString16(str, str);

    #region CacheString8

    private Func<string, String8>? m_cache_CreateString8;
    public String8 CacheString8(string str) => m_string_cache.GetOrAdd(str, m_cache_CreateString8 ??= CreateString8);

    #endregion

    #endregion

    #region SlotId

    private uint AllocSlotId(string _) => m_slot_id_inc++;
    private Func<string, uint>? m_cache_AllocSlotId;
    public uint GetSlotId(string name) => m_slot_id_cache.GetOrAdd(name, m_cache_AllocSlotId ??= AllocSlotId);

    #endregion

    #region CreateDevice

    public GpuDevice CreateDevice(
        GpuDeviceOptions options = default,
        GpuPreference Preference = GpuPreference.HighPerformance,
        string? Name = null,
        ReadOnlySpan<byte> Name8 = default,
        bool Debug = false
    )
    {
        var QueueName = !Debug || Name is null ? null : $"{Name} Main Queue";
        var QueueName8 = !Debug || Name8.Length == 0 ? Name8 : Utils.JoinUtf8String(Name8, " Main Queue"u8);

        fixed (char* p_name = Name)
        {
            fixed (byte* p_name8 = Name8)
            {
                FGpuDeviceCreateOptions f_options = new()
                {
                    Name = new(Name, Name8, p_name, p_name8),
                    D3dFeatureLevel = (FD3dFeatureLevel)options.D3dFeatureLevel,
                    VulkanVersion = (FVulkanVersion)options.VulkanVersion,
                    Preference = (FGpuPreference)Preference,
                    Debug = Debug
                };
                FGpuDevice* ptr;
                m_ptr->CreateDevice(&f_options, &ptr).TryThrow();
                return new(ptr, this, Name, QueueName: QueueName, QueueName8: QueueName8);
            }
        }
    }

    #endregion
}
