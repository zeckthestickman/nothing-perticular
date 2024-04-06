#pragma once

#include <math/seadVector.h>

namespace nn {
namespace g3d {
class ModelObj;
class ResSkeleton;
}  // namespace g3d
}  // namespace nn

namespace al {
class ByamlIter;

struct SklAnimRetargettingInfo {
    SklAnimRetargettingInfo(const nn::g3d::ModelObj*, const nn::g3d::ModelObj*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const nn::g3d::ResSkeleton*, const nn::g3d::ResSkeleton*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const nn::g3d::ResSkeleton*, const al::ByamlIter&, const char*,
                            const sead::Vector3f&);
    SklAnimRetargettingInfo(const al::ByamlIter&, const char*, const char*, const sead::Vector3f&);
};
}  // namespace al