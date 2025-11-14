#include "message_formatter.h"

const char* getError(UErrorCode error) {
    switch(error) {
        case U_ZERO_ERROR:
            return "No error.";
        case U_ILLEGAL_ARGUMENT_ERROR:
            return "Illegal argument.";
        case U_MISSING_RESOURCE_ERROR:
            return "The requested resource cannot be found.";
        case U_FILE_ACCESS_ERROR:
            return "The requested file cannot be found.";
        case U_INTERNAL_PROGRAM_ERROR:
            return "The ICU4C library has failed in some way.";
        case U_MESSAGE_PARSE_ERROR:
            return "Cannot parse message.";
        case U_MEMORY_ALLOCATION_ERROR:
            return "Could not allocate memory.";
        case U_INDEX_OUTOFBOUNDS_ERROR:
            return "Trying to access index out of bounds.";
        case U_PARSE_ERROR:
            return "Unexpected error while parsing.";
        case U_INVALID_CHAR_FOUND:
            return "Invalid character found.";
        case U_TRUNCATED_CHAR_FOUND:
            return "Incomplete input sequence.";
        case U_INVALID_TABLE_FORMAT:
            return "Conversion table file found, but is corrupted.";
        // TODO: MF errors, but somewhere else with more robust error printing.
        // Can't be bothered with everything else right now:
        default:
            return "Unrecognized error code.";
    }
}

void MessageFormatterBuilder::_bind_methods() {

}

MessageFormatterBuilder::MessageFormatterBuilder() : inner(error) {
    if (error != U_ZERO_ERROR) {
        print_error(vformat("Error creating MessageFormatter::Builder - %s (%i)", getError(error), error));
    }
}

void MessageFormatter::_bind_methods() {

}