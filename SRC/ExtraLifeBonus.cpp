// ExtraLifeBonus.cpp
#include "ExtraLifeBonus.h"
#include "GameWorld.h"
#include "BoundingSphere.h"
#include "Shape.h"
#include "Player.h"

ExtraLifeBonus::ExtraLifeBonus(float lifetime_ms)
    : Bonus("ExtraLifeBonus", lifetime_ms)
{}

void ExtraLifeBonus::OnCollision(const GameObjectList& /*objects*/) {
    // 1) Grant an extra life
    if (mWorld) {
        mWorld->GetPlayer().AddLife();  // Player::AddLife increments mLives and updates GUI :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}
    }
    // 2) Remove this bonus
    Bonus::OnCollision(objects);
}
