#pragma once

// MF2 defs:
#define U_SHOW_CPLUSPLUS_API 1
#define UCONFIG_NO_MF2 0

#include "godot_cpp/variant/variant.hpp"
#include <unicode/messageformat2.h>

#define STRING_TO_UNICODE(string) icu::UnicodeString::fromUTF32((const UChar32*) string.ptr(), string.length())

godot::Variant formattable_to_variant(icu::message2::Formattable formattable);

icu::message2::Formattable variant_to_formattable(godot::Variant value);

godot::String getError(UErrorCode error);