cbuffer GUIViewport : register(b0) { float4 scale; };

struct Input {
	uint2 grid : POSITION;
	int4 x : TEXCOORD0;
	int4 y : TEXCOORD1;
	float4 client : COLOR0;
	float4 left : COLOR1;
	float4 top : COLOR2;
	float4 right : COLOR3;
	float4 bottom : COLOR4;
	uint flags : TEXCOORD2;
};

struct Output {
	float4 position : SV_POSITION;
	nointerpolation float4 client : COLOR0;
	nointerpolation float4 left : COLOR1;
	nointerpolation float4 top : COLOR2;
	nointerpolation float4 right : COLOR3;
	nointerpolation float4 bottom : COLOR4;
	nointerpolation uint flags : TEXCOORD0;
	nointerpolation int4 x : TEXCOORD1;
	nointerpolation int4 y : TEXCOORD2;
};

Output main(Input input) {
	Output output;
	const float2 pixel = float2(input.x[input.grid.x], input.y[input.grid.y]);
	output.position = float4(pixel * scale.xy + scale.zw, 0, 1);
	output.client = input.client.bgra;
	output.left = input.left.bgra;
	output.top = input.top.bgra;
	output.right = input.right.bgra;
	output.bottom = input.bottom.bgra;
	output.flags = input.flags;
	output.x = input.x;
	output.y = input.y;
	return output;
}
