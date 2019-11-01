struct OutputVertex
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float4 worldPosition : WORLDPOSITION;
};

cbuffer Light_Constant_Buffer : register(b1)
{
    // [0] = directional lighting, [1] = point lighting, [2] = spot lighting
    float4 lightPos[3];
    float4 lightNormal[3];
    float4 lightColor[3];
	// x = radius, y = rotation, z = time
    float4 lightRadius;
};

static const float4 ambient = float4(0.25f, 0.25f, 0.25f, 0.25f);
float4 main(OutputVertex inputPixel) : SV_TARGET
{
    // directional lighting
    float ratio = saturate(dot((float3) -normalize(lightNormal[0]), normalize(inputPixel.normal)));
    float4 color1 = lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), lightColor[0], ratio);
    // point lighting
    float3 pointLightDir = (float3) normalize(lightPos[1] - inputPixel.worldPosition);
    float pointLightRatio = saturate(dot(pointLightDir, normalize(inputPixel.normal)));
    float attenuation = 1.0f - saturate((length(lightPos[1] - inputPixel.worldPosition) / lightRadius.x));
    pointLightRatio = attenuation * attenuation * pointLightRatio;
    float4 color2 = lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), lightColor[1], pointLightRatio);

    inputPixel.color = inputPixel.color * (color1 + color2 + ambient);
    return inputPixel.color;
}