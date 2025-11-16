#pragma once

#include "defs.h"
#include "function_registry.h"

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

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
		String locale;
		FunctionRegistry* registry;

	public:
		friend class MessageFormatter;
		MessageFormatterBuilder();
		~MessageFormatterBuilder();

		void set_pattern(const String pattern);
		String get_pattern() const;

		// TODO: Replace with actual Locale wrapper (but also String setter function).
		void set_locale(const String locale);
		String get_locale() const;

		void set_function_registry(FunctionRegistry* registry);
		FunctionRegistry* get_function_registry() const;

		MessageFormatter* build();
		// void setLocale(const String language);
};