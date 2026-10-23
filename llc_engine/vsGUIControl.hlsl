cbuffer GUIViewport : register(b0) { float4 scale; };

struct Input {
	float2 position : POSITION;
	float2 uv : TEXCOORD0;
	float4 color : COLOR;
	uint mode : TEXCOORD1;
	float2 a : TEXCOORD2;
	float2 b : TEXCOORD3;
	float2 c : TEXCOORD4;
};
struct Output {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float2 pixel : TEXCOORD1;
	nointerpolation float4 color : COLOR;
	nointerpolation uint mode : TEXCOORD2;
	nointerpolation float2 a : TEXCOORD3;
	nointerpolation float2 b : TEXCOORD4;
	nointerpolation float2 c : TEXCOORD5;
};
Output main(Input input) {
	Output output;
	output.position = float4(input.position * scale.xy + scale.zw, 0, 1);
	output.pixel = input.position;
	output.uv = input.uv;
	output.color = input.color.bgra;
	output.mode = input.mode;
	output.a = input.a; output.b = input.b; output.c = input.c;
	return output;
}
