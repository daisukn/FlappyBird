#pragma once


#include <string>

// テクスチャを管理
class Texture
{
public:
	Texture();
	~Texture();
	
    // テクスチャ読み込み（SDL2_image）
	bool Load(const std::string& fileName);
    
    void CreateForRendering(int width, int height, unsigned int format);
    
    // 削除
	void Unload();
    // 描画用にBind
	void SetActive();
	// 幅と高さ取得
    int GetWidth() const { return Width; }
    int GetHeight() const { return Height; }

private:
    // OpenGLが管理するID
	unsigned int TextureID;
	int Width;
	int Height;
};


