#include "Player/PlayerJudgeForceSlopeSlide.h"

#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"

PlayerJudgeForceSlopeSlide::PlayerJudgeForceSlopeSlide(const al::LiveActor* player,
                                                       const PlayerConst* pConst,
                                                       const IUsePlayerCollision* collider)
    : mPlayer(player), mConst(pConst), mCollider(collider) {}

void PlayerJudgeForceSlopeSlide::reset() {
    mIsJudge = false;
}

void PlayerJudgeForceSlopeSlide::update() {
    mIsJudge = rs::isOnGroundForceSlideCode(mPlayer, mCollider, mConst);
}

bool PlayerJudgeForceSlopeSlide::judge() const {
    return !rs::isPlayerHack(mPlayer) && mIsJudge;
}
