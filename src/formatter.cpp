#include "formatter.h"

icu::message2::Formatter* FormatterFactory::createFormatter(const icu::Locale& locale, UErrorCode& status) {
	String str_locale = locale.getName();
	Formatter* f = create_formatter(str_locale);

	if (f == nullptr) {
		status = U_MF_FORMATTING_ERROR;
		return nullptr;
	} else {
		// ICU will need this Formatter* ptr for as long as FormatterFactory is attached to an MFFunctionRegistry. 
		// To simplify, we just reference until ~FormatterFactory:
		f->reference();
		created_formatters.push_back(f);

		status = U_ZERO_ERROR;
		return f;
	}
}

FormatterFactory::~FormatterFactory() {
	for (auto f : created_formatters) {
		f->unreference();
	}
}

Formatter* FormatterFactory::create_formatter(String locale) {
	return nullptr;
}

void FormatterFactory::_bind_methods() {
	BIND_VIRTUAL_METHOD(FormatterFactory, create_formatter, 0);
}

icu::message2::FormattedPlaceholder Formatter::format(icu::message2::FormattedPlaceholder&& toFormat, icu::message2::FunctionOptions&& options, UErrorCode& status) const {
	FormattedPlaceholder* wrap = memnew(FormattedPlaceholder());
	wrap->inner = std::move(toFormat);

	Dictionary dict_options{};
	icu::message2::FunctionOptionsMap options_map = options.getOptions();
	for (auto opt : options_map) {
		icu::UnicodeString opt_name_unicode = opt.first;
		String option_name = String::utf16(opt_name_unicode.getBuffer(), opt_name_unicode.length());
		Variant option_value = formattable_to_variant(opt.second);
		dict_options[option_name] = option_value;
	}

	String out = format_input(wrap, dict_options);

	return icu::message2::FormattedPlaceholder(toFormat, std::move(options), icu::message2::FormattedValue(STRING_TO_UNICODE(out)));
}

String Formatter::format_input(FormattedPlaceholder* to_format, Dictionary options) const {
	return "";
}

void Formatter::_bind_methods() {
	BIND_VIRTUAL_METHOD(Formatter, format_input, 0);
}