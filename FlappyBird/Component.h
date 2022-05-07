#pragma once

#include "Math.h"

#include <cstdint>


// コンポーネント系の基底クラス　Sprite, Mesh等に派生する　Actor系が保持して実行
class Component
{
public:
	// コンストラクタ UpdateOrderが小さいものが優先される
	Component( class Actor* a, int order = 100 );
    // デストラクタ
    virtual ~Component();
	// Actorから呼ばれる　Overrideして更新処理
	virtual void Update( float deltaTime );
	// Overrideして入力を受け付ける
	virtual void ProcessInput( const struct InputState& state ) {}
	// Overrideして座標系の更新
	virtual void OnUpdateWorldTransform() {}

    // UpdateOrder取得
	int GetUpdateOrder() const { return updateOrder; }
    
    // ポジションを返す
    virtual Vector3 GetPosition();
    
    class Actor* GetOwner() const { return owner; }

    
protected:
    // オーナーとなるActor
    class Actor* owner;
	// アップデートオーダー
	int updateOrder;
    

    
};


