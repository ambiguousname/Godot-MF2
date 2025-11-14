#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

// MF2 defs:
#define U_SHOW_CPLUSPLUS_API 1
#define UCONFIG_NO_MF2 0
#include <unicode/messageformat2.h>

using namespace godot;

class MessageFormatterBuilder : public RefCounted {
    GDCLASS(MessageFormatterBuilder, RefCounted)

    protected:
        static void _bind_methods();
        UErrorCode error = U_ZERO_ERROR;
        icu::message2::MessageFormatter::Builder inner;

        PackedByteArray pattern;

    public:
        MessageFormatterBuilder();
        ~MessageFormatterBuilder() override = default;

        void set_pattern(const PackedByteArray pattern);
        PackedByteArray get_pattern() const;
        // TODO: Replace with actual Locale wrapper.
        // void setLocale(const String language);
};

class MessageFormatter : public RefCounted {
    GDCLASS(MessageFormatter, RefCounted)

    protected:
        static void _bind_methods();
    public:
        MessageFormatter() = default;
        ~MessageFormatter() override = default;

};