struct OutputVertex
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float4 worldPosition : WORLDPOSITION;
};

float4 main(OutputVertex inputPixel) : SV_TARGET
{
    return inputPixel.color;
}