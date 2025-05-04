#ifndef __WEAPONUPGRADEBONUS_H__
#define __WEAPONUPGRADEBONUS_H__

#include "Bonus.h"

/// A pickup that grants the ship dual-shot (front + back) for a short time.
class WeaponUpgradeBonus : public Bonus {
public:
    /// @param lifetime_ms  ms before the bonus expires if uncollected
    WeaponUpgradeBonus(float lifetime_ms = 10000.0f);
    virtual ~WeaponUpgradeBonus() {}

    /// Activate the ship’s dual-shot and remove this bonus
    virtual void OnCollision(const GameObjectList& objects) override;
};

#endif // __WEAPONUPGRADEBONUS_H__
