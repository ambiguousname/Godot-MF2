#pragma once

#include "defs.h"
#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"

#include <unicode/messageformat2_function_registry.h>

using namespace godot;

// UMemory (parent class of FormatterFactory) redefines void* operator new, so we re-re-define it again here (just copying godot-cpp's definitions):
#define OVERRIDE_ALLOC() \
static void* operator new(std::size_t p_size, const char*, const char*) { \
	return godot::Memory::alloc_static(p_size); \
} \
static void *operator new(size_t p_size, const char *p_dummy, void *(*p_allocfunc)(size_t p_size)) { \
	return p_allocfunc(p_size); \
} \
static _ALWAYS_INLINE_ void *operator new(size_t p_size, const char *p_dummy, void *p_pointer, size_t check, const char *p_description) { \
	return p_pointer; \
}

class Formatter;
class FormatterFactory : public RefCounted, public icu::message2::FormatterFactory {
	GDCLASS(FormatterFactory, RefCounted)
	protected:
		icu::message2::Formatter* createFormatter(const icu::Locale& locale, UErrorCode& status) override;

	public:
		OVERRIDE_ALLOC()

		FormatterFactory() = default;
		~FormatterFactory() override = default;

		virtual Formatter* create_formatter(String locale);
		static void _bind_methods();
};

class Formatter : public RefCounted, icu::message2::Formatter {
	GDCLASS(Formatter, RefCounted)
	public:
		OVERRIDE_ALLOC()

		Formatter() = default;
		~Formatter() override = default;

		static void _bind_methods();
};