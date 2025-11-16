#include "function_registry.h"

FunctionRegistryBuilder::FunctionRegistryBuilder() : inner(error) {

}

void FunctionRegistryBuilder::_bind_methods() {
    ClassDB::bind_method(D_METHOD("build"), &FunctionRegistryBuilder::build);
}

FunctionRegistry* FunctionRegistryBuilder::build() {
    icu::message2::MFFunctionRegistry registry = inner.build();

    FunctionRegistry* out = memnew(FunctionRegistry);
    out->inner = std::move(registry);
    return out;
}
void FunctionRegistry::_bind_methods() {

}