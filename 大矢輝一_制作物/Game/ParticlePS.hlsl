#include "Particle.hlsli"
Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
SamplerState samLinear : register(s0);

//歪みの偏差を求める処理
float distortion(float2 center, float r, float d_r, float2 uv)
{
	//今のピクセルと中心座標の距離
    float dist = distance(center, uv);
	//半径d_rの歪みを作る。
    return 1 - smoothstep(r, d_r, dist);
}


float4 main(PS_INPUT input) : SV_TARGET
{
	//float4 red = float4(1.0f,0.0f,0.0f,1.0f);

	//float4 blue = float4(0.0f,0.0f,1.0f,1.0f);

	////画像表示
	//float4 tom = tex.Sample(samLinear, input.Tex);
	////画像表示
	//float4 ball = tex2.Sample(samLinear, input.Tex);
	////真っ白な板ポリゴン
	//float4 outputw = float4(1, 0, 0, 1);

	////tom.rgb = diffuse.rgb;
	////シャドウ
	///*float4 shadow = tex2.Sample(samLinear, input.Tex);

	//shadow.rgb = diffuse.rgb;*/
	//return tom;
	
    float2 center2 = float2(Time.z, Time.w);
    float d2 = distortion(center2, abs(sin(Time.x / 2)) /* - 0.7f*/, abs(sin(Time.x / 2)) /* - 0.6f*/, input.Tex);
    float2 uv2 = lerp(input.Tex, center2, d2);
    float4 r2 = tex.Sample(samLinear, uv2);
    
   
    float4 base = float4(0, 0, 0, 0);
    
    if (color.x == true)
    {
   //     return lerp(lerp(r, base, step(1, d)),
			//lerp(r2, base, step(1, d2)),
			//step(d, d2)
			//);
        return lerp(base, base, step(1, d2));
			
    }
	
   // return lerp(lerp(r, tex2.Sample(samLinear, input.Tex), step(1, d)),
			//lerp(r2, tex2.Sample(samLinear, input.Tex), step(1, d2)),
			//step(d, d2)
			//);
    return lerp(base, tex2.Sample(samLinear, input.Tex), step(1, d2));
}