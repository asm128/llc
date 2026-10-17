#include "PixelShaderShared.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET {
	const float4 cloudColor = texDiffuse.Sample(samplerLinear, input.world.uv);
	const float4 cloudMask = texAuxiliary.Sample(samplerLinear, input.world.uv);
	const float3 lightDelta = LightPosition.xyz - input.world.position;
	const float lightDistance = length(lightDelta);
	const float diffuseFactor = max(0.0f, dot(normalize(input.world.normal), normalize(lightDelta)));
	const float attenuation = 1.0f + .001f * lightDistance + .001f * lightDistance * lightDistance;
	float4 finalColor = cloudColor * (float4(.05f, .05f, .07f, 1.0f) + diffuseFactor / attenuation);
	finalColor.a = (1.0f - cloudMask.r) * Diffuse.a;
	return saturate(finalColor);
}
