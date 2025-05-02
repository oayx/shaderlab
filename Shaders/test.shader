Shader "Custom/Cube/ShowCube"
{
	Properties
	{
		_CubeMap ("CubeMap", Cube) = "white" {}
	}
	SubShader
	{
		Tags { "RenderType"="Opaque" }
		LOD 100
		UsePass "Shader object name/PASS NAME IN UPPERCASE"
		GrabPass {  }
		
		Pass
		{
			Name "TestShader"
			Tags { "LightMode"="ForwardBase" }
			AlphaToMask Off
			Blend 1 One Zero, Zero One
			BlendOp Sub
			ColorMask RGB 2
			Cull Back
			Offset 0.5, 1
			Conservative True
			Cull Back
			ZClip False
			ZTest Equal
			ZWrite Off

			Stencil
			{
				Ref 2
				Comp equal
				Pass keep
				ZFail decrWrap
			}

			// legacy
			Color (1,0,0,0)
			Material {
                Diffuse [_Color]
                Ambient [_Color]
                Shininess [_Shininess]
                Specular [_SpecColor]
                Emission [_Emission]
            }
            Lighting On
            SeparateSpecular On
            SetTexture [_MainTex] {
                Combine texture * primary DOUBLE, texture * primary
            }
			
			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			#include "UnityCG.cginc"

			uniform float4 _OutlineColor;
			uniform float _OutlineWidth;

			struct v2f
			{	
				float3 normal : TEXCOORD0;
				float3 worldPos : TEXCOORD1;
				float4 vertex : SV_POSITION;
			};

			v2f vert(appdata_base v) 
			{
				v2f o;
				v.vertex.xyz += v.normal * (_OutlineWidth * 0.1);    //进行法线方向的扩展
				o.vertex = UnityObjectToClipPos(v.vertex);
				return o;
			}

			fixed4 frag(v2f i) : COLOR
			{
				return fixed4(_OutlineColor.rgb,0);
			}
			ENDCG
		}
	}
	//Fallback Off
	Fallback "built-in"
	CustomEditor "[custom editor class name]"
}
