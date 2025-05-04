// ExtraLifeBonus.h
#ifndef __EXTRALIFEBONUS_H__
#define __EXTRALIFEBONUS_H__

#include "Bonus.h"
class ExtraLifeBonus : public Bonus {
public:
    ExtraLifeBonus(float lifetime_ms = 10000.0f);
    virtual ~ExtraLifeBonus() {}
    virtual void OnCollision(const GameObjectList& objects) override;
};
#endif
