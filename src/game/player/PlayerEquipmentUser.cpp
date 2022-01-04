#include "game/player/PlayerEquipmentUser.h"

#include "al/sensor/SensorMsg.h"

#include "rs/Sensor.h"

PlayerEquipmentUser::PlayerEquipmentUser(PlayerInput const* input,
                                         PlayerModelHolder const* modelHolder,
                                         IPlayerModelChanger const* modelChanger)
    : mPlayerInput(input), mPlayerModelHolder(modelHolder), mPlayerModelChanger(modelChanger) {}

void PlayerEquipmentUser::startEquip(al::HitSensor* equipmentSensor, al::HitSensor* sourceSensor,
                                     EquipmentInfo const* info) {
    mSourceSensor = sourceSensor;
    mEquipmentSensor = equipmentSensor;
    mEquipmentInfo = info;
}

void PlayerEquipmentUser::cancelEquip() {
    al::sendMsgPlayerReleaseEquipment(mEquipmentSensor, mSourceSensor);
}

void PlayerEquipmentUser::endEquip() {
    mEquipmentSensor = nullptr;
    mEquipmentInfo = nullptr;
    mSourceSensor = nullptr;
}

void PlayerEquipmentUser::noticeDamage() {
    rs::sendMsgNoticePlayerDamage(mEquipmentSensor, mSourceSensor);
}
