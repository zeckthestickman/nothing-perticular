#include "al/MapObj/FixMapParts.h"
#include "al/sensor/SensorMsg.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/OtherUtil.h"
#include "al/util/StageSwitchUtil.h"

namespace al {
FixMapParts::FixMapParts(const char* name) : LiveActor(name) {}
void FixMapParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);
    al::trySyncStageSwitchAppearAndKill(this);
    al::registActorToDemoInfo(this, info);

    if (mModelKeeper != nullptr && !al::isExistAction(this) && !al::isViewDependentModel(this)) {
        mStatic = true;
    }
}
void FixMapParts::appear() {
    LiveActor::appear();

    if (al::isExistModel(this))
        al::tryStartAction(this, "Appear");
}
void FixMapParts::movement() {
    if (!mStatic)
        LiveActor::movement();
}
void FixMapParts::calcAnim() {
    if (!mStatic)
        LiveActor::calcAnim();
    else
        al::calcViewModel(this);
}
bool FixMapParts::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {
    if (al::isMsgAskSafetyPoint(message))
        return !al::isValidSwitchAppear(this) && !al::isValidSwitchKill(this);

    if (al::isMsgShowModel(message)) {
        al::showModelIfHide(this);
        return true;
    }

    if (al::isMsgHideModel(message)) {
        al::hideModelIfShow(this);
        return true;
    }

    return false;
}
}  // namespace al
