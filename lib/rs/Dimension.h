#pragma once

namespace al {
class LiveActor;
}
class IUseDimension;
class ActorDimensionKeeper;

namespace rs {

ActorDimensionKeeper* createDimensionKeeper(const al::LiveActor* live_actor);
void updateDimensionKeeper(ActorDimensionKeeper* actor_dimension_keeper);

bool is2D(const IUseDimension* dimension);
bool isIn2DArea(const IUseDimension* dimension);
bool isChange2D(const IUseDimension* dimension);
bool isChange3D(const IUseDimension* dimension);
bool is3D(const IUseDimension* dimension);
void snap2D(const al::LiveActor* live_actor, const IUseDimension* dimension, float);

}  // namespace rs
