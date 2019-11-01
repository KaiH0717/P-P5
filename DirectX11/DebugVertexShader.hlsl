#pragma pack_matrix(row_major)

struct InputVertex
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
};

struct OutputVertex
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float4 worldPosition : WORLDPOSITION;
};

cbuffer Matrix_Constant_Buffer : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4 time; // x = delta, y = smooth delta, z = elapsed time
};

OutputVertex main(InputVertex input)
{
    OutputVertex output = (OutputVertex) 0;
    output.position = input.position;
    output.position.w = 1.0f;
    output.normal = input.normal;
    output.color = input.color;

    output.position = mul(output.position, world);
    output.normal = mul(float4(output.normal, 0.0f), world).xyz;
    output.worldPosition = output.position;
    output.position = mul(output.position, viewProjection);
	
    return output;
}