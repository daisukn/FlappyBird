#version 410

// Matrixと、ビューマトリックス
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// 頂点バッファ（座標、法線、UV）
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// フラグメントシェーダーに渡すUV
out vec2 fragTexCoord;

void main()
{
	// 頂点座標変換
	vec4 pos = vec4(inPosition, 1.0);
	// 投影座標に変換
	gl_Position = pos * uWorldTransform * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに送る
	fragTexCoord = inTexCoord;
}
