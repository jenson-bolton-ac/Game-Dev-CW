#include "Bonus.h"
#include "BoundingShape.h"
#include "GameWorld.h"

Bonus::Bonus(const char* type_name, float lifetime)
    : GameObject(type_name), mLifetime(lifetime)
{}

void Bonus::Update(int t) {
    GameObject::Update(t);
    mLifetime -= t;
    if (mLifetime <= 0 && mWorld) {
        mWorld->FlagForRemoval(GetThisPtr());
    }
}

bool Bonus::CollisionTest(std::shared_ptr<GameObject> o) {
    // Only interact with the player’s ship
    if (o->GetType() != GameObjectType("Spaceship")) return false;
    if (!mBoundingShape || !o->GetBoundingShape())  return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void Bonus::OnCollision(const GameObjectList& objects) {
    // Subclasses will implement effect; we simply remove the bonus
    if (mWorld) {
        mWorld->FlagForRemoval(GetThisPtr());
    }
}
