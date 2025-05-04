#ifndef __BONUS_H__
#define __BONUS_H__

#include "GameUtil.h"
#include "GameObject.h"

/// Base class for all pick-ups (“bonuses”) in the world.
class Bonus : public GameObject {
public:
    /// @param type       The RTTI name for this bonus (e.g. "BlackHoleBonus")
    /// @param lifetime   How long (ms) before the bonus self-destructs if uncollected.
    Bonus(const char* type_name, float lifetime = 10000.0f);
    virtual ~Bonus() {}


    /// Decrease lifetime; remove when <= 0.
    virtual void Update(int t) override;

    /// Only collide with the Spaceship.
    virtual bool CollisionTest(std::shared_ptr<GameObject> o) override;

    /// On collision: remove this bonus (effect applied in subclass).
    virtual void OnCollision(const GameObjectList& objects) override;

protected:
    float mLifetime;
};

#endif // __BONUS_H__