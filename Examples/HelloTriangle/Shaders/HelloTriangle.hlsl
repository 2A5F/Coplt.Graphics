struct Attribute
{
    float4 PositionCS : POSITION;
    float4 Color : COLOR;
};

struct Varying
{
    float4 PositionCS : SV_POSITION;
    float4 Color : COLOR;
};

[shader("vertex")]
Varying Vertex(Attribute input)
{
    Varying output;
    output.PositionCS = input.PositionCS;
    output.Color = input.Color;
    return output;
}

[shader("pixel")]
float4 Pixel(Varying input)
{
    return input.Color;
}
