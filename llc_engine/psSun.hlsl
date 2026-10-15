#include "PixelShaderShared.hlsli"

float4 main(PixelShaderInput input) : SV_TARGET {
	return texDiffuse.Sample(samplerLinear, input.world.uv);
}
