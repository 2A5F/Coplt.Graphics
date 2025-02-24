struct Attribute
{
    float4 PositionCS : Position;
};

struct Varying
{
    float4 PositionCS : SV_Position;
    // float4 Color : Color;
    // float2 Uv : Uv;
};

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = input.PositionCS;
    // output.Color = Colors[input.VertexID % 3];
    // output.Uv = Uvs[input.VertexID % 3];
    return output;
}

[shader("pixel")]
float4 Pixel(Varying input) : SV_Target
{
    return float4(1, 1, 1, 1);
}
