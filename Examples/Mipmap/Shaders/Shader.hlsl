cbuffer Args : register(b0)
{
    float2 image_size;
    uint mip_levels;
}

RWTexture2D<float> images[] : register(u0);

[shader("compute")]
[numthreads(32, 32, 1)]
void Compute(uint2 gtid : SV_GroupThreadID, uint2 gid: SV_GroupID)
{
}
