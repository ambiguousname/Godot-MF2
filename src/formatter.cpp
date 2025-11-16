#include "formatter.h"

icu::message2::Formatter* FormatterFactory::createFormatter(const icu::Locale& locale, UErrorCode& status) {
	status = U_MF_FORMATTING_ERROR;
	return nullptr;
}