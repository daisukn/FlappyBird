#pragma once

#include <SDL2/SDL.h>
#include <vector>

// アプリ制御クラス
class Application
{
public:
    // コンストラクタ
    Application();
    // デストラクタ
    ~Application();

    // 初期化
    bool Initialize();
    // メインループ
    void RunLoop();
    // 終了処理
    void Shutdown();
    
    // アクターを登録/削除
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);
    
    // 描画エンジンを取得
    class Renderer* GetRenderer() const { return renderer; }
    // 物理エンジンを取得
    class PhysWorld* GetPhysWorld() const { return phyWorld; }
    

    
    
private:
    
    // ゲームデータ読み込み/開放
    void UnloadData();
    void LoadData();
    
    // false 終了、true 実行中
    bool bActive;
    // 描画エンジン
    class Renderer* renderer;
    // 入力エンジン
    class InputSystem* inputSys;
    // 物理エンジン
    class PhysWorld* phyWorld;
    // 時間管理
    unsigned int ticksCount;

    
    // アクティブなアクター
    std::vector<class Actor*> actors;
    // ペンディング中のアクター
    std::vector<class Actor*> pendingActors;
    // true の場合Pendingsにまわす
    bool bUpdatingActors;

    // カメラ
    //class CameraActor* cameraAct;


    // 入力受付
    void ProcessInput();
    // ゲームメイン
    void UpdateGame();
    // 描画
    void Draw();


};


