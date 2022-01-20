#include "al/util/LiveActorUtil.h"
#include "al/util/MathUtil.h"
#include "math/seadVector.h"

namespace al {

// regalloc
#ifdef NON_MATCHING
float calcDistance(const al::LiveActor* l1, const al::LiveActor* l2) {
    const sead::Vector3f* l2Trans = al::getTrans(l2);
    const sead::Vector3f* l1Trans = al::getTrans(l1);
    float x = l2Trans->x - l1Trans->x;
    float y = l2Trans->y - l1Trans->y;
    float z = l2Trans->z - l1Trans->z;

    return sqrtf(x * x + y * y + z * z);
}
#endif

float calcSpeed(const al::LiveActor* actor) {
    const sead::Vector3f* velocity = actor->mPoseKeeper->getVelocity();
    return sqrtf(velocity->x * velocity->x + velocity->y * velocity->y + velocity->z * velocity->z);
}

float calcSpeedH(const al::LiveActor* actor) {
    const sead::Vector3f* gravity = al::getGravity(actor);
    const sead::Vector3f* velocity = actor->mPoseKeeper->getVelocity();
    sead::Vector3f verticalized;

    al::verticalizeVec(&verticalized, *gravity, *velocity);
    return sqrtf(verticalized.x * verticalized.x + verticalized.y * verticalized.y +
                 verticalized.z * verticalized.z);
}

float calcSpeedV(const al::LiveActor* actor) {
    const sead::Vector3f* velocity = actor->mPoseKeeper->getVelocity();
    const sead::Vector3f* gravity = al::getGravity(actor);

    return -(velocity->x * gravity->x + velocity->y * gravity->y + velocity->z * gravity->z);
}

}  // namespace al
