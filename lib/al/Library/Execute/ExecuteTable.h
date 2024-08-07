#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ExecuteOrder;

struct ExecuteTable {
    const char* mName = nullptr;
    const ExecuteOrder* mExecuteOrders = nullptr;
    s32 mExecuteOrderCount = 0;
};
}  // namespace al
