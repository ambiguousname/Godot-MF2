#include "function_registry.h"

FunctionRegistryBuilder::FunctionRegistryBuilder() : inner(error) {

}

void FunctionRegistryBuilder::_bind_methods() {

}

FunctionRegistry* FunctionRegistryBuilder::build() {
    icu::message2::MFFunctionRegistry registry = inner.build();

    ::godot::_pre_initialize<FunctionRegistry>();
    FunctionRegistry* out = new ("", "") FunctionRegistry(std::move(registry));
    ::godot::_post_initialize(out);
    return out;
}

FunctionRegistry::FunctionRegistry(icu::message2::MFFunctionRegistry&& registry) : inner(std::move(registry)) {

}