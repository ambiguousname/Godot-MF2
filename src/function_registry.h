#pragma once

#include "defs.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

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
};

class FunctionRegistry : public RefCounted {
    GDCLASS(FunctionRegistry, RefCounted)

    protected:
        icu::message2::MFFunctionRegistry inner;
    public:
        FunctionRegistry(icu::message2::MFFunctionRegistry&& registry);
        ~FunctionRegistry() override = default;
};