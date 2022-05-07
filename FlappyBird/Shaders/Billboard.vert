#version 410

// Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform vec3 uPosition;

// Attribute（頂点座標、法線、UV）
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// フラグメントシェーダーに渡すUV
out vec2 fragTexCoord;


void main()
{
    
    vec3 p = inPosition + uPosition;
    
    
    // コンバート
    vec4 pos = vec4(p, 1.0);
    // 頂点座標のワールド変換
    pos = pos * uWorldTransform;

    // 投影座標
    gl_Position = pos * uViewProj;


    // UV座標
    fragTexCoord = inTexCoord;
}
