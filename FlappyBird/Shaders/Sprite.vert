#version 410

// Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Attributes 頂点座標、法線ベクトル、UV
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// フラグメントシェーダーに送るUV
out vec2 fragTexCoord;

void main()
{
	// コンバート
	vec4 pos = vec4(inPosition, 1.0);
	// グローバル座標に変換
	gl_Position = pos * uWorldTransform * uViewProj;

	// UVをフラグメントシェーダーに送る
	fragTexCoord = inTexCoord;
}
