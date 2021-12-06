#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/actor/ActorDimensionKeeper.h"
#include "al/actor/ActorInitInfo.h"
#include "al/util/NerveUtil.h"
#include "game/iuse/IUseDimension.h"

class FireDrum2D : public al::LiveActor, public IUseDimension {
public:
    FireDrum2D(const char* actorName);
    virtual void init(const al::ActorInitInfo&) override;
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;

    void exeWait();
    void exeBurn();

    virtual ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    ActorDimensionKeeper* mActorDimensionKeeper;
};

namespace {
NERVE_HEADER(FireDrum2D, Wait)
NERVE_HEADER(FireDrum2D, Burn)
}  // namespace