#include "icu_util.h"

using namespace godot;
#include "godot_cpp/core/print_string.hpp"

Variant formattable_to_variant(icu::message2::Formattable formattable) {
	UErrorCode status = U_ZERO_ERROR;
	// Never check the status for any of these, since we've checked the type ahead of time.
	switch (formattable.getType()) {
		case UFMT_DOUBLE: {
			return formattable.getDouble(status);
		}
		case UFMT_LONG: {
			return formattable.getLong(status);
		}
		case UFMT_STRING: {
			icu::UnicodeString str = formattable.getString(status);
			return String::utf16(str.getBuffer(), str.length());
		}
		case UFMT_INT64: {
			return formattable.getInt64Value(status);
		}
		default: {
			print_error(vformat("Currently unsupported Formattable type %d", formattable.getType()));
			return nullptr;
		}
	}
}

icu::message2::Formattable variant_to_formattable(Variant value) {
	switch (value.get_type()) {
		case Variant::Type::INT: {
			return icu::message2::Formattable((int64_t)value);
		}
		case Variant::Type::STRING: {
			String str_v = value;
			return icu::message2::Formattable(STRING_TO_UNICODE(str_v));
		}
		case Variant::Type::FLOAT: {
			return icu::message2::Formattable((double)value);
		}
		// case Variant::Type::ARRAY: {
		//     Array values = value;
		//     if (values.size() > std::numeric_limits<int32_t>::max) {
		//         print_error(vformat("Array is of size %d, which exceeds int32_t's maximum size (icu's Formattable type can only be indexed up to %d)", values.size(), std::numeric_limits<int32_t>::max));
		//         return icu::message2::Formattable();
		//     }
		//     icu::message2::Formattable* formatArr = new icu::message2::Formattable[values.size()];
		//     for (int64_t i = 0; i < values.size(); i++) {
		//         Variant v = values[i];
		//         formatArr[i] = variant_to_formattable(v);
		//     }

		//     icu::message2::Formattable arr(formatArr, values.size());
		//     // TODO: This does not work since Formattable does not copy over the array.
		//     delete formatArr;
		//     return arr;
		// }
		default: {
			return icu::message2::Formattable();
		}
	}
}