#include "message_formatter.h"

void MessageFormatterBuilder::_bind_methods() {
	ClassDB::bind_method(D_METHOD("build"), &MessageFormatterBuilder::build);

	ClassDB::bind_method(D_METHOD("set_pattern"), &MessageFormatterBuilder::set_pattern);
	ClassDB::bind_method(D_METHOD("get_pattern"), &MessageFormatterBuilder::get_pattern);
	ADD_PROPERTY(PropertyInfo(godot::Variant::STRING, "pattern"), "set_pattern", "get_pattern");

	ClassDB::bind_method(D_METHOD("set_locale"), &MessageFormatterBuilder::set_locale);
	ClassDB::bind_method(D_METHOD("get_locale"), &MessageFormatterBuilder::get_locale);
	ADD_PROPERTY(PropertyInfo(godot::Variant::STRING, "locale"), "set_locale", "get_locale");

	ClassDB::bind_method(D_METHOD("set_function_registry"), &MessageFormatterBuilder::set_function_registry);
	ClassDB::bind_method(D_METHOD("get_function_registry"), &MessageFormatterBuilder::get_function_registry);
	ADD_PROPERTY(PropertyInfo(godot::Variant::OBJECT, "function_registry"), "set_function_registry", "get_function_registry");
}

MessageFormatterBuilder::~MessageFormatterBuilder() {
	if (registry != nullptr) {
		registry->unreference();
	}
}

String MessageFormatterBuilder::get_pattern() const {
	return pattern;
}

void MessageFormatterBuilder::set_pattern(const String string_pattern) {
	pattern = string_pattern;
	icu::UnicodeString unicode_str = STRING_TO_UNICODE(pattern);

	UParseError parse_error;
	UErrorCode error_code = U_ZERO_ERROR;
	inner.setPattern(unicode_str, parse_error, error_code);

	if (error_code > U_ZERO_ERROR) {
		// String pre_context = String(parse_error.preContext);
		// String post_context = String(parse_error.postContext);
		print_error(vformat("Could not parse: \n%s\nAt line %d, offset %d: %s", string_pattern, parse_error.line, parse_error.offset, getError(error_code)));
	}
}

String MessageFormatterBuilder::get_locale() const {
	return locale;
}

void MessageFormatterBuilder::set_locale(const String str_locale) {
	locale = str_locale;
	PackedByteArray a = str_locale.to_utf8_buffer();
	// TODO: Fix to be more robust.
	icu::Locale icu_locale((const char*)a.ptr(), "", "");


	inner.setLocale(icu_locale);
}

void MessageFormatterBuilder::set_function_registry(FunctionRegistry* p_registry) {
	registry = p_registry;
	// Safety: decremented by ~MessageFormatterBuilder:
	registry->reference();
	if (registry->inner.has_value()) {
		inner.setFunctionRegistry(registry->inner.value());
	} else {
		print_error("FunctionRegistry must be created with FunctionRegistryBuilder.build().");
	}
}

FunctionRegistry* MessageFormatterBuilder::get_function_registry() const {
	return registry;
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
	// Stupid hack to allocate our MessageFormatter (since my compiler is complaining about the move assignment being a copy assignment?).
	// Safety: owned by MessageFormatter*, freed on ~MessageFormatter.
	icu::message2::MessageFormatter* formatter_ptr_v = (icu::message2::MessageFormatter*) malloc(sizeof(icu::message2::MessageFormatter));
	*formatter_ptr_v = std::move(formatter);

	// Safety: reference counting starts immediately when we return.
	MessageFormatter* f = memnew(MessageFormatter);
	f->inner = formatter_ptr_v;
	return f;
}

void MessageFormatter::_bind_methods() {
	ClassDB::bind_method(D_METHOD("format_to_string", "arg_map"), &MessageFormatter::format_to_string);
}

String MessageFormatter::format_to_string(Dictionary args) {
	if (inner == nullptr) {
		print_error("MessageFormatter needs to be created with MessageFormatterBuilder.");
		return String();
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
		return String();
	}

	icu::UnicodeString str = inner->formatToString(m_args, error_code);

	if (error_code > U_ZERO_ERROR) {
		print_error(vformat("Could not format string: %s", getError(error_code)));
		return String();
	}

	String out;
	out.parse_utf16(str.getBuffer(), str.length());
	return out;
}