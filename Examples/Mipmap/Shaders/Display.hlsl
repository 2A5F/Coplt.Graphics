struct Attribute
{
    uint VertexID : SV_VertexID;
};

struct Varying
{
    float4 PositionCS : SV_Position;
    float2 Uv : UV;
};

float4 GetQuadVertexPosition(out float2 uv, uint vid, float z = 1)
{
    float2 uv_ = float2((vid << 1) & 2, vid & 2);
    uv = uv_ * 0.5f;
    return float4(uv_ * float2(1, -1) + float2(-1, 1), z, 1);
}

cbuffer Args : register(b0)
{
    float4x4 ViewProj;
}

float4 ApplyVP(float3 positionWS)
{
    return mul(ViewProj, float4(positionWS, 1.0));
}

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    float3 pos = GetQuadVertexPosition(output.Uv, input.VertexID, 0).xyz;
    output.PositionCS = ApplyVP(pos);
    return output;
}

Texture2D<float3> Texture : register(t0);
SamplerState Sampler : register(s0);

[shader("pixel")]
float4 Pixel(Varying input) : SV_Target
{
    float3 color = Texture.Sample(Sampler, input.Uv);
    return float4(color, 1);
}
