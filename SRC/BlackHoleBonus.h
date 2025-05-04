#ifndef __BLACKHOLEBONUS_H__
#define __BLACKHOLEBONUS_H__

#include "Bonus.h"
#include <memory>

class BlackHoleBonus : public Bonus {
public:
    /// @param lifetime_ms  How long (ms) the bonus floats before disappearing
    BlackHoleBonus(float lifetime_ms = 10000.0f);
    virtual ~BlackHoleBonus() {}

    /// When the ship collides, spawn a BlackHole at this position then remove the bonus
    virtual void OnCollision(const GameObjectList& objects) override;
};

#endif // __BLACKHOLEBONUS_H__
