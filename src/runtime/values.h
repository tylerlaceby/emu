#ifndef EMU_RUNTIME_VALUES
#define EMU_RUNTIME_VALUES

#include "../emu.h"

// Represents a value at runtime for a emu value.
typedef enum class ValueType {
    NUMBER,
    BOOLEAN,
    NULLISH,
    CUSTOM,
} ValueType;

// Represents a value at runtime that cannot be held on the stack.
typedef enum class ObjectType {
    STRING,
    // FUNCTION,
    // STRUCTURE,
    // ARRAY,
} ObjectType;

// A dynamic object on the heap. can be a user defined struct, string, or array, function etc...
struct Object {
    ObjectType kind;
};

struct StrObject : public Object {
    std::string value;

    StrObject (std::string str) {
        kind = ObjectType::STRING;
        value = str;
    }
};


// The base return type and storage type for all complex data.
struct Value {
    ValueType type;
    union {
        long double number;
        bool boolean;
        Object* object;
    };
};

#endif