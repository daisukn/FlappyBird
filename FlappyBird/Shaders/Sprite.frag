#version 410

// 頂点シェーダーから受け取るUV座標
in vec2 fragTexCoord;

// 出力する色
out vec4 outColor;

// Uniform テクスチャ
uniform sampler2D uTexture;

void main()
{
	// テクスチャから色を計算
    outColor = texture(uTexture, fragTexCoord);
}
