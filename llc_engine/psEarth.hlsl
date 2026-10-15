#include "PixelShaderShared.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET {
	const float3 lightVector = normalize(LightPosition.xyz - input.world.position);
	const float lightFactor = dot(normalize(input.world.normal), lightVector);
	const float4 dayTexture = texDiffuse.Sample(samplerLinear, input.world.uv);
	const float4 nightTexture = texAuxiliary.Sample(samplerLinear, input.world.uv);
	const float4 dayColor = dayTexture * max(lightFactor, 0.0f);
	float4 finalColor = lerp(dayColor, nightTexture, max(-lightFactor, 0.0f));
	finalColor.a = Diffuse.a;
	return saturate(finalColor);
}
