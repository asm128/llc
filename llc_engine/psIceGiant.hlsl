#include "PixelShaderShared.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET {
	const float3 normal = normalize(input.world.normal);
	const float3 lightDelta = LightPosition.xyz - input.world.position;
	const float3 lightVector = normalize(lightDelta);
	const float diffuseFactor = max(dot(lightVector, normal), 0.0f);
	const float4 textureColor = texDiffuse.Sample(samplerLinear, input.world.uv);
	const float3 ambient = Ambient.rgb * float3(.05f, .05f, .07f);
	const float3 diffuse = Diffuse.rgb * diffuseFactor;
	return float4(saturate((ambient + diffuse) * textureColor.rgb), Diffuse.a);
}
