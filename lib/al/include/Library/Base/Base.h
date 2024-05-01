#pragma once

#include <basis/seadTypes.h>
#include <cctype>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace al {
s32 calcHashCode(char const* hash);
s32 calcHashCodeLower(char const* hash);
s32 calcHashCodeFmt(char const* format, std::va_list argv);
s32 calcHashCodeFmt(char const* format, ...);
const char* getBaseName(char const* name);
}  // namespace al