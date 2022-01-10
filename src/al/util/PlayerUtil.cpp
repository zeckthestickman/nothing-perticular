#include "al/util/PlayerUtil.h"

#include "al/player/PlayerHolder.h"
#include "al/util/LiveActorUtil.h"
#include "al/util/OtherUtil.h"

namespace al {

int getPlayerNumMax(const PlayerHolder* holder) {
    return holder->getPlayerNum();
}

int getAlivePlayerNum(const PlayerHolder* holder) {
    int player_num = holder->getPlayerNum();

    if (player_num < 1) {
        return 0;
    }

    int alive_players = 0;
    int current_player = 0;

    do {
        LiveActor* player = holder->tryGetPlayer(current_player);
        if (isAlive(player)) {
            alive_players++;
        }

        current_player++;
    } while (current_player < player_num);

    return alive_players;
}

LiveActor* getPlayerActor(const PlayerHolder* holder, int index) {
    return holder->tryGetPlayer(index);
}

#ifdef NON_MATCHING
sead::Vector3f* getPlayerPos(const PlayerHolder* holder, int index) {
    // The game's version uses two BL instructions in this function, but this code always
    // compiles to BL, LDP, then B.
    LiveActor* player = holder->tryGetPlayer(index);
    sead::Vector3f* pos = getTrans(player);

    return pos;
}
#endif

LiveActor* tryGetPlayerActor(const PlayerHolder* holder, int index) {
    return holder->tryGetPlayer(index);
}

bool isPlayerDead(const PlayerHolder* holder, int index) {
    LiveActor* player = holder->tryGetPlayer(index);
    return isDead(player);
}

bool isPlayerAreaTarget(const PlayerHolder* holder, int index) {
    LiveActor* player = holder->tryGetPlayer(index);
    return isAreaTarget(player);
}

LiveActor* tryFindAlivePlayerActorFirst(const PlayerHolder* holder) {
    u32 player_num = holder->getPlayerNum();

    if (player_num == 0) {
        return nullptr;
    }

    for (u32 i = 0; i < player_num; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (!isDead(player)) {
            return player;
        }
    }

    return nullptr;
}

LiveActor* findAlivePlayerActorFirst(const PlayerHolder* holder) {
    u32 player_num = holder->getPlayerNum();

    if (player_num == 0) {
        return nullptr;
    }

    for (u32 i = 0; i < player_num; i++) {
        LiveActor* player = holder->tryGetPlayer(i);
        if (!isDead(player)) {
            return player;
        }
    }

    return nullptr;
}
}  // namespace al
