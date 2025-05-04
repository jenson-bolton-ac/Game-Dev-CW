#include "WeaponUpgradeBonus.h"
#include "GameWorld.h"
#include "Spaceship.h"
#include "BoundingSphere.h"

WeaponUpgradeBonus::WeaponUpgradeBonus(float lifetime_ms)
    : Bonus("WeaponUpgradeBonus", lifetime_ms)
{
}

void WeaponUpgradeBonus::OnCollision(const GameObjectList& objects) {
    for (auto& obj : objects) {
        if (obj->GetType() == GameObjectType("Spaceship")) {
            auto ship = static_cast<Spaceship*>(obj.get());
            ship->ActivateDualShot(10000);  // 10 seconds
        }
    }
    Bonus::OnCollision(objects);
}
