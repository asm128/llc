Texture2D texDiffuse : register(t0);
SamplerState samplerPoint : register(s0);
struct Input {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR0;
	nointerpolation uint mode : TEXCOORD1;
};
float4 main(Input input) : SV_TARGET {
	if(input.mode == 0) return input.color;
	float4 sampled = texDiffuse.Sample(samplerPoint, input.uv);
	if(input.mode == 1) { clip(sampled.a - .5f); return input.color; }
	return sampled;
}
