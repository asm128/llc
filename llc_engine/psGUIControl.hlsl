struct Input {
	float4 position : SV_POSITION;
	nointerpolation float4 client : COLOR0;
	nointerpolation float4 left : COLOR1;
	nointerpolation float4 top : COLOR2;
	nointerpolation float4 right : COLOR3;
	nointerpolation float4 bottom : COLOR4;
	nointerpolation uint flags : TEXCOORD0;
};

float4 main(Input input, uint primitive : SV_PrimitiveID) : SV_TARGET {
	static const uint areas[18] = {
		2, 1, 2, 2, 2, 3,
		1, 1, 0, 0, 3, 3,
		1, 4, 4, 4, 3, 4,
	};
	const uint area = areas[primitive];
	if((area == 0 && !(input.flags & 1)) || (area != 0 && !(input.flags & 2)))
		discard;
	if(area == 0) return input.client;
	if(area == 1) return input.left;
	if(area == 2) return input.top;
	if(area == 3) return input.right;
	return input.bottom;
}
