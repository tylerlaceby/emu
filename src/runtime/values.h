#ifndef EMU_RUNTIME_VALUES
#define EMU_RUNTIME_VALUES
#include "../parser/ast.h"
#include "../emu.h"

////////////////////////////
//   NATIVE TYPED VALUES  //
////////////////////////////

typedef enum class ValueType {
    NUMBER,
    BOOLEAN,
    NULLISH,
    CUSTOM,
} ValueType;

///////////////////////////
//  CUSTOM TYPED VALUES  //
///////////////////////////

typedef enum class ObjectType {
    FUNCTION,
} ObjectType;

// The default class that other values will inherit from.
struct Object {
    ObjectType kind;
};



// The base return type and storage type for all complex data.
struct Value {
    ValueType type;
    union {
        long double number;
        bool boolean;
        Object* object;
    };

    Value (long double num) {
        type = ValueType::NUMBER;
        number = num;
    }

    Value (bool b) {
        type = ValueType::BOOLEAN;
        boolean = b;
    }

    // provide default type.
    Value () {
        type = ValueType::NULLISH;
    }
};

/////////////////////////////////
//   CUSTOM VALUE CONSTRUCTORS  /
/////////////////////////////////

static bool IS_CUSTOM_VAL (Value* v) { return v->type == ValueType::CUSTOM; }

///////////////////////////
//  NATIVE VALUE CHECKS  //
///////////////////////////

static bool IS_NUMBER (Value* v) { return v->type == ValueType::NUMBER; }
static bool IS_NULLISH (Value* v) { return v->type == ValueType::NULLISH; }
static bool IS_BOOLEAN (Value* v) { return v->type == ValueType::BOOLEAN; }

/////////////////////////////////
//  NATIVE VALUE CONSTRUCTORS  //
/////////////////////////////////

static Value* CREATE_NUMBER_VAL (long double num) {
    Value* value = new Value(num);
    return value;
}

static Value* CREATE_SYMBOL_VAL (long double num) {
    Value* value = new Value(num);
    return value;
}

static Value* CREATE_BOOLEAN_VAL (bool boolean) {
    Value* value = new Value(boolean);
    return value;
}

static Value* CREATE_NULL_VAL () {
    Value* value = new Value();
    value->type = ValueType::NULLISH;
    return value;
}


////////////////////////////
//  NATIVE VALUE GETTERS  //
////////////////////////////

static long double TO_NUMBER (Value* val) { return val->number; }
static bool TO_BOOLEAN (Value* val) { return val->boolean; }
static const char* TO_NULLSTR (Value* val) { return "null"; }


/////////////////////////////
//  NATIVE VALUE PRINTING  //
/////////////////////////////

static void PRINT_EMU_VALUE (Value* value) {

    if (IS_NULLISH(value))
        printf(F_BLUE("null"));
    else if (IS_BOOLEAN(value)) {
        if (TO_BOOLEAN(value))
            printf(F_BLUE("true"));
        else printf(F_BLUE("false"));
    } else if (IS_NUMBER(value))
        printf(KGRN " %Lf " RST, TO_NUMBER(value));

    else {
        emu::throw_error("Cannot print value\n");
    }

}

#endif