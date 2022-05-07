#version 410

// 頂点シェーダーからのデータ
// テクスチャ座標
in vec2 fragTexCoord;


// アウトプットする色
out vec4 outColor;

// テクスチャ
uniform sampler2D uTexture;


void main()
{

    // テクスチャから色を計算
    outColor = texture(uTexture, fragTexCoord);
    
}
