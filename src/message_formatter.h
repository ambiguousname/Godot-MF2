#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

// MF2 defs:
#define U_SHOW_CPLUSPLUS_API 1
#define UCONFIG_NO_MF2 0
#include <unicode/messageformat2.h>

using namespace godot;

class MessageFormatterBuilder;

class MessageFormatter : public RefCounted {
	GDCLASS(MessageFormatter, RefCounted)

	protected:
		static void _bind_methods();

		icu::message2::MessageFormatter* inner = nullptr;
	public:
		MessageFormatter() = default; 
		~MessageFormatter() override {
			::free(inner);
		}

		static MessageFormatter* from_builder(MessageFormatterBuilder* builder);

		String format_to_string(Dictionary args);

};

class MessageFormatterBuilder : public RefCounted {
	GDCLASS(MessageFormatterBuilder, RefCounted)

	protected:
		static void _bind_methods();
		UErrorCode error = U_ZERO_ERROR;
		icu::message2::MessageFormatter::Builder inner;

		String pattern;

	public:
		friend class MessageFormatter;
		MessageFormatterBuilder();
		~MessageFormatterBuilder() override = default;

		void set_pattern(const String pattern);
		String get_pattern() const;

		MessageFormatter* build();
		// TODO: Replace with actual Locale wrapper.
		// void setLocale(const String language);
};