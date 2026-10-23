#include "../llc_engine/ShaderLights.hlsli"

float4 main(float4 position : SV_POSITION) : SV_TARGET {
	float3 color = 0;
	for(uint i = 0; i < EngineLightCount; ++i)
		color += EngineLights[i].Diffuse.rgb;
	return float4(color * (1 - position.x / 32), 1);
}
