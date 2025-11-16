#pragma once

#include "defs.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include <unicode/messageformat2_function_registry.h>

using namespace godot;

class Formatter;
class FormatterFactory : public RefCounted, public icu::message2::FormatterFactory {
	GDCLASS(FormatterFactory, RefCounted)
	protected:
		virtual icu::message2::Formatter* createFormatter(const icu::Locale& locale, UErrorCode& status) override;

	public:
		// Formatter* create_formatter();
		// static void _bind_methods();
};

class Formatter : public RefCounted, public icu::message2::Formatter {
	public:
		// static void _bind_methods();
};