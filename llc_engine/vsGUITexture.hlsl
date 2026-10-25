cbuffer GUIViewport : register(b0) { float4 scale; };

struct Input {
	float2 corner : POSITION;
	int4 rectangle : TEXCOORD0;
	float4 uvRectangle : TEXCOORD1;
	float4 color : COLOR0;
	uint mode : TEXCOORD2;
};

struct Output {
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	nointerpolation float4 color : COLOR0;
	nointerpolation uint mode : TEXCOORD1;
};

Output main(Input input) {
	Output output;
	const float2 pixel = input.rectangle.xy + input.corner * input.rectangle.zw;
	output.position = float4(pixel * scale.xy + scale.zw, 0, 1);
	output.uv = input.uvRectangle.xy + input.corner * input.uvRectangle.zw;
	output.color = input.color.bgra;
	output.mode = input.mode;
	return output;
}
