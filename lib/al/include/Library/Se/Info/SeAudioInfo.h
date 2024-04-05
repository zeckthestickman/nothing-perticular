#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <prim/seadSafeString.h>

#include "Library/Yaml/ByamlIter.h"

namespace al {
class AudioInfoListWithParts;

struct SeUserInfo {
    static SeUserInfo* createInfo(const ByamlIter&, const sead::SafeString&);

    SeUserInfo();
    ~SeUserInfo();

    bool isIncludingSePartsUser(const SeUserInfo*);

    static s32 compareInfo(const SeUserInfo*, const SeUserInfo*);

    const char* mName;
    bool mIsPartsUser = false;
    sead::PtrArray<char*> mPartsUserNameList;
    AudioInfoListWithParts* mEmitterInfoList;
    AudioInfoListWithParts* mActionInfoList;
    AudioInfoListWithParts* mPlayInfoList;
    AudioInfoListWithParts* mHitReactionInfoList;
};
}  // namespace al
