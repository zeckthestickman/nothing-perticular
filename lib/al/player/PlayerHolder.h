#pragma once

namespace al {

class PadRumbleKeeper;
class LiveActor;

class PlayerHolder {
public:
    PlayerHolder(int);
    void clear();
    void registerPlayer(LiveActor* actor, PadRumbleKeeper* rumble_keeper);
    LiveActor* getPlayer(int index) const;
    LiveActor* tryGetPlayer(int index) const;
    int getPlayerNum() const;
    int getBufferSize() const;
    bool isFull() const;
    bool isExistPadRumbleKeeper(int index) const;
    PadRumbleKeeper* getPadRumbleKeeper(int index) const;

private:
    struct Player {
        LiveActor* mActor;
        PadRumbleKeeper* mPadRumbleKeeper;
    };

    Player* mPlayers = nullptr;
    int mBufferSize = 0;
    int mPlayerNum = 0;
};
}  // namespace al
