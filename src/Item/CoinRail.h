#pragma once

#include "Library/LiveActor/LiveActor.h"

class Coin;

class CoinRail : public al::LiveActor {
public:
    CoinRail(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void makeActorDead() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* source,
                    al::HitSensor* target) override;

    bool isGot() const;

    void exeMove();
    void exeCloseMove();

private:
    Coin** mCoins = nullptr;
    f32* mRailPos = nullptr;
    s32 mCoinNum = 0;
    f32 mMoveVelocity = 0;
    s32 unk21 = 0;
    s32 unk22 = 0;
    sead::Vector3f mClippingInfo;
    sead::Vector3f mDisplayOffset = sead::Vector3f::zero;
};
