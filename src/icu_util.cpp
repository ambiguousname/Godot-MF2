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

String getError(UErrorCode error)  {
	const char* error_explainer;
	switch(error) {
		case U_ZERO_ERROR: {
			error_explainer = "No error.";
			break;
		}
		case U_ILLEGAL_ARGUMENT_ERROR: {
			error_explainer = "Illegal argument."; 
			break;
		}
		case U_MISSING_RESOURCE_ERROR: {
			error_explainer = "The requested resource cannot be found."; 
			break;
		}
		case U_FILE_ACCESS_ERROR: {
			error_explainer = "The requested file cannot be found."; 
			break;
		}
		case U_INTERNAL_PROGRAM_ERROR: {
			error_explainer = "The ICU4C library has failed in some way."; 
			break;
		}
		case U_MESSAGE_PARSE_ERROR: {
			error_explainer = "Cannot parse message."; 
			break;
		}
		case U_MEMORY_ALLOCATION_ERROR: {
			error_explainer = "Could not allocate memory."; 
			break;
		}
		case U_INDEX_OUTOFBOUNDS_ERROR: {
			error_explainer = "Trying to access index out of bounds."; 
			break;
		}
		case U_PARSE_ERROR: {
			error_explainer = "Unexpected error while parsing."; 
			break;
		}
		case U_INVALID_CHAR_FOUND: {
			error_explainer = "Invalid character found."; 
			break;
		}
		case U_TRUNCATED_CHAR_FOUND: {
			error_explainer = "Incomplete input sequence."; 
			break;
		}
		case U_INVALID_TABLE_FORMAT: {
			error_explainer = "Conversion table file found, but is corrupted."; 
			break;
		}
		case U_MF_UNRESOLVED_VARIABLE_ERROR: {
			error_explainer = "Could not find definition for variable.";
			break;
		}
		case U_MF_SYNTAX_ERROR: {
			error_explainer = "Syntax error.";
			break;
		}
		case U_MF_UNKNOWN_FUNCTION_ERROR: {
			error_explainer = "Unknown function.";
			break;
		}
		// TODO: MF errors, but somewhere else with more robust error printing.
		// Can't be bothered with everything else right now:
		default: {
			error_explainer = "Unrecognized error code."; 
			break;
		}
	}
	return vformat("%s (ICU Error Code %d)", error_explainer, error);
}