using System.Collections.Concurrent;
using System.Collections.Frozen;
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
    internal readonly List<GpuAdapter> m_adapters = new();
    internal readonly FrozenDictionary<nuint, GpuAdapter> m_ptr_to_adapters;
    internal readonly ConcurrentDictionary<string, String8> m_string_cache = new();
    internal readonly ConcurrentDictionary<string, SlotId> m_slot_id_cache = new(StringComparer.OrdinalIgnoreCase);
    internal readonly ConcurrentDictionary<SlotId, string> m_slot_id_name = new();
    internal readonly ConcurrentDictionary<SlotId, String8> m_slot_id_name8 = new();
    internal uint m_slot_id_inc;

    #endregion

    #region Props

    public FInstance* Ptr => m_ptr;
    public bool DebugEnabled => m_ptr->m_debug_enabled;
    public bool GpuBasedValidationEnabled => m_ptr->m_gpu_based_validation_enabled;
    public ReadOnlySpan<GpuAdapter> Adapters => CollectionsMarshal.AsSpan(m_adapters);

    #endregion

    #region Ctor

    public GraphicsInstance(FInstance* ptr)
    {
        m_ptr = ptr;
        uint count = 0;
        var adapters = m_ptr->GetAdapters(&count);
        var ptr_to_adapters = new Dictionary<nuint, GpuAdapter>();
        for (var i = 0; i < count; i++)
        {
            var adapter = adapters[i];
            adapter->AddRef();
            var obj = new GpuAdapter(adapter, this);
            m_adapters.Add(obj);
            ptr_to_adapters.Add((nuint)adapter, obj);
        }
        m_ptr_to_adapters = ptr_to_adapters.ToFrozenDictionary();
    }

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

    public static GraphicsInstance LoadD3d12(bool Debug = false, bool GpuBasedValidation = false)
    {
        FInstance* ptr = default;
        FResult r = default;
        FInstanceCreateOptions f_options = new()
        {
            Debug = Debug,
            GpuBasedValidation = GpuBasedValidation
        };
        D3d12Native.Coplt_Graphics_D3d12_Create_Instance(&r, &f_options, &ptr);
        r.TryThrow();
        return new(ptr);
    }

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
        Action<FLogLevel, FLogType, string> logger
    )
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

    #region Blob

    public Blob CreateBlob(nuint size)
    {
        FBlob* ptr;
        m_ptr->CreateBlob(size, &ptr).TryThrow();
        return new(ptr);
    }

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

    private SlotId AllocSlotId(string name)
    {
        var id = new SlotId(m_slot_id_inc++);
        m_slot_id_name.TryAdd(id, name);
        m_slot_id_name8.TryAdd(id, CacheString8(name));
        return id;
    }
    private Func<string, SlotId>? m_cache_AllocSlotId;
    /// <param name="name">不区分大小写</param> // todo 改进 SlotId，保留区分大小写的字符串
    public SlotId GetSlotId(string name) => m_slot_id_cache.GetOrAdd(name, m_cache_AllocSlotId ??= AllocSlotId);
    public string? TryGetSlotName(SlotId SlotId) => m_slot_id_name.GetValueOrDefault(SlotId);
    public String8? TryGetSlotName8(SlotId SlotId) => m_slot_id_name8.GetValueOrDefault(SlotId);

    #endregion

    #region CreateDevice

    public GpuDevice CreateDevice(
        DeviceRequires requires = default,
        string? Name = null, ReadOnlySpan<byte> Name8 = default
    )
    {
        fixed (char* p_name = Name)
        fixed (byte* p_name8 = Name8)
        {
            FGpuAutoSelectDeviceCreateOptions f_options = new()
            {
                Base = { Name = new(Name, Name8, p_name, p_name8) },
                Requires =
                {
                    D3dFeatureLevel = (FD3dFeatureLevel)requires.D3dFeatureLevel,
                    VulkanVersion = (FVulkanVersion)requires.VulkanVersion,
                    DeviceType = (FDeviceTypeRequire)requires.DeviceType,
                    FeatureRequires =
                    {
                        ShaderModelLevel = (FShaderModelLevel)requires.FeatureRequires.ShaderModelLevel,
                        RayTracing = requires.FeatureRequires.RayTracing,
                        MeshShader = requires.FeatureRequires.MeshShader,
                        DescriptorHeap = requires.FeatureRequires.DescriptorHeap,
                        EnhancedBarriers = requires.FeatureRequires.EnhancedBarriers,
                        ArrBindless = requires.FeatureRequires.ArrBindless,
                        DynBindless = requires.FeatureRequires.DynBindless,
                    },
                },
            };
            FGpuDeviceCreateResult result;
            m_ptr->CreateDevice(&f_options, &result).TryThrow();
            return new(result, this, null, Name);
        }
    }

    #endregion
}
