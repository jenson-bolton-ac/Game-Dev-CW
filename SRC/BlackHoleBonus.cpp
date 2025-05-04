#include "GameUtil.h"
#include <memory>
#include "Animation.h"
#include "AnimationManager.h"
#include "BlackHoleBonus.h"
#include "BlackHole.h"
#include "BoundingSphere.h"   // for pickup’s collision shape
#include "GameWorld.h"
#include "Sprite.h"

BlackHoleBonus::BlackHoleBonus(float lifetime_ms)
    : Bonus("BlackHoleBonus", lifetime_ms)
{
}

void BlackHoleBonus::OnCollision(const GameObjectList& objects) {
    // Construct the hole
    auto hole = make_shared<BlackHole>(
        GetPosition(),
        /*radius=*/100.0f,
        /*strength=*/500.0f,
        /*duration_ms=*/5000
    );

    hole->SetBoundingShape(
        make_shared<BoundingSphere>(hole->GetThisPtr(), 100.0f)
    );

    Animation* bh_anim_ptr = AnimationManager::GetInstance()
        .GetAnimationByName("blackhole");
    auto bh_sprite = make_shared<Sprite>(
        bh_anim_ptr->GetWidth(),
        bh_anim_ptr->GetHeight(),
        bh_anim_ptr
    );
    bh_sprite->SetLoopAnimation(true);
    hole->SetSprite(bh_sprite);
    hole->SetScale(0.5f);

    hole->Reset();

    // Add it to the world
    if (mWorld) {
        mWorld->AddObject(hole);
    }

    // Remove the bonus itself
    Bonus::OnCollision(objects);

}