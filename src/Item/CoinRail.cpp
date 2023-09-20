#include "Item/CoinRail.h"

#include <math/seadVector.h>
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathLengthUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

#include "Item/Coin.h"

namespace {
using namespace al;
NERVE_IMPL(CoinRail, CloseMove);
NERVE_IMPL(CoinRail, Move);

struct {
    NERVE_MAKE(CoinRail, CloseMove);
    NERVE_MAKE(CoinRail, Move);
} NrvCoinRail;
}  // namespace

CoinRail::CoinRail(const char* name) : al::LiveActor(name) {}

void CoinRail::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    if (!al::isExistRail(this)) {
        kill();
        return;
    }

    al::getArg(&mCoinNum, info, "CoinNum");
    if (mCoinNum <= 1) {
        kill();
        return;
    }

    bool isLoop = al::isLoopRail(this);
    if (al::getRailPointNum(this) <= 1) {
        kill();
        return;
    }

    al::getArg(&mMoveVelocity, info, "MoveVelocity");
    if (mMoveVelocity < 0.0f) {
        kill();
        return;
    }

    al::tryGetDisplayOffset(&mDisplayOffset, info);

    mCoins = new Coin*[mCoinNum];
    mRailPos = new f32[mCoinNum];

    if (al::isNearZero(mMoveVelocity, 0.001f)) {
        auto v12 = mCoins;
        auto v13 = mRailPos;
        auto v14 = mCoinNum;
        f32 posOnRail = 0.0f;
        f32 railDist = al::getRailTotalLength(this) / (v14 - !isLoop);
        for (int i = 0; i < v14; i++) {
            sead::Vector3f pos;
            al::calcRailPosAtCoord(&pos, this, posOnRail);
            auto coin = new Coin("コイン", false);
            al::initCreateActorWithPlacementInfo(coin, info);
            v12[i] = coin;
            coin->makeActorDead();
            al::setTrans(v12[i], pos);
            v13[i] = posOnRail;
            al::tryAddDisplayOffset(v12[i], info);
            posOnRail += railDist;
        }
    } else {
        auto v12 = mCoins;
        auto v13 = mRailPos;
        auto v14 = mCoinNum;
        f32 posOnRail = 0.0f;
        f32 railDist = 150.0f;
        for (int i = 0; i < v14; i++) {
            sead::Vector3f pos = sead::Vector3f::zero;
            al::calcRailPosAtCoord(&pos, this, posOnRail);
            auto coin = new Coin("コイン", false);
            al::initCreateActorWithPlacementInfo(coin, info);
            v12[i] = coin;
            coin->makeActorDead();
            v13[i] = posOnRail;
            al::setTrans(v12[i], pos);
            posOnRail += railDist;
            al::tryAddDisplayOffset(v12[i], info);
        }
        al::getRailTotalLength(this ? this : nullptr);
    }

    f32 shadowLength = 1500.0f;
    al::tryGetArg(&shadowLength, info, "ShadowLength");
    for (int i = 0; i < mCoinNum; i++) {
        mCoins[i]->setShadowDropLength(shadowLength);
    }

    unk22 = mCoinNum - 1;
    unk21 = 0;

    f32 clipInfo = 0.0f;
    al::calcRailClippingInfo(&mClippingInfo, &clipInfo, this, 100.0f, 100.0f);
    al::setClippingInfo(this, clipInfo, &mClippingInfo);
    al::initSubActorKeeperNoFile(this, info, mCoinNum);

    for (int i = 0; i < mCoinNum; i++) {
        al::invalidateClipping(mCoins[i]);
        al::registerSubActorSyncClipping(this, mCoins[i]);
    }

    if (isLoop)
        al::initNerve(this, &NrvCoinRail.CloseMove, 0);
    else
        al::initNerve(this, &NrvCoinRail.Move, 0);

    LiveActor::makeActorDead();
    if (!al::trySyncStageSwitchAppear(this)) {
        LiveActor::appear();
        for (int i = 0; i < mCoinNum; i++) {
            mCoins[i]->appearCoinRail();
        }
    }
    al::invalidateHitSensors(this);
}

void CoinRail::appear() {
    LiveActor::appear();
    al::startHitReaction(this, "出現");
    for (int i = 0; i < mCoinNum; i++) {
        mCoins[i]->appearCoinRail();
    }
}

void CoinRail::kill() {
    LiveActor::kill();
    for (int i = 0; i < mCoinNum; i++) {
        mCoins[i]->kill();
    }
}

void CoinRail::makeActorDead() {
    LiveActor::makeActorDead();
    for (int i = 0; i < mCoinNum; i++) {
        mCoins[i]->makeActorDead();
    }
}

bool CoinRail::receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                          al::HitSensor* target) {
    if (al::isMsgShowModel(message)) {
        for (int i = 0; i < mCoinNum; i++) {
            if (!mCoins[i]->isGot())
                al::showModelIfHide(mCoins[i]);
        }
        return true;
    }
    if (al::isMsgHideModel(message)) {
        for (int i = 0; i < mCoinNum; i++) {
            al::hideModelIfShow(mCoins[i]);
        }
        return true;
    }

    return false;
}

bool CoinRail::isGot() const {
    for (int i = 0; i < mCoinNum; i++) {
        if (!mCoins[i]->isGot())
            return false;
    }
    return true;
}

void CoinRail::exeMove() {
    if (al::isNearZero(mMoveVelocity, 0.001f))
        return;

    if (mMoveVelocity > 0.0f) {
        float v3 = al::getRailTotalLength(this);
        mRailPos[unk22] += mMoveVelocity;
        float posOf_unk22 = mRailPos[unk22];
        if (posOf_unk22 > v3) {
            posOf_unk22 = v3;
            mMoveVelocity = -mMoveVelocity;
        }

        sead::Vector3f pos = sead::Vector3f::zero;
        for (int i = unk22; i >= unk21; i--) {
            al::calcRailPosAtCoord(&pos, this, posOf_unk22);
            mRailPos[i] = posOf_unk22;
            posOf_unk22 -= 150.0f;
            al::setTrans(mCoins[i], pos + mDisplayOffset);
        }
    } else {
        mRailPos[unk21] += mMoveVelocity;
        float posOf_unk21 = mRailPos[unk21];
        if (posOf_unk21 < 0.0f) {
            posOf_unk21 = 0.0f;
            mMoveVelocity = -mMoveVelocity;
        }

        sead::Vector3f pos = sead::Vector3f::zero;
        for (int i = unk21; i <= unk22; i++) {
            al::calcRailPosAtCoord(&pos, this, posOf_unk21);
            mRailPos[i] = posOf_unk21;
            posOf_unk21 += 150.0f;
            al::setTrans(mCoins[i], pos + mDisplayOffset);
        }
    }

    for (int i = unk21; i <= unk22; i++) {
        if (!mCoins[i]->isGot()) {
            unk21 = i;
            break;
        }
    }
    for (int i = unk22; i >= unk21; i--) {
        if (!mCoins[i]->isGot()) {
            unk22 = i;
            break;
        }
    }
}

void CoinRail::exeCloseMove() {
    sead::Vector3f pos = sead::Vector3f::zero;
    for (int i = 0; i < mCoinNum; i++) {
        if (!mCoins[i]->isGot()) {
            mRailPos[i] += mMoveVelocity;
            al::calcRailPosAtCoord(&pos, this, mRailPos[i]);
            setTrans(mCoins[i], pos);
        }
    }
}
