#include "al/actor/rsDimension.h"

#include "al/LiveActor/LiveActor.h"
#include "al/actor/ActorDimensionKeeper.h"
#include "game/iuse/IUseDimension.h"

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* liveActor) {
    return new ActorDimensionKeeper(liveActor);
}

bool is2D(const IUseDimension* pKeeper) {
    return pKeeper->getActorDimensionKeeper()->is2D;
}

bool is3D(const IUseDimension* pKeeper) {
    ActorDimensionKeeper* keeper = pKeeper->getActorDimensionKeeper();
    return !keeper->is2D && !keeper->isCurrently2D;
}

bool isChange2D(const IUseDimension* pKeeper) {
    return pKeeper->getActorDimensionKeeper()->isCanChange2D;
}

bool isChange3D(const IUseDimension* pKeeper) {
    return pKeeper->getActorDimensionKeeper()->isCanChange3D;
}

bool isIn2DArea(const IUseDimension* pKeeper) {
    return pKeeper->getActorDimensionKeeper()->isIn2DArea;
}

};  // namespace rs