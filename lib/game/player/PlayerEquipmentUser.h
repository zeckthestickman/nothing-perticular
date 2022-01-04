#pragma once

namespace al {
class HitSensor;
}

class EquipmentInfo;
class IPlayerModelChanger;
class PlayerInput;
class PlayerModelHolder;

class PlayerEquipmentUser {
public:
    PlayerEquipmentUser(PlayerInput const* input, PlayerModelHolder const* modelHolder,
                        IPlayerModelChanger const* modelChanger);

    void startEquip(al::HitSensor*, al::HitSensor*, EquipmentInfo const*);
    void cancelEquip();
    void endEquip();
    void noticeDamage();

    al::HitSensor* mSourceSensor = nullptr;
    al::HitSensor* mEquipmentSensor = nullptr;
    EquipmentInfo const* mEquipmentInfo = nullptr;
    PlayerInput const* mPlayerInput;
    PlayerModelHolder const* mPlayerModelHolder;
    IPlayerModelChanger const* mPlayerModelChanger;
    void* mUnk = nullptr;
};
