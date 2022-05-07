#include "Texture.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/glew.h>


Texture::Texture()
    :TextureID(0)
    ,Width(0)
    ,Height(0)
{
	
}

Texture::~Texture()
{
	
}

// テクスチャのロード（SDL2_image利用）
bool Texture::Load(const std::string& fileName)
{
    
    SDL_Surface* image = NULL;
    SDL_Surface* tmpImage;

    // 画像フォーマット指定
    IMG_Init(IMG_INIT_PNG & IMG_INIT_JPG & IMG_INIT_TIF);
    // 読み込み
    image = IMG_Load(fileName.c_str());
    if (image)
    {
        // 仮のSurface作成してフォーマット指定
        tmpImage = SDL_CreateRGBSurface(SDL_SWSURFACE, 0, 0, 32,
                0x000000ff,
                0x0000ff00,
                0x00ff0000,
                0xff000000);
        /*Usage
                SDL_Surface* SDL_CreateRGBSurface(Uint32 flags, int width, int height, int depth, Uint32 Rmask, Uint32 Gmask, Uint32 Bmask, Uint32 Amask)
         
         flags    未使用 常に0にすること
         width    サーフェイスの幅
         height    サーフェイスの高さ
         depth    サーフェイスのビット深度 (詳細を参照すること)
         Rmask    ピクセルの赤マスク
         Gmask    ピクセルの緑マスク
         Bmask    ピクセルの青マスク
         Amask    ピクセルのαマスク

        */
        //実際のSuraceにコンバート
        image = SDL_ConvertSurface(image, tmpImage->format, SDL_SWSURFACE);
        // 仮のSurface解放
        SDL_FreeSurface(tmpImage);

        // OpenGL用テクスチャ作成
        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);
  
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // 拡大縮小の方法を指定
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 //       glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // SurfaceのPIXELデータをテクスチャに転送
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
        
        // 大きさを記録
        Width  = image->w;
        Height = image->h;

        // Surfce解放
        SDL_FreeSurface(image);
        
    }
    

	return true;
}

// レンダリング用テクスチャを生成
void Texture::CreateForRendering(int width, int height, unsigned int format)
{
    Width = width;
    Height = height;
    // テクスチャ生成
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_2D, TextureID);
    // 空のテクスチャ
    glTexImage2D(GL_TEXTURE_2D, 0, format, Width, Height, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}


// テクスチャ削除
void Texture::Unload()
{
	glDeleteTextures(1, &TextureID);
}

// アクティブ化（Bind）
void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, TextureID);
}
