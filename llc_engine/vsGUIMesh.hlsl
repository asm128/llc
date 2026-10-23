#include "ShaderShared.hlsli"
struct Input {
	float3 position : POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	uint mode : TEXCOORD1;
};
struct Output {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	nointerpolation uint mode : TEXCOORD1;
	float4 clipBounds : SV_ClipDistance;
};
Output main(Input input) {
	Output output;
	output.position = mul(float4(input.position, 1), MVP);
	output.uv = input.uv;
	output.color = input.color.bgra;
	output.mode = input.mode;
	output.clipBounds = float4(input.position.xy, NodeSize.xy - input.position.xy);
	return output;
}
