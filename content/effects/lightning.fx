uniform extern float4x4 gWVP;
uniform extern texture gBackTex;
uniform extern texture gTex;
float4 min_color = float4(0.8,0.8,0.8,1.0);
float4 ambient_color = float4(0.4,0.4,0.4,1.0);

sampler TexS = sampler_state {
	Texture = <gTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

sampler BackTexS = sampler_state {
	Texture = <gBackTex>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct OutputVS {
	float4 posH   : POSITION0;
	float2 tex0   : TEXCOORD0;
	float4 color0 : COLOR0;
};

OutputVS BasicVS(float3 posL : POSITION0, float2 tex0 : TEXCOORD0, float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;
	outVS.posH = mul(float4(posL, 1.0f), gWVP);
	outVS.tex0 = tex0;
	outVS.color0 = color;
	return outVS;
}

float4 BasicPS(OutputVS input) : COLOR{
	float4 clr = tex2D(TexS, input.tex0);
	//clr = clamp(clr,min_color,float4(1,1,1,1));
	float4 bg = tex2D(BackTexS, input.tex0);
	return bg + ambient_color * clr;
}

technique LTTech {
	pass P0 {
		vertexShader = compile vs_2_0 BasicVS();
		pixelShader = compile ps_2_0 BasicPS();
	}
};