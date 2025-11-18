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
	Variant virtual_in = Variant(nullptr);
	if (toFormat.canFormat()) {
		icu::message2::Formattable formattable = toFormat.asFormattable();
		virtual_in = formattable_to_variant(formattable);
	}

	Dictionary dict_options{};
	icu::message2::FunctionOptionsMap options_map = options.getOptions();
	for (auto opt : options_map) {
		icu::UnicodeString opt_name_unicode = opt.first;
		String option_name = String::utf16(opt_name_unicode.getBuffer(), opt_name_unicode.length());
		Variant option_value = formattable_to_variant(opt.second);
		dict_options[option_name] = option_value;
	}

	Variant out = format_input(virtual_in, dict_options);
	icu::message2::Formattable formattable_out = variant_to_formattable(out);
	// return icu::message2::FormattedPlaceholder(toFormat, std::move(options), formattable_out); 
	return {};
}

Variant Formatter::format_input(Variant to_format, Dictionary options) const {
	return to_format;
}

void Formatter::_bind_methods() {
	BIND_VIRTUAL_METHOD(Formatter, format_input, 0);
}