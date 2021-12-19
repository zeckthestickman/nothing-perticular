#pragma once

namespace al {
class LiveActor;
}
class In2DAreaMoveControl;

class ActorDimensionKeeper {
public:
    ActorDimensionKeeper(const al::LiveActor* live_actor);
    void validate();
    void invalidate();
    ActorDimensionKeeper* forceChange2DKeep();
    ActorDimensionKeeper* forceEndChange2DKeep();
    bool update();

    bool getIs2D() { return is2D; }
    bool getIsIn2DArea() { return isIn2DArea; }
    bool getIsCurrently2D() { return isCurrently2D; }
    bool getIsCanChange2D() { return isCanChange2D; }
    bool getIsCanChange3D() { return isCanChange3D; }

private:
    const al::LiveActor* mLiveActor;
    bool isValid = true;
    bool is2D = false;
    bool isIn2DArea = false;
    bool isCurrently2D = false;
    bool isCanChange2D = false;
    bool isCanChange3D = false;
    In2DAreaMoveControl* mIn2DAreaMoveControl = nullptr;
};
