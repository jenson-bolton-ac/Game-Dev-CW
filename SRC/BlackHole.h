#ifndef __BLACKHOLE_H__
#define __BLACKHOLE_H__

#include "GameObject.h"

/// A temporary gravity well that attracts (and can optionally destroy) asteroids.
class BlackHole : public GameObject {
public:
    /// @param centre      Initial position of the hole
    /// @param radius      Radius of effect
    /// @param strength    Gravitational strength constant
    /// @param duration_ms How long (ms) the hole remains active
    BlackHole(const GLVector3f& centre,
        float radius,
        float strength,
        int duration_ms);

    virtual ~BlackHole() {}

    /// Advance lifetime (removal logic only)
    virtual void Update(int t) override;
    virtual bool CollisionTest(std::shared_ptr<GameObject> o) override;
    virtual void OnCollision(const GameObjectList& objects) override;

private:
    float mRadius;
    float mStrength;
    int   mDuration;  ///< Remaining lifetime, in ms
};

#endif // __BLACKHOLE_H__
