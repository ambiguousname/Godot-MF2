#include "message_formatter.h"

String getError(UErrorCode error) {
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

void MessageFormatterBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_pattern"), &MessageFormatterBuilder::set_pattern);
	ClassDB::bind_method(D_METHOD("get_pattern"), &MessageFormatterBuilder::get_pattern);
	ClassDB::bind_method(D_METHOD("build"), &MessageFormatterBuilder::build);

	ADD_PROPERTY(PropertyInfo(godot::Variant::PACKED_BYTE_ARRAY, "pattern"), "set_pattern", "get_pattern");
}

PackedByteArray MessageFormatterBuilder::get_pattern() const {
	return pattern;
}

#define STRING_TO_UNICODE(string) icu::UnicodeString::fromUTF32((const UChar32*) string.ptr(), string.length())

void MessageFormatterBuilder::set_pattern(const PackedByteArray byte_pattern) {
	pattern = byte_pattern;
	icu::UnicodeString unicode_str = icu::UnicodeString::fromUTF8(icu::StringPiece((const char*)byte_pattern.ptr(), byte_pattern.size()));

	UParseError parse_error;
	UErrorCode error_code = U_ZERO_ERROR;
	inner.setPattern(unicode_str, parse_error, error_code);

	if (error_code > U_ZERO_ERROR) {
		// String pre_context = String(parse_error.preContext);
		// String post_context = String(parse_error.postContext);
		print_error(vformat("Could not parse: \n%s\nAt line %d, offset %d: %s", byte_pattern.get_string_from_utf8(), parse_error.line, parse_error.offset, getError(error_code)));
	}
}

MessageFormatterBuilder::MessageFormatterBuilder() : inner(error) {
	if (error > U_ZERO_ERROR) {
		print_error(vformat("Error creating MessageFormatter::Builder - %s", getError(error)));
	}
}

MessageFormatter* MessageFormatterBuilder::build() {
	return MessageFormatter::from_builder(this);
}

MessageFormatter* MessageFormatter::from_builder(MessageFormatterBuilder* builder) {
	UErrorCode error_code = U_ZERO_ERROR;
	icu::message2::MessageFormatter formatter = builder->inner.build(error_code);
	if (error_code > U_ZERO_ERROR) {
		print_error(vformat("Error building MessageFormatter - %d", error_code));
		return nullptr;
	}
	// Stupid hack to allocate our MessageFormatter (since my compiler is complaining about the move assignment being a copy assignment?):
	icu::message2::MessageFormatter* formatter_ptr_v = (icu::message2::MessageFormatter*) malloc(sizeof(icu::message2::MessageFormatter));
	*formatter_ptr_v = std::move(formatter);

	MessageFormatter* f = memnew(MessageFormatter);
	f->inner = formatter_ptr_v;
	return f;
}

void MessageFormatter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("format_to_string"), &MessageFormatter::format_to_string, "args_map");
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

PackedByteArray MessageFormatter::format_to_string(Dictionary args) {
	if (inner == nullptr) {
		print_error("MessageFormatter needs to be created with MessageFormatterBuilder.");
		return PackedByteArray();
	}

	std::map<icu::UnicodeString, icu::message2::Formattable> arg_map{};

	Array keys = args.keys();
	Array values = args.values();
	for (int64_t i = 0; i < keys.size(); i++) {
		if (keys[i].get_type() == Variant::Type::STRING) {
			String key = keys[i];
			icu::message2::Formattable formatVariant = variant_to_formattable(values[i]);
			icu::UnicodeString key_u = STRING_TO_UNICODE(key);
			arg_map[key_u] = formatVariant;
		}
	}

	UErrorCode error_code = U_ZERO_ERROR;
	icu::message2::MessageArguments m_args = icu::message2::MessageArguments(arg_map, error_code);
	if (error_code > U_ZERO_ERROR) {
		print_error(vformat("Could not create argument dictionary: %s", getError(error_code)));
		return PackedByteArray();
	}

	icu::UnicodeString str = inner->formatToString(m_args, error_code);

	if (error_code > U_ZERO_ERROR) {
		print_error(vformat("Could not format string: %s", getError(error_code)));
		return PackedByteArray();
	}

	PackedByteArray out;
	out.resize(str.length());
	// Not any other super easy ways to handle this that I can think of.
	for (int32_t i = 0; i < str.length(); i++) {
		out[i] = str.charAt(i);
	}
	return out;
}