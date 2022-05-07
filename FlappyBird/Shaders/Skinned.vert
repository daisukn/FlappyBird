#version 410

// Uniforms
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
// Uniform（ボーンごとのマトリックス）
uniform mat4 uMatrixPalette[96];

// Attributes （頂点座標、法線ベクトル、UV座標、ボーンID、ウェイト）
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uvec4 inSkinBones;
layout(location = 4) in vec4 inSkinWeights;


// フラグメントシェーダーに渡すUV
out vec2 fragTexCoord;
// フラグメントシェーダーに渡す法線ベクトル
out vec3 fragNormal;
// フラグメントシェーダーに渡す頂点座標
out vec3 fragWorldPos;

void main()
{
    // ベクトルに変換
    vec4 pos = vec4(inPosition, 1.0);
    
    // ボーンとウェイトから頂点座標を計算
    mat4 skinnedMat = uMatrixPalette[inSkinBones[0]] * inSkinWeights[0];
    skinnedMat += uMatrixPalette[inSkinBones[1]] * inSkinWeights[1];
    skinnedMat += uMatrixPalette[inSkinBones[2]] * inSkinWeights[2];
    skinnedMat += uMatrixPalette[inSkinBones[3]] * inSkinWeights[3];
    
    vec4 skinnedPos = pos * skinnedMat;

    // 頂点座標のワールド変換
    skinnedPos = skinnedPos * uWorldTransform;
    // フラグメントシェーダーに送る
    fragWorldPos = skinnedPos.xyz;
    // 投影座標
    gl_Position = skinnedPos * uViewProj;

    // 法線ベクトルのワールド変換
    vec4 skinnedNormal = vec4(inNormal, 0.0f);
    skinnedNormal = skinnedNormal * skinnedMat;
    // フラグメントシェーダーに送る
    fragNormal = (skinnedNormal * uWorldTransform).xyz;

    // ふrグメントシェーダーにUVを送る
    fragTexCoord = inTexCoord;
}


