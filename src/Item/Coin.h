#pragma once

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActor.h"

class Coin : public al::LiveActor {
public:
    Coin(const char*, bool);

    void init(const al::ActorInitInfo&);

    void control();
    void appearCoinRail();
    bool isGot() const;

    void setShadowDropLength(f32);

private:
    void* filler[0x15];
};
