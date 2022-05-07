#version 410

// 頂点シェーダーから受け取るテクスチャ座標
in vec2 fragTexCoord;

// アウトプットする色
out vec4 outColor;

// テクスチャ
uniform sampler2D uTexture;

void main()
{
	// テクスチャから色取得
    outColor = texture(uTexture, fragTexCoord);
}
