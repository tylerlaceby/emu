#include "Values.h"
#include "../lib/colors.h"
#include "stdio.h"
#include "iostream"

void display_value (Val* val) {

    if (val->istype(ValTypes::Number)) 
        printf(KGRN "%Lf" RST, ((NumberVal*)val)->number);

    else if (val->istype(ValTypes::Boolean)) { 
        bool b = ((BoolVal*)val)->boolean;
        printf(KMAG "%s" RST, b ? "true" : "false");
    }

    else if (val->istype(ValTypes::NullValue)) 
        printf(KYEL "null" RST );

    else {
        // Handle fallback.
        printf("Unimplimented print value function Values.cpp\nENUM TYPE=%d\n", (int)val->type);
    }
}