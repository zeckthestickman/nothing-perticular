#pragma once

namespace al {
class LiveActor;
}
class In2DAreaMoveControl;

class ActorDimensionKeeper {
public:
    ActorDimensionKeeper(const al::LiveActor*);
    void validate();
    void invalidate();

    const al::LiveActor* mLiveActor;                      // 0x00
    bool isValid = true;                                  // 0x08
    bool is2D = false;                                    // 0x09
    bool isIn2DArea = false;                              // 0x0A
    bool isCurrently2D = false;                           // 0x0B
    bool isCanChange2D = false;                           // 0x0C
    bool isCanChange3D = false;                           // 0x0D
    char padding_2[0x2];                                  // 0x0E
    In2DAreaMoveControl* mIn2DAreaMoveControl = nullptr;  // 0x10
};