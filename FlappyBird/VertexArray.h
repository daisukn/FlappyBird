#pragma once


#include <vector>

// 頂点データ管理
class VertexArray
{
public:

    // コンストラクタ（フォーマットごとに３種類）
    
    // 主にスプライト用
    VertexArray(const float* verts, unsigned int num_verts,
                 const unsigned int* indices, unsigned int num_indices);

    // メッシュ用
    VertexArray(unsigned int num_verts,
                const float* verts,
                const float* norms,
                const float* uvs,
                unsigned int num_indices,
                const unsigned int* indices);


    // アニメーションメッシュ用
    VertexArray(unsigned int num_verts,
                const float* verts,
                const float* norms,
                const float* uvs,
                const unsigned int* boneids,
                const float* weights,
                unsigned int num_indices,
                const unsigned int* indices);
    
    // デストラクタ
    ~VertexArray();
    
    // 描画用にBind
    void SetActive();
    // テクスチャIDを設定
    void SetTextureID(unsigned int id) { textureID = id; }
    unsigned int GetTextureID() const { return textureID; }

    // インデックス数
    unsigned int GetNumIndices() const { return numIndices; }
    // 頂点数
    unsigned int GetNumVerts() const { return numVerts; }
    // 面データ
    std::vector<struct Polygon*> GetPolygon() const { return polygons; }
    

private:
    // 頂点の数
    unsigned int numVerts;
    // インデックスの数
    unsigned int numIndices;
    // 頂点バッファVBO
    unsigned int vertexBuffer[5];

    
    // 頂点データID
    unsigned int vertexBufferID;
    // インデックスバッファID
    unsigned int indexBufferID;
    // テクスチャID
    unsigned int textureID;
    
    // ポリゴンデータ格納（計算用途）
    std::vector<struct Polygon*> polygons;
    void CreatePolygon(const float* verts, const unsigned int* indices, const unsigned int num);
    
};


