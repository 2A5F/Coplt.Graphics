struct Attribute
{
    uint VertexID : SV_VertexID;
};

struct Varying
{
    float4 PositionCS : SV_Position;
    float4 Color : Color;
    float2 Uv : Uv;
};

static const float3 Vertexs[3] = {
    float3(0.0, 0.5 * 1.7, 0),
    float3(0.5, -0.5 * 1.7, 0),
    float3(-0.5, -0.5 * 1.7, 0),
};
static const float2 Uvs[3] = {
    float2(0.0, 0.0),
    float2(-1.0, 2.0),
    float2(1.0, 2.0),
};
static const float4 Colors[3] = {
    float4(0.71, 0.22, 0.21, 1),
    float4(0.34, 0.64, 0.31, 1),
    float4(0.36, 0.42, 0.92, 1),
};

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = float4(Vertexs[input.VertexID % 3], 1);
    output.Color = Colors[input.VertexID % 3];
    output.Uv = Uvs[input.VertexID % 3];
    return output;
}

float dot2(in float2 v) { return dot(v, v); }

float SdTriangle(float2 p, float2 p0, float2 p1, float2 p2)
{
    float2 e0 = p1 - p0, v0 = p - p0;
    float d0 = dot2(v0 - e0 * clamp(dot(v0, e0) / dot(e0, e0), 0.0, 1.0));
    float2 e1 = p2 - p1, v1 = p - p1;
    float d1 = dot2(v1 - e1 * clamp(dot(v1, e1) / dot(e1, e1), 0.0, 1.0));
    float2 e2 = p0 - p2, v2 = p - p2;
    float d2 = dot2(v2 - e2 * clamp(dot(v2, e2) / dot(e2, e2), 0.0, 1.0));

    float o = e0.x * e2.y - e0.y * e2.x;
    float2 d = min(
        min(
            float2(d0, o * (v0.x * e0.y - v0.y * e0.x)),
            float2(d1, o * (v1.x * e1.y - v1.y * e1.x))
        ),
        float2(d2, o * (v2.x * e2.y - v2.y * e2.x))
    );
    return -sqrt(d.x) * sign(d.y);
}

[shader("pixel")]
float4 Pixel(Varying input) : SV_Target
{
    float d = SdTriangle(input.Uv, float2(0.0f, 0.5f), float2(-0.5f, 1.5f), float2(0.5f, 1.5f));
    float aa = sqrt(2.71) / 512;
    return float4(input.Color.rgb * 1.2, lerp(0, 1, smoothstep(aa, -aa, d)));
}
