#include "VertexArray.h"
#include "Polygon.h"

#include <GL/glew.h>

VertexArray::VertexArray(unsigned int num_verts,
                         const float* verts,
                         const float* norms,
                         const float* uvs,
                         const unsigned int* bonids,
                         const float* weights,
                         unsigned int num_indices,
                         const unsigned int* indices)
{

    numVerts  = num_verts;
    numIndices = num_indices;
       
       
    glGenVertexArrays(1, &vertexBufferID);
    glBindVertexArray(vertexBufferID);
       
    // インデックスバッファー
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_indices, indices, GL_STATIC_DRAW);
       
       
    glGenBuffers(5, vertexBuffer);
    // 頂点バッファー転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts[0]) * num_verts * 3, verts, GL_STATIC_DRAW);

    // 法線バッファー転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(norms[0]) * num_verts * 3, norms, GL_STATIC_DRAW);
    
    // UVバッファー転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs[0]) * num_verts * 2, uvs, GL_STATIC_DRAW);

    // BoneID転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bonids[0]) * num_verts * 4, bonids, GL_STATIC_DRAW);
    // Weight転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(weights[0]) * num_verts * 4, weights, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);

    // Position
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // Norm
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    // UV
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    // BoneID
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[3]);
    glVertexAttribIPointer(3, 4, GL_INT, 0, 0);
    // Weights
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[4]);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, 0);
    
    CreatePolygon(verts, indices, numIndices);

}


VertexArray::VertexArray(unsigned int num_verts,
                         const float* verts,
                         const float* norms,
                         const float* uvs,
                         unsigned int num_indices,
                         const unsigned int* indices)
{

    numVerts  = num_verts;
    numIndices = num_indices;
    
    
    glGenVertexArrays(1, &vertexBufferID);
    glBindVertexArray(vertexBufferID);
    
    // インデックスバッファー
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * num_indices, indices, GL_STATIC_DRAW);
    
    
    
    // 頂点バッファー転送
    glGenBuffers(3, vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_verts * 3, verts, GL_STATIC_DRAW);

    // 法線バッファー転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_verts * 3, norms, GL_STATIC_DRAW);
 
    // UVバッファー転送
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_verts * 2, uvs, GL_STATIC_DRAW);


    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    // バッファオブジェクトに転送した頂点位置をインデックス 0 に関連付ける
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[2]);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
    
    CreatePolygon(verts, indices, numIndices);

}

// VBO作成
VertexArray::VertexArray(const float* verts,unsigned int i_verts, 
                           const unsigned int* indices, unsigned int i_indices)
{
    
    numVerts  = i_verts;
    numIndices = i_indices;
    
    // vertex array生成
    glGenVertexArrays(1, &vertexBufferID);
    glBindVertexArray(vertexBufferID);

    unsigned vertexSize = 8 * sizeof(float);

    
    // 頂点バッファー
    glGenBuffers(1, vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer[0]);
    glBufferData(GL_ARRAY_BUFFER, numVerts * vertexSize, verts, GL_STATIC_DRAW);

    // インデックスバッファー
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // 頂点Index用アトリビュート
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, 0);
    // 法線ベクトル用アトリビュート
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 3));
    // UV座標用アトリビュート
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, reinterpret_cast<void*>(sizeof(float) * 6));
    
    
    CreatePolygon(verts, indices, numIndices);

}

// ポリゴンデータ生成
void VertexArray::CreatePolygon(const float* verts, const unsigned int* indices, const unsigned int num)
{
    for (unsigned int i = 0;  i < num/3; i++)
    {
        Polygon* f = new Polygon();
        
        f->a.x = verts[(indices[i*3] * 3)];
        f->a.y = verts[(indices[i*3] * 3)+1];
        f->a.z = verts[(indices[i*3] * 3)+2];

        f->b.x = verts[(indices[i*3+1] * 3)];
        f->b.y = verts[(indices[i*3+1] * 3)+1];
        f->b.z = verts[(indices[i*3+1] * 3)+2];

        f->c.x = verts[(indices[i*3+2] * 3)];
        f->c.y = verts[(indices[i*3+2] * 3)+1];
        f->c.z = verts[(indices[i*3+2] * 3)+2];
        
        polygons.push_back(f);

    }
}


VertexArray::~VertexArray()
{
    polygons.clear();

    glDeleteBuffers(1, vertexBuffer);
    glDeleteBuffers(1, &indexBufferID);
    glDeleteVertexArrays(1, &vertexBufferID);
}

// アクティブ化
void VertexArray::SetActive()
{
    glBindVertexArray(vertexBufferID);
}
