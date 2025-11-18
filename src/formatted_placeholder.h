#pragma once

#include "icu_util.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include <unicode/messageformat2_function_registry.h>

using namespace godot;

class Formatter;

class FormattedPlaceholder : public RefCounted {
	GDCLASS(FormattedPlaceholder, RefCounted)
	protected:
		icu::message2::FormattedPlaceholder inner;
	public:
		friend class Formatter;
		
		FormattedPlaceholder() = default;
		~FormattedPlaceholder() override = default;

		// void create_from(const FormattedPlaceholder* input, String value);
		static void _bind_methods();
};