#pragma once

#include "Object.h"
#include "Result.h"
#include "Features.h"
#include "Device.h"

namespace Coplt
{

    COPLT_INTERFACE_DEFINE(FGpuAdapter, "b3bbbed6-e997-4bd5-be53-0aac0ca8114d", FUnknown)
    {
        FStr8or16 m_name{};
        FStr8or16 m_driver{};
        FStr8or16 m_driver_info{};
        u32 m_vendor_id{};
        u32 m_device_id{};
        FD3dFeatureLevel m_d3d_feature_level{};
        FVulkanVersion m_vulkan_version{};
        FDeviceType m_device_type{};
        FBackend m_backend{};
        FDeviceFeatures m_features{};

        virtual b8 IsSupport(const FDeviceRequires& Requires) noexcept = 0;

        virtual FResult CreateDevice(const FGpuDeviceCreateOptions& options, FGpuDeviceCreateResult* out) noexcept = 0;
    };
}
