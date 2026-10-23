Texture2D image : register(t0);
SamplerState pointSampler : register(s0);

float edge(float2 a, float2 b, float2 p) { return (b.x - a.x) * (p.y - a.y) - (b.y - a.y) * (p.x - a.x); }
float4 main(float4 position : SV_POSITION, float2 uv : TEXCOORD0, float2 pixel : TEXCOORD1,
	nointerpolation float4 color : COLOR, nointerpolation uint mode : TEXCOORD2,
	nointerpolation float2 a : TEXCOORD3, nointerpolation float2 b : TEXCOORD4, nointerpolation float2 c : TEXCOORD5) : SV_TARGET {
	if(mode == 3) {
		float2 p = floor(pixel);
		if(edge(b, a, p) < 0 || edge(c, b, p) < 0 || edge(a, c, p) < 0) discard;
	}
	if(mode == 1 || mode == 2) {
		float4 texel = image.Sample(pointSampler, uv);
		if(texel.a == 0) discard;
		if(mode == 2) return float4(floor(texel.rgb * texel.a * 255) / 255, texel.a);
	}
	return color;
}
