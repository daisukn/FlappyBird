#version 410

// 頂点シェーダーからのデータ
// テクスチャ座標
in vec2 fragTexCoord;
// 法線ベクトル
in vec3 fragNormal;
// 座標
in vec3 fragWorldPos;

// アウトプットする色
out vec4 outColor;

// テクスチャ
uniform sampler2D uTexture;

// ディレクショナルライト
struct DirectionalLight
{
	// 方向
	vec3 mDirection;
	// 拡散光色
	vec3 mDiffuseColor;
	// 反射光色
	vec3 mSpecColor;
};

// ライティングのUniforms
// カメラ位置
uniform vec3 uCameraPos;
// 反射光
uniform float uSpecPower;
// 環境光
uniform vec3 uAmbientLight;

// ディレクショナルライト
uniform DirectionalLight uDirLight;


struct FogInfo {
    float maxDist;
    float minDist;
    vec3 color;
};
// フォグ情報
uniform FogInfo uFoginfo;

void main()
{
	// 法線ベクトル
	vec3 N = normalize(fragNormal);
	// ライティング計算
	vec3 L = normalize(-uDirLight.mDirection);
	// 面からカメラのベクトル
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// 反射角度
	vec3 R = normalize(reflect(-L, N));

    // フォグの値を計算
    float dist = length(uCameraPos.xyz - fragWorldPos.xyz);
    float fogFactor = (uFoginfo.maxDist - dist) / (uFoginfo.maxDist - uFoginfo.minDist);
    fogFactor = clamp(fogFactor, 0.0f, 1.0f);
    
    
	// 反射光を計算
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0)
	{
		vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
		vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
		Phong += Diffuse + Specular;
	}

	// テクスチャから色を計算
    vec4 col = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
    
    // フォグを適用
    vec3 c = mix(uFoginfo.color, col.xyz, fogFactor);
    outColor = vec4(c, 1.0f);
    
    
    
}
