#include "Renderer.h"
#include "Shader.h"
#include "SpriteComponent.h"
#include "Texture.h"
#include "VertexArray.h"



#include <GL/glew.h>
#include <algorithm>

// コンストラクタ
Renderer::Renderer()
{
}

// デストラクタ
Renderer::~Renderer()
{
}


// ウィンドウ生成とGL初期化
bool Renderer::Initialize(std::string title, float scWidth, float scHeight)
{
    strTitle        = title;    // ウィンドウタイトル
    screenWidth     = scWidth;  // スクリーン幅
    screenHeight    = scHeight; // スクリーン高さ
    
    
    // OpenGL プロファイル, バージョン
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // ダブルバッファリング
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // RGBAチャンネル
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    // ハードウェアアクセラレーション有効化
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    
    
    //ウインドウ生成
    lpWnd = SDL_CreateWindow(strTitle.c_str(), 100, 100, static_cast<int>(screenWidth), static_cast<int>(screenHeight), SDL_WINDOW_OPENGL);
    if (!lpWnd )
    {
        std::cout << "Unable to create window" << std::endl;
        return false;
    }
    
    
    // OpenGL コンテキスト生成
    lpGL = SDL_GL_CreateContext(lpWnd);
    
    // GLEW初期化
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }
   
    
    /*シェーダー のロードなどはここでやる*/
    LoadShaders();
    CreateSpriteVerts();
 
    return true;
}

// リリース処理
void Renderer::Shutdown()
{
    SDL_GL_DeleteContext(lpGL);
    SDL_DestroyWindow(lpWnd);

}


// 描画処理
void Renderer::Draw()
{
    glClearColor(0.596f, 0.733f, 0.858f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    
    //glEnable( GL_CULL_FACE );
    //Culling する面の向きを指定
    //glFrontFace(GL_CCW);
    
    

//    glEnable(GL_BLEND);

    
    
    // スプライト処理
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    // アルファブレンディング
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    spriteShader->SetActive();    
    for (auto sprite : spriteComps)
    {
        sprite->Draw(spriteShader);
    }
    

    SDL_GL_SwapWindow(lpWnd);

}

// シェーダー 読み込み
bool Renderer::LoadShaders()
{

    // スプライト用シェーダー生成
    spriteShader = new Shader();
    if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
    {
        return false;
    }
    spriteShader->SetActive();
    
    
    // ビューマトリックス、プロジェクションマトリックス
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(screenWidth, screenHeight);
    spriteShader->SetMatrixUniform("uViewProj", viewProj);

    
    /*
    // パーティクル用シェーダー
    billboardShader = new Shader();
    if (!billboardShader->Load("Shaders/Billboard.vert", "Shaders/Sprite.frag"))
    {
        return false;
    }
    billboardShader->SetActive();
    
    
    // メッシュ用シェーダー生成
    meshShader = new Shader();
    if (!meshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    meshShader->SetActive();
    
    
    // スキンメッシュ用シェーダー
    skinnedShader = new Shader();
    if (!skinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
    {
        return false;
    }
    skinnedShader->SetActive();
    */

    
    // ビューマトリックス、プロジェクションマトリックス（デフォルト値）
    viewMatrix = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitZ, Vector3::UnitY);
    projectionMatrix = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f), screenWidth, screenHeight, 1.0f, 10000.0f);
    
    // シェーダーに送る
//    meshShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
//    skinnedShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
//    billboardShader->SetMatrixUniform("uViewProj", viewMatrix * projectionMatrix);
    
    return true;

}

// ライト設定
void Renderer::SetLightUniforms(Shader* shader)
{
    // カメラポジション
    Matrix4 invView = viewMatrix;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", ambientLight);
    // Directional light
    shader->SetVectorUniform("uDirLight.mDirection", dirLight.Direction);
    shader->SetVectorUniform("uDirLight.mDiffuseColor", dirLight.DiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", dirLight.SpecColor);
    
    
    // フォグ
    shader->SetFloatUniform("uFoginfo.maxDist", 1600.0f);
    shader->SetFloatUniform("uFoginfo.minDist", 1.0f);
    shader->SetVectorUniform("uFoginfo.color", Vector3(0.596f, 0.733f, 0.858f) );

}

// スプライトコンポーネントの登録
void Renderer::AddSprite(SpriteComponent* sprite)
{
    // DrawOrderを探して 自分より優先度の高いものの次を見つける
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = spriteComps.begin();
    for (;iter != spriteComps.end(); ++iter)
    {
        if (myDrawOrder < (*iter)->GetDrawOrder())
        {
            break;
        }
    }

    // 見つけた箇所に挿入
    spriteComps.insert(iter, sprite);
}

// スプライト削除
void Renderer::RemoveSprite(SpriteComponent* sprite)
{
    auto iter = std::find(spriteComps.begin(), spriteComps.end(), sprite);
    spriteComps.erase(iter);
}

// テクスチャ取り出し
Texture* Renderer::GetTexture(const std::string &fileName){
    Texture* tex = nullptr;
    auto iter = textures.find(fileName);
    if (iter != textures.end())
    {
        tex = iter->second;
    }
    else
    {
        tex = new Texture();
        if(tex->Load(fileName))
        {
            textures.emplace(fileName, tex);
        }
        else
        {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

//スプライト用ポリゴン
void Renderer::CreateSpriteVerts()
{
    const float vertices[] = {
        -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f, // top left
        0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // top right
        0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // bottom right
        -0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f  // bottom left
    };
    

    const unsigned int indices[] = {
        2, 1, 0,
        0, 3, 2
    };
    spriteVerts = new VertexArray((float*)vertices, (unsigned int)4, (unsigned int*)indices, (unsigned int)6);
}













// データ解放
void Renderer::UnloadData()
{
    // テクスチャ削除
    for (auto i : textures)
    {
        i.second->Unload();
        delete i.second;
    }
    textures.clear();


}
