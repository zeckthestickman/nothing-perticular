#include "al/LiveActor/ActorPoseKeeper.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/MathUtil.h"
#include <math/seadVector.h>

namespace al {

// regalloc
#ifdef NON_MATCHING
float calcDistance(const al::LiveActor* l1, const al::LiveActor* l2) {
    sead::Vector3f distance = al::getTrans(l2) - al::getTrans(l1);
    return distance.length();
}
#endif

float calcSpeed(const al::LiveActor* actor) {
    return actor->mPoseKeeper->getVelocity().length();
}

float calcSpeedH(const al::LiveActor* actor) {
    sead::Vector3f verticalized;
    al::verticalizeVec(&verticalized, al::getGravity(actor), actor->mPoseKeeper->getVelocity());
    return verticalized.length();
}

float calcSpeedV(const al::LiveActor* actor) {
    const sead::Vector3f& velocity = actor->mPoseKeeper->getVelocity();
    const sead::Vector3f& gravity = al::getGravity(actor);

    return -(velocity.x * gravity.x + velocity.y * gravity.y + velocity.z * gravity.z);
}

}  // namespace al
