enum class Mode : uint
{
    Common,
    L4,
};

cbuffer Args : register(b0)
{
    Mode mode;
    uint mip_levels; // 0 .. 4
    uint2 size;
}

RWTexture2D<float4> images[] : register(u0);

groupshared float tmp_r[8][8];
groupshared float tmp_g[8][8];
groupshared float tmp_b[8][8];
groupshared float tmp_a[8][8];

void StoreColor(uint2 pos, float4 color)
{
    tmp_r[pos.y][pos.x] = color.r;
    tmp_g[pos.y][pos.x] = color.g;
    tmp_b[pos.y][pos.x] = color.b;
    tmp_a[pos.y][pos.x] = color.a;
}

float4 LoadColor(uint2 pos)
{
    return float4(
        tmp_r[pos.y][pos.x],
        tmp_g[pos.y][pos.x],
        tmp_b[pos.y][pos.x],
        tmp_a[pos.y][pos.x]
    );
}

void X8_8(uint2 gtid, uint2 gid, inout uint index, inout float4 color);
void X4_4(uint2 gtid, uint2 gid, inout uint index, inout float4 color);
void X2_2(uint2 gid, inout uint index, inout float4 color);

[shader("compute")]
[numthreads(8, 8, 1)]
void Compute(uint2 tid : SV_DispatchThreadID, uint2 gtid : SV_GroupThreadID, uint2 gid: SV_GroupID)
{
    uint index = 1;

    float4 color; // cur pixel color
    switch (mip_levels)
    {
    case 4:
        {
            uint2 pos = 2 * tid;
            RWTexture2D<float4> src = images[0]; // 16x16
            RWTexture2D<float4> dst = images[1]; // 8x8
            color = (
                src.Load(pos + uint2(0, 0)) +
                src.Load(pos + uint2(0, 1)) +
                src.Load(pos + uint2(1, 0)) +
                src.Load(pos + uint2(1, 1))
            ) * 0.25;
            dst[gtid] = color;
            index = 2;
            X8_8(gtid, gid, index, color);
            return;
        }
    case 3:
        {
            uint2 base = gid * 8;
            uint2 pos = base + gtid;
            RWTexture2D<float4> src = images[0]; // 8x8
            color = src.Load(pos);
            X8_8(gtid, gid, index, color);
            return;
        }
    case 2:
        {
            if (any(gtid >= 4)) return;
            uint2 base = gid * 4;
            uint2 pos = base + gtid;
            RWTexture2D<float4> src = images[0]; // 4x4
            color = src.Load(pos);
            X4_4(gtid, gid, index, color);
            return;
        }
    case 1:
        {
            if (any(gtid >= 2)) return;
            uint2 base = gid * 2;
            uint2 pos = base + gtid;
            RWTexture2D<float4> src = images[0]; // 2x2
            color = src.Load(pos);
            X2_2(gid, index, color);
        }
    default:
        return;
    }
}

bool QuadIsFirst()
{
    return WaveGetLaneIndex() % 4 == 0;
}

void X8_8(uint2 gtid, uint2 gid, inout uint index, inout float4 color)
{
    RWTexture2D<float4> dst = images[index]; // 4x4
    // color from 8x8
    float4 c1 = QuadReadAcrossX(color);
    float4 c2 = QuadReadAcrossY(color);
    float4 c3 = QuadReadAcrossDiagonal(color);
    float4 dst_color = (color + c1 + c2 + c3) * 0.25;
    bool can_wave = WaveGetLaneCount() >= 64;
    if (QuadIsFirst())
    {
        uint2 base = gid * 4;
        uint2 pos = gtid / 2;
        dst[base + pos] = dst_color;
        if (!can_wave) StoreColor(pos, dst_color);
    }
    if (!can_wave)
    {
        GroupMemoryBarrierWithGroupSync();
        if (any(gtid >= 4)) return;
        color = LoadColor(gtid);
    }
    else
    {
        if (all(gtid < 4))
        {
            color = WaveReadLaneAt(dst_color, WaveGetLaneIndex() * 4);
        }
        if (any(gtid >= 4)) return;
    }
    index++;
    X4_4(gtid, gid, index, color);
}

void X4_4(uint2 gtid, uint2 gid, inout uint index, inout float4 color)
{
    RWTexture2D<float4> dst = images[index]; // 2x2
    // color from 4x4
    float4 c1 = QuadReadAcrossX(color);
    float4 c2 = QuadReadAcrossY(color);
    float4 c3 = QuadReadAcrossDiagonal(color);
    float4 dst_color = (color + c1 + c2 + c3) * 0.25;
    bool can_wave = WaveGetLaneCount() >= 32;
    if (QuadIsFirst())
    {
        uint2 base = gid * 2;
        uint2 pos = gtid / 2;
        dst[base + pos] = dst_color;
        if (!can_wave) StoreColor(pos, dst_color);
    }
    if (!can_wave)
    {
        GroupMemoryBarrierWithGroupSync();
        if (any(gtid >= 2)) return;
        color = LoadColor(gtid);
    }
    else
    {
        if (all(gtid < 2))
        {
            color = WaveReadLaneAt(dst_color, WaveGetLaneIndex() * 2);
        }
        if (any(gtid >= 2)) return;
    }
    index++;
    X2_2(gid, index, color);
}

void X2_2(uint2 gid, inout uint index, inout float4 color)
{
    RWTexture2D<float4> dst = images[index]; // 1x1
    // color from 2x2
    float4 c1 = QuadReadAcrossX(color);
    float4 c2 = QuadReadAcrossY(color);
    float4 c3 = QuadReadAcrossDiagonal(color);
    float4 dst_color = (color + c1 + c2 + c3) * 0.25;
    if (QuadIsFirst())
    {
        dst[gid] = dst_color;
    }
}
