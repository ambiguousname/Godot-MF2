#include "example_class.h"

// MF2 defs:
#define U_SHOW_CPLUSPLUS_API 1
#define UCONFIG_NO_MF2 0
#include <unicode/messageformat2.h>

using namespace icu;
using namespace message2;


void ExampleClass::_bind_methods() {
	godot::ClassDB::bind_method(D_METHOD("print_type", "variant"), &ExampleClass::print_type);
}

void ExampleClass::print_type(const godot::Variant &p_variant) const {
    UErrorCode errorCode = U_ZERO_ERROR;
    UParseError parseError;

    MessageFormatter::Builder builder(errorCode);
    UnicodeString pattern = "Hello, {$userName}!";
    MessageFormatter mf =
        builder.setPattern(pattern, parseError, errorCode)
               .build(errorCode);

    std::map<UnicodeString, message2::Formattable> argMap;
    argMap["userName"] = message2::Formattable("John");
    MessageArguments args(argMap, errorCode);

    UnicodeString result = mf.formatToString(args, errorCode);

	int32_t size = result.length();
	char* c_str = (char*)malloc(sizeof(char) * size);
	result.extract(0, size, c_str, size, 0);

    String buffer(c_str);
	::free(c_str);

	print_line(vformat("Type: %s", buffer));
}
