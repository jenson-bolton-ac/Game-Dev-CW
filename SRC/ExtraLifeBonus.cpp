// ExtraLifeBonus.cpp
#include "ExtraLifeBonus.h"
#include "GameWorld.h"
#include "BoundingSphere.h"
#include "Shape.h"
#include "Player.h"

ExtraLifeBonus::ExtraLifeBonus(float lifetime_ms)
    : Bonus("ExtraLifeBonus", lifetime_ms)
{}

void ExtraLifeBonus::OnCollision(const GameObjectList& objects) {
    Bonus::OnCollision(objects);
}
