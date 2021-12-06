#include "al/objects/FireDrum2D/FireDrum2D.h"

#include "al/actor/rsDimension.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/NerveUtil.h"
#include "al/util/OtherUtil.h"

FireDrum2D::FireDrum2D(const char* actorName)
    : LiveActor(actorName) {}  // TODO minor mismatch about storing `gap`

void FireDrum2D::init(const al::ActorInitInfo& actorInitInfo) {
    al::initActor(this, actorInitInfo);
    al::initNerve(this, &nrvWait, 0);
    rs::updateDimensionKeeper(mActorDimensionKeeper = rs::createDimensionKeeper(this));

    if (rs::isIn2DArea(this)) {
        rs::snap2D(this, this, 500.0f);
        makeActorAlive();
    } else {
        makeActorDead();
    }
}

ActorDimensionKeeper* FireDrum2D::getActorDimensionKeeper() const {
    return mActorDimensionKeeper;
}

void FireDrum2D::exeWait() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Burn");
    }
}

void FireDrum2D::exeBurn() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
    }
    if (al::isActionEnd(this)) {
        al::setNerve(this, &nrvWait);
    }
}

void FireDrum2D::attackSensor(al::HitSensor* source, al::HitSensor* target) {
    if (rs::sendMsgTouchFireDrum2D(target, source) || rs::sendMsgEnemyAttack2D(target, source)) {
        al::setNerve(this, &nrvBurn);
    }
}

bool FireDrum2D::receiveMsg(const al::SensorMsg* sensorMsg, al::HitSensor*, al::HitSensor*) {
    return al::isMsgPlayerDisregard(sensorMsg);
}

namespace {
NERVE_IMPL(FireDrum2D, Wait)
NERVE_IMPL(FireDrum2D, Burn)
}  // namespace