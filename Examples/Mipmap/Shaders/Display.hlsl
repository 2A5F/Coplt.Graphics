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
    float4x4 Mesh;
    float4x4 View;
    float4x4 Proj;
}

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = GetQuadVertexPosition(output.Uv, input.VertexID);
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
