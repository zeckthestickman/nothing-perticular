#include "Library/MapObj/GateMapParts.h"

#include "Library/Audio/System/SimpleAudioUser.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(GateMapParts, Wait)
NERVE_ACTION_IMPL(GateMapParts, Open)
NERVE_ACTION_IMPL(GateMapParts, Bound)
NERVE_ACTION_IMPL(GateMapParts, End)

NERVE_ACTIONS_MAKE_STRUCT(GateMapParts, Wait, Open, Bound, End)
}  // namespace

namespace al {
void setQuat(sead::Quatf* out, const sead::Quatf& other) {
    out->x = other.x;
    out->y = other.y;
    out->z = other.z;
    out->w = other.w;
}

// can be something like

// template <typename T>
// inline void Quat<T>::set(const Quat<T>& other)
// {
//     QuatCalcCommon<T>::set(*this, other);
// }

GateMapParts::GateMapParts(const char* name) : LiveActor(name) {}

void GateMapParts::init(const ActorInitInfo& info) {
    using GateMapPartsFunctor = FunctorV0M<GateMapParts*, void (GateMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvGateMapParts.mCollector, 0);
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);

    mTrans = getTrans(this);
    // mQuat = getQuat(this);
    setQuat(&mQuat, getQuat(this));

    tryGetLinksTrans(&mMoveNextTrans, info, "MoveNext");
    tryGetLinksQuat(&mMoveNextQuat, info, "MoveNext");

    tryGetArg(&mOpenTime, info, "OpenTime");
    tryGetArg(&mBoundRate, info, "BoundRate");
    tryGetArg(&mHitReactionCount, info, "HitReactionCount");

    listenStageSwitchOnStart(this, GateMapPartsFunctor(this, &GateMapParts::start));

    bool isPlaySuccessSe = false;
    tryGetArg(&isPlaySuccessSe, info, "IsPlaySuccessSe");

    if (isPlaySuccessSe)
        mSuccessSeObj = new SimpleAudioUser("SuccessSeObj", info);

    makeActorAlive();
}

void GateMapParts::start() {
    if (!isNerve(this, NrvGateMapParts.Wait.data()))
        return;

    appearAndSetStart();
}

void GateMapParts::appearAndSetStart() {
    invalidateClipping(this);
    startNerveAction(this, "Open");
}

void GateMapParts::exeWait() {
    if (isFirstStep(this))
        validateClipping(this);
}

void GateMapParts::exeOpen() {
    updatePose(calcNerveSquareInRate(this, mOpenTime - 1));

    if (isGreaterEqualStep(this, mOpenTime - 1)) {
        _154 = mBoundRate;
        _14c = (s32)(mBoundRate * (f32)mOpenTime + mBoundRate * (f32)mOpenTime);
        _150 = 0;

        if (_140 > _150 && _14c > 1) {
            startAction(this, "Bound");

            return;
        }

        if (mSuccessSeObj != nullptr)
            startSe(mSuccessSeObj, "Riddle");

        startAction(this, "End");

        if (mHitReactionCount < 2)
            startHitReaction(this, "バウンド1回目");
    }
}

void GateMapParts::updatePose(f32 rate) {
    rate = sead::Mathf::clamp(rate, 0.0f, 1.0f);

    lerpVec(getTransPtr(this), mTrans, mMoveNextTrans, rate);
    slerpQuat(getQuatPtr(this), mQuat, mMoveNextQuat, rate);
}

// TODO: Non Matching
void GateMapParts::exeBound() {
    if (isFirstStep(this)) {
        if (_150++ < mHitReactionCount)
            startHitReaction(this, StringTmp<32>("バウンド%d回目", _150).cstr());

        tryStartSeWithParam(this, "BoundStart", (f32)(_140 - _150), "");
    }

    f32 fVar7 = calcNerveRate(this, _14c - 1);
    fVar7 = _154 * (fVar7 * 2 - 1.0f);

    updatePose((1.0f - sead::Mathf::pow(_154, 2)) + sead::Mathf::pow(fVar7, 2));

    if (isGreaterEqualStep(this, _14c - 1)) {
        _154 *= mBoundRate;
        _14c = (s32)(mBoundRate * (f32)_14c);

        if (_140 > _150 && _14c > 1) {
            startAction(this, "Bound");

            return;
        }

        if (mSuccessSeObj != nullptr)
            startSe(mSuccessSeObj, "Riddle");

        startAction(this, "End");
    }
}

void GateMapParts::exeEnd() {
    if (isFirstStep(this)) {
        validateClipping(this);
        updatePose(1.0f);
    }
}
}  // namespace al
