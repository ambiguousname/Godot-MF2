#include "formatted_placeholder.h"

#include "godot_cpp/core/print_string.hpp"

// void FormattedPlaceholder::create_from(const FormattedPlaceholder* input, String value) {
// 	// TODO: Add support for passing over options.
// 	// icu::message2::FunctionOptionsMap options_map{};
// 	// Array keys = options.keys();
// 	// Array values = options.values();
// 	// for (int64_t i = 0; i < keys.size(); i++) {
// 	// 	if (keys[i].get_type() == Variant::Type::STRING) {
// 	// 		String key = keys[i];
// 	// 		icu::UnicodeString ustr = STRING_TO_UNICODE(key);
// 	// 		icu::message2::Formattable f = variant_to_formattable(values[i]);
// 	// 		options_map[ustr] = f;
// 	// 	}
// 	// }

// 	// TODO: Add support for formattedNumber.
	
// 	// FIXME: Add reference/unreference of input->inner
// 	inner = icu::message2::FormattedPlaceholder(input->inner, icu::message2::FormattedValue(STRING_TO_UNICODE(value)));
// }

void FormattedPlaceholder::_bind_methods() {
	// ClassDB::bind_method(D_METHOD("create_from"), &FormattedPlaceholder::create_from);
}