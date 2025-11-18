#pragma once

#include "icu_util.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include "formatter.h"

#include <unicode/messageformat2_function_registry.h>

using namespace godot;

class FunctionRegistry;
class FunctionRegistryBuilder : public RefCounted {
	GDCLASS(FunctionRegistryBuilder, RefCounted)

	protected:
		static void _bind_methods();

		UErrorCode error = U_ZERO_ERROR;
		icu::message2::MFFunctionRegistry::Builder inner;
	public:
		FunctionRegistryBuilder();
		~FunctionRegistryBuilder() override = default;

		FunctionRegistry* build();

		void adopt_formatter(String function_name, FormatterFactory* formatter_factory);
};

class MessageFormatterBuilder;

class FunctionRegistry : public RefCounted {
	GDCLASS(FunctionRegistry, RefCounted)

	protected:
		std::optional<icu::message2::MFFunctionRegistry> inner;
	public:
		static void _bind_methods();

		friend class MessageFormatterBuilder;
		friend class FunctionRegistryBuilder;
		FunctionRegistry() = default;
		~FunctionRegistry() override = default;
};