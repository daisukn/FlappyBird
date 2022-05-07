#include "Application.h"
#include "Actor.h"
#include "Component.h"

#include <algorithm>

//コンストラクタ
Actor::Actor(Application* a)
    : status(EActive)
    , position(Vector3::Zero)
    , rotation(Quaternion::Identity)
    , scale(1.0f)
    , app(a)
    , bRecomputeWorldTransform(true)
{
    app->AddActor(this);
}

// デストラクタ
Actor::~Actor()
{
    // Applicaationから削除
    app->RemoveActor(this);

    // 保持しているComponentはここで削除
	while (!components.empty())
    {
		delete components.back();
	}
}

// メインルーチン
void Actor::Update(float deltaTime)
{
    // EActiveの場合にComponentもUpdate
	if (status == EActive)
    {


        // 派生先の処理を呼ぶ
        UpdateActor(deltaTime);
        
        // コンポーネントを更新
		UpdateComponents(deltaTime);
        
        
        // 座標系更新
        ComputeWorldTransform();

	}
}

// コンポーネントのUpdateを呼ぶ
void Actor::UpdateComponents(float deltaTime)
{
	for (auto comp : components)
    {
		comp->Update(deltaTime);
	}
}


void Actor::UpdateActor(float deltaTime)
{
    // 実体は派生先でOverride
}

// 入力を受け取る
void Actor::ProcessInput(const struct InputState& state)
{
	if (status == EActive)
    {
        // コンポーネントの入力処理
		for (auto comp : components)
        {
			comp->ProcessInput(state);
		}
        // 派生先の入力処理に渡す
		ActorInput(state);
	}
}

// 入力処理　実装は派生先
void Actor::ActorInput(const struct InputState& state)
{
}

// ワールドマトリックス
void Actor::ComputeWorldTransform()
{
	if (bRecomputeWorldTransform)
    {
		bRecomputeWorldTransform = false;
		// Scale → rotate → translate
		worldTransform = Matrix4::CreateScale(scale);
		worldTransform *= Matrix4::CreateFromQuaternion(rotation);
		worldTransform *= Matrix4::CreateTranslation(position);


		// 各Componentも計算する
		for (auto comp : components)
		{
			comp->OnUpdateWorldTransform();
		}
	}
}

// コンポーネントを追加
void Actor::AddComponent(Component* component)
{
    // 自分より優先度の高いものを探しす
	int myOrder = component->GetUpdateOrder();
	auto iter = components.begin();
	for (;iter != components.end(); ++iter) {
		if (myOrder < (*iter)->GetUpdateOrder()){
			break;
		}
	}

	// その次に自分を挿入
	components.insert(iter, component);
}

// コンポーネントを削除
void Actor::RemoveComponent(Component* component)
{	
    auto iter = std::find(components.begin(), components.end(), component);
	if (iter != components.end())
    {
		components.erase(iter);
	}
}


