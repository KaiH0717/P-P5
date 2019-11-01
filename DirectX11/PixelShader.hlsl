Texture2D tx_diffuse : register(t0);
Texture2D tx_emissive : register(t1);
Texture2D tx_specular : register(t2);
SamplerState samLinear : register(s0);

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

cbuffer Material_Constant_Buffer : register(b0)
{
    // x, y, z = r, g, b
    // w = factor
    float4 diffuse;
    float4 emissive;
    float4 specular;
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
static const float4 ambientLight = float4(1.0f, 1.0f, 1.0f, 0.0f);
float4 main(OutputVertex inputPixel) : SV_TARGET
{
    float3 pointLightDir = lightPos[1].xyz - inputPixel.worldPosition.xyz;
    float squaredDistance = dot(pointLightDir, pointLightDir);
    pointLightDir = pointLightDir / sqrt(squaredDistance);
    float3 viewDir = inputPixel.viewDirection;
    float squaredDistanceView = dot(viewDir, viewDir);
    viewDir = viewDir / sqrt(squaredDistanceView);
    float diffuseIntensity = saturate(dot(inputPixel.normal, pointLightDir));
    float3 halfVector = normalize(pointLightDir + viewDir);
    float specularIntensity = pow(saturate(dot(inputPixel.normal, halfVector)), specular.w);
    float lightIntensity = float4(lightColor[1].xyz, 0.0f) * 2 / squaredDistance;

    // point lighting
    float pointLightRatio = saturate(dot(pointLightDir, inputPixel.normal));
    float attenuation = 1.0f - saturate((length(lightPos[1] - inputPixel.worldPosition) / lightRadius.x));
    pointLightRatio = attenuation * attenuation * pointLightRatio;
    float4 pointLightPixelColor = lerp(float4(0.0f, 0.0f, 0.0f, 1.0f), lightColor[1], pointLightRatio);

    float4 mat_diffuse = tx_diffuse.Sample(samLinear, inputPixel.tex) * float4(diffuse.xyz, 0.0f) * diffuse.w;
    float4 mat_specular = tx_specular.Sample(samLinear, inputPixel.tex) * float4(specular.xyz, 0.0f) * specular.w;
    float4 mat_emissive = tx_emissive.Sample(samLinear, inputPixel.tex) * float4(emissive.xyz, 0.0f) * emissive.w;
    float4 emissive = mat_emissive;
    float4 ambient = mat_diffuse * ambientLight;
    float4 specular = mat_specular * specularIntensity * lightIntensity;
    float4 diffuse = mat_diffuse * diffuseIntensity * lightIntensity;
    diffuse.xyz -= specular.xyz;
    diffuse.xyz = saturate(diffuse.xyz);
    float4 color = ambient + specular + diffuse + emissive + pointLightPixelColor;
    return color;
}