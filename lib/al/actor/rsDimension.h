#pragma once

namespace al {
class LiveActor;
}
class IUseDimension;
class ActorDimensionKeeper;

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* liveActor);
void updateDimensionKeeper(ActorDimensionKeeper*);

bool is2D(const IUseDimension*);
bool isIn2DArea(const IUseDimension*);
bool isChange2D(const IUseDimension*);
bool isChange3D(const IUseDimension*);
bool is3D(const IUseDimension*);
void snap2D(const al::LiveActor*, const IUseDimension*, float);

}  // namespace rs
