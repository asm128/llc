#ifndef LLC_SHADER_LIGHTS
#define LLC_SHADER_LIGHTS
struct SRenderLight {
	float4 Diffuse;
	float4 Ambient;
	float4 Specular;
	float3 Position;
	float Range;
	float3 Direction;
	float SpotPower;
	uint Type;
	uint3 Padding;
};
StructuredBuffer<SRenderLight> EngineLights : register(t2);
cbuffer SRenderLightConstants : register(b2) {
	uint EngineLightCount;
	uint3 EngineLightPadding;
};
#endif
