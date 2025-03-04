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

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = GetQuadVertexPosition(output.Uv, input.VertexID);
    return output;
}

cbuffer Args : register(b0)
{
    float time;
} 

[shader("pixel")]
float4 Pixel(Varying input) : SV_Target
{
    return float4(input.Uv, time, 1);
}
