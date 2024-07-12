#include "Library/MapObj/FallMapParts.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(FallMapParts, Appear)
NERVE_ACTION_IMPL(FallMapParts, Wait)
NERVE_ACTION_IMPL(FallMapParts, FallSign)
NERVE_ACTION_IMPL(FallMapParts, Fall)
NERVE_ACTION_IMPL(FallMapParts, End)

NERVE_ACTIONS_MAKE_STRUCT(FallMapParts, Appear, Wait, FallSign, Fall, End)
}  // namespace

namespace al {
FallMapParts::FallMapParts(const char* name) : LiveActor(name) {}

void FallMapParts::init(const al::ActorInitInfo& info) {
    initNerveAction(this, "Wait", &NrvFallMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    registerAreaHostMtx(this, info);

    mPos = getTrans(this);

    tryGetArg(&mFallTime, info, "FallTime");
    tryGetArg(&mIsInvalidAutoRestart, info, "IsInvalidAutoRestart");

    trySyncStageSwitchAppear(this);
}

// bool FallMapParts::receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) {}

void FallMapParts::appearAndSetStart() {
    setTrans(this, mPos);
    resetPosition(this);
    showModelIfHide(this);
    startNerveAction(this, "Appear");
    setVelocityZero(this);

    makeActorAlive();
}

void FallMapParts::exeAppear() {
    if (isFirstStep(this)) {
        validateCollisionParts(this);
        if (!tryStartAction(this, "Appear")) {
            startAction(this, "Wait");

            return;
        }
    }

    if (isExistAction(this) && !isActionEnd(this))
        return;

    startAction(this, "Wait");
}

void FallMapParts::exeWait() {
    if (isFirstStep(this)) {
        tryStartAction(this, "Wait");
        validateClipping(this);
    }
}

// TODO: Non Matching
void FallMapParts::exeFallSign() {
    if (isFirstStep(this))
        mIsStartAction = tryStartAction(this, "FallSign");

    if (!mIsStartAction) {
        setTrans(this,
                 (sinf(calcNerveValue(this, 20, 0.0f, 9.424778f)) * 3) * sead::Vector3f::ey + mPos);
        if (!mIsStartAction) {
            if (isGreaterEqualStep(this, 20))
                startNerveAction(this, "Fall");

            return;
        }
    }

    if (isActionEnd(this))
        startNerveAction(this, "Fall");
}
}  // namespace al
