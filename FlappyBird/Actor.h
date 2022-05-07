#pragma once



#include "Math.h"
#include <vector>
#include <string>



// アクター管理
class Actor
{
public:
	enum State
    { // アクターの状態
		EActive,
		EPaused,
		EDead
	};

    // コンストラクタ
	Actor(class Application* a);
    // デストラクタ
	virtual ~Actor();

    // Updateメソッド、Applicationから呼ばれる
	void Update(float deltaTime);
    // コンポーネントの更新
	void UpdateComponents(float deltaTime);
    // Override前提 派生先の処理を書く
	virtual void UpdateActor(float deltaTime);

    // 入力処理 Applicationから呼ばれる Override不可
	void ProcessInput(const struct InputState& state);
    // 派生先での入力処理 Override
	virtual void ActorInput(const struct InputState& state);

    // ポジションを操作
	const Vector3& GetPosition() const { return position; }
	void SetPosition(const Vector3& pos) { position = pos; bRecomputeWorldTransform = true; }
    // スケールを操作
	float GetScale() const { return scale; }
	void SetScale(float sc) { scale = sc;  bRecomputeWorldTransform = true; }
    // 回転角を操作
	const Quaternion& GetRotation() const { return rotation; }
	void SetRotation(const Quaternion& rot) { rotation = rot;  bRecomputeWorldTransform = true; }

	
    // 座標系を更新
    void ComputeWorldTransform();
    // ワールドマトリックスを取得
    const Matrix4 GetWorldTransform() const { return worldTransform; }
    void SetWorldTransform(const Matrix4 mat) { worldTransform = mat; }
    
    
 
    // 前方を取得（Z方向）
    virtual Vector3 GetForward() { return Vector3::Transform(Vector3::UnitZ, rotation); }
    // 右方を取得（X方向）
    virtual Vector3 GetRight() { return Vector3::Transform(Vector3::UnitX, rotation);}
    // 上方向を取得（Y）
    virtual Vector3 GetUpward() { return Vector3::Transform(Vector3::UnitY, rotation); }

    // ステータスを操作
	State GetState() const { return status; }
	void SetState(State state) { status = state; }

    // オーナーのアプリケーションポインタを取る
    class Application* GetApp() { return app; }


	// コンポーネントの追加削除
	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

    


    
private:


    // マトリックス
    Matrix4     worldTransform;
    Vector3     position;
    Quaternion  rotation;
    float       scale;
    // turueの時のみマトリックスを再計算
    bool        bRecomputeWorldTransform;

    // 保有コンポーネント
    std::vector<class Component*> components;
    // アプリクラス
    class Application* app;
    
    // ステータス
    enum State status; // emum State 状態を管理
    

};

