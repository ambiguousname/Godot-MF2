#include "function_registry.h"

#include "godot_cpp/core/print_string.hpp"

FunctionRegistryBuilder::FunctionRegistryBuilder() : inner(error) {

}

void FunctionRegistryBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("adopt_formatter", "function_name", "formatter_factory"), &FunctionRegistryBuilder::adopt_formatter);

	ClassDB::bind_method(D_METHOD("build"), &FunctionRegistryBuilder::build);
}

void FunctionRegistryBuilder::adopt_formatter(String function_name, FormatterFactory* formatter_factory) {
	UErrorCode status = U_ZERO_ERROR;
	inner.adoptFormatter(STRING_TO_UNICODE(function_name), formatter_factory, status);
	if (status > U_ZERO_ERROR) {
		print_error(vformat("Could not adopt formatter: %s", getError(status)));
	}
}

FunctionRegistry* FunctionRegistryBuilder::build() {
	icu::message2::MFFunctionRegistry registry = inner.build();

	// Safety: Reference counting starts immediately as soon as we return.
	FunctionRegistry* out = memnew(FunctionRegistry);
	out->inner = std::move(registry);
	return out;
}

void FunctionRegistry::_bind_methods() {

}