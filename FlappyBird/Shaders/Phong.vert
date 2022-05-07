#version 410

// Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attribute（頂点座標、法線、UV）
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// フラグメントシェーダーに渡すUV
out vec2 fragTexCoord;
// フラグメントシェーダーに渡す法線ベクトル
out vec3 fragNormal;
// フラグメントシェーダーに渡す頂点座標
out vec3 fragWorldPos;

void main()
{
	// コンバート
	vec4 pos = vec4(inPosition, 1.0);
	// 頂点座標のワールド変換
	pos = pos * uWorldTransform;
	// フラグメントシェーダーに渡すワールド座標
	fragWorldPos = pos.xyz;
	// 投影座標
	gl_Position = pos * uViewProj;

	// 法線ベクトルのワールド変換
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// UV座標
	fragTexCoord = inTexCoord;
}
