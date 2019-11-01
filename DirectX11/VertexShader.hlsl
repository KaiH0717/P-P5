#pragma pack_matrix(row_major)

struct InputVertex
{
    float4 position : POSITION;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    uint4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHT;
};

struct OutputVertex
{
    float4 position : SV_POSITION;
    float4 tangent : TANGENT;
    float4 binormal : BINORMAL;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    float4 worldPosition : WORLDPOSITION;
    float3 viewDirection : VIEWDIRECTION;
};

cbuffer Matrix_Constant_Buffer : register(b0)
{
    float4x4 world;
    float4x4 viewProjection;
    float4 time; // x = delta, y = smooth delta, z = elapsed time
    float4 cameraPosition;
};

cbuffer Joint_Matrix_Constant_Buffer : register(b1)
{
    float4x4 worldMatrices[32];
};

OutputVertex main(InputVertex input)
{
    OutputVertex output = (OutputVertex) 0;
    float4 skinnedPosition = { 0.0f, 0.0f, 0.0f, 0.0f };
    float4 skinnedNorm = { 0.0f, 0.0f, 0.0f, 0.0f };
    [unroll]
    for (int j = 0; j < 4; ++j)
    {
        skinnedPosition += mul(float4(input.position.xyz, 1.0f), worldMatrices[input.indices[j]]) * input.weights[j];
        skinnedNorm += mul(float4(input.normal.xyz, 0.0f), worldMatrices[input.indices[j]]) * input.weights[j];
    }
    output.position = skinnedPosition;
    output.normal = normalize(skinnedNorm).xyz;
    output.tangent = mul(input.tangent, world);
    output.binormal = mul(input.binormal, world);
    //output.normal = mul(float4(input.normal, 0), world).xyz;
    //output.normal = normalize(output.normal);
    output.tex = input.tex;

    //output.position = mul(output.position, world);
    output.worldPosition = output.position;
    output.position = mul(output.position, viewProjection);
    output.viewDirection = (cameraPosition - output.worldPosition).xyz;
    return output;
}