#include "Component.h"
#include "Actor.h"

Component::Component(Actor* a, int order)
    : owner(a)
    , updateOrder(order)
{
	// アクターに持たせる
	owner->AddComponent(this);
}

Component::~Component()
{
	owner->RemoveComponent(this);
}

void Component::Update(float deltaTime)
{
}

Vector3 Component::GetPosition()
{
    return owner->GetPosition();
}
