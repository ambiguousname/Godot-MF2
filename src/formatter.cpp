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

void Formatter::_bind_methods() {

}