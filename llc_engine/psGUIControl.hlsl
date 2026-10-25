struct Input {
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

uint cpuCornerArea(Input input, uint primitive) {
	const int2 pixel = int2(input.position.xy);
	if(primitive <= 1) {
		const int2 offset = pixel - int2(input.x[0], input.y[0]);
		return offset.y * (input.x[1] - input.x[0]) >= offset.x * (input.y[1] - input.y[0]) ? 1 : 2;
	}
	if(primitive >= 4 && primitive <= 5) {
		const int2 offset = pixel - int2(input.x[2], input.y[0]);
		const int width = input.x[3] - input.x[2], height = input.y[1] - input.y[0];
		return offset.y * width + offset.x * height < width * height ? 2 : 3;
	}
	if(primitive >= 12 && primitive <= 13) {
		const int2 offset = pixel - int2(input.x[0], input.y[2]);
		const int width = input.x[1] - input.x[0], height = input.y[3] - input.y[2];
		return offset.y * width + offset.x * height <= width * height ? 1 : 4;
	}
	if(primitive >= 16) {
		const int2 offset = pixel - int2(input.x[2], input.y[2]);
		return offset.y * (input.x[3] - input.x[2]) < offset.x * (input.y[3] - input.y[2]) ? 3 : 4;
	}
	return primitive >= 8 && primitive <= 9 ? 0 : primitive < 6 ? 2 : primitive < 12 ? (primitive < 8 ? 1 : 3) : 4;
}

float4 main(Input input, uint primitive : SV_PrimitiveID) : SV_TARGET {
	const uint area = cpuCornerArea(input, primitive);
	if((area == 0 && !(input.flags & 1)) || (area != 0 && !(input.flags & 2)))
		discard;
	if(area == 0) return input.client;
	if(area == 1) return input.left;
	if(area == 2) return input.top;
	if(area == 3) return input.right;
	return input.bottom;
}
