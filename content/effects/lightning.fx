uniform extern float4x4 gWVP;
uniform extern texture gTex;
float sx = 1600.0;
float sy = 900.0;

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
	float2 tex1   : TEXCOORD1;
	float4 color0 : COLOR0;
};

OutputVS BasicVS(float3 posL : POSITION0,float2 tex0 : TEXCOORD0 , float4 color : COLOR0) {
	OutputVS outVS = (OutputVS)0;	
	outVS.posH = mul(float4(posL, 1.0f), gWVP);		
	outVS.tex1.x = 0.5 + (posL.x + sx / 2.0) / (2.0 * sx) * 0.78125;
	outVS.tex1.y = (sy / 2.0 - posL.y ) / sy * 0.21972;
	outVS.tex0 = tex0;
	outVS.color0 = color;
	return outVS;
}

float4 BasicPS(OutputVS input) : COLOR {
	float4 clr = tex2D(TexS, input.tex0);
	float4 bg = tex2D(TexS,input.tex1);
	//return clr * input.color0;// * bg;	
	return bg;
}

technique LTTech {
	pass P0 {
		vertexShader = compile vs_2_0 BasicVS();
		pixelShader  = compile ps_2_0 BasicPS();
	}
};