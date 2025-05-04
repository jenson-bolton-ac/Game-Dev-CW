#include "BlackHole.h"
#include "GameWorld.h"
#include "Asteroid.h"
#include "BoundingSphere.h"

BlackHole::BlackHole(const GLVector3f& centre,
    float radius,
    float strength,
    int duration_ms)
    : GameObject("BlackHole", centre, GLVector3f(), GLVector3f(), 0.0f, 0.0f),
    mRadius(radius),
    mStrength(strength),
    mDuration(duration_ms)
{
}

void BlackHole::Update(int t) {
    GameObject::Update(t);
    mDuration -= t;
    if (mDuration <= 0 && mWorld) {
        mWorld->FlagForRemoval(GetThisPtr());
    }
}

bool BlackHole::CollisionTest(std::shared_ptr<GameObject> o) {
    if (o->GetType() != GameObjectType("Asteroid")) return false;
    if (!mBoundingShape || !o->GetBoundingShape()) return false;
    return mBoundingShape->CollisionTest(o->GetBoundingShape());
}

void BlackHole::OnCollision(const GameObjectList& objects) {
    for (auto& obj : objects) {
        auto toCentre = GetPosition() - obj->GetPosition();
        float dist = toCentre.length();
        if (dist > 0.0f) {
            toCentre.normalize();
            float force = mStrength / (dist * dist);
            obj->AddAcceleration(toCentre * force);
        }
    }
}