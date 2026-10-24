Texture2D image : register(t0);
SamplerState pointSampler : register(s0);

float4 main(float4 position : SV_POSITION, float2 uv : TEXCOORD0, nointerpolation float4 color : COLOR0, nointerpolation uint mode : TEXCOORD1) : SV_TARGET {
	const float4 texel = image.Sample(pointSampler, uv);
	if(texel.a == 0)
		discard;
	if(mode == 2)
		return float4(floor(texel.rgb * texel.a * 255) / 255, texel.a);
	return color;
}
