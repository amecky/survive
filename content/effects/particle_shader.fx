uniform extern float4x4 gWVP;
uniform extern texture gTex;
uniform extern float2 viewportPosition = float2(512,384);

float VP_ARRAY[8] = {-0.5f, 0.5f, 0.5f, 0.5f,0.5f, -0.5f, -0.5f, -0.5f};

sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

struct OutputVS {
	float4 posH   : POSITION0;
	float2 tex0   : TEXCOORD0;
	float4 color0 : COLOR0;
};

OutputVS BasicVS(float3 posL : POSITION0,float2 tex0 : TEXCOORD0,float2 scale : TEXCOORD1,float2 dim : TEXCOORD2,float2 rotindex : TEXCOORD3 , float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;	
	//outVS.posH = mul(float4(posL, 1.0f), gWVP);		
	outVS.tex0 = tex0;
	outVS.color0 = color;

	float3 pos = posL;
	pos -= float3(viewportPosition,0.0f);
	float px = VP_ARRAY[rotindex.y * 2] * dim.x;
	float py = VP_ARRAY[rotindex.y * 2 + 1] * dim.y;

	float sx = px * scale.x;
	float sy = py * scale.y;

	float xt = cos(rotindex.x) * sx - sin(rotindex.x) * sy;
	float yt = sin(rotindex.x) * sx + cos(rotindex.x) * sy;

	xt += pos.x;
	yt += pos.y;

	outVS.posH = mul(float4(xt,yt,0.0f, 1.0f), gWVP);	
	return outVS;
}

float4 BasicPS(OutputVS input) : COLOR {
	float4 clr = tex2D(TexS, input.tex0);
	return clr * input.color0;
}

technique ParticleTech {
	pass P0 {
		vertexShader = compile vs_2_0 BasicVS();
		pixelShader  = compile ps_2_0 BasicPS();
	}
};