struct Attribute
{
    float4 PositionCS : Position;
    float4 Color : Color;
    float2 Uv : Uv;
};

struct Varying
{
    float4 PositionCS : SV_Position;
    float4 Color : Color;
    float2 Uv : Uv;
};

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = input.PositionCS;
    output.Color = input.Color;
    output.Uv = input.Uv;
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
