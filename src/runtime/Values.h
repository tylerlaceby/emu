#ifndef EMU_VALUE_TYPES
#define EMU_VALUE_TYPES

// Defines them labels for the types.
enum ValTypes {
    Number,     // Long doubles.
    Boolean,    // Boolean values.
    NullValue,       // User defined empty type.
    // Error,      // Defines types that handle execptions or errors. Used by try/catch blocks and runtime errs.
};

// Defines the types used in runtime for emu.
struct Val { 
    ValTypes type;

    // Determines the type of the Val.
    bool istype (ValTypes t) { return type == t; } 
};

///////////////////////////
///// EMU BOOLEAN VALUES //
///////////////////////////

struct BoolVal : public Val {
    bool boolean;

    BoolVal (bool _b) {
        boolean = _b;
        type = ValTypes::Boolean;
    }
};

//////////////////////////
///// EMU NUMBER VALUES //
//////////////////////////

struct NumberVal : public Val {
    long double number;

    NumberVal (long double _n) {
        number = _n;
        type = ValTypes::Number;
    }
};

////////////////////////
///// EMU NULL VALUES //
////////////////////////

struct NullVal : public Val {
    NullVal () { type = ValTypes::NullValue; }
};


void display_value (Val* value);

#endif