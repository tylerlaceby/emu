#ifndef EMU_AST_
#define EMU_AST_

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "../emu.h"


typedef enum class NodeType {
    Program,
    NumericLiteral,
    BooleanLiteral,
    Null,
    Identifier
} NodeType;


struct Node {
    NodeType type;
};

struct Program : public Node {
    std::vector<Node*> body;

    Program () {
        type = NodeType::Program;
        body = std::vector<Node*>();
    }
}; 


struct NumericLiteral : public Node {
    long double value;
    NumericLiteral (long double num) {
        value = num;
        type = NodeType::NumericLiteral;
    }

    void print () {
        printf(KGRN " %Lf " RST, value);
    }
};

struct BooleanLiteral : public Node {
    bool value;
    BooleanLiteral (bool b) {
        value = b;
        type = NodeType::BooleanLiteral;
    }

    void print () {
        if (value)
            std::cout << F_BLUE(" true ");
        else std::cout << F_BLUE(" false ");
    }
};

struct Null : public Node {
    Null () {
        type = NodeType::Null;
    }

    void print () {
       std::cout << F_BLUE (" null");
    }
};


struct Identifier : public Node {
    std::string value;
    Identifier (std::string symbol) {
        value = symbol;
        type = NodeType::Identifier;
    }

    void print () {
        printf(KYEL " %s " RST, value.c_str());
    }
};


/*
struct ListExpr : public Node {
    std::vector<Node*> list;

    ListExpr () {
        type = NodeType::LIST;
        list = std::vector<Node *>();
    }

    void print (int space = 0) {
        char spaceChar = ' ';
        repeatPrint(spaceChar, space);
        if (list.size() == 0)
            F_YELLOW("(  )\n");
        else 
            printf(KRED "List: \n" RST);
        
        for (int i = 0; i < list.size(); i++) {
            repeatPrint(spaceChar, space + 2);
            auto expr = list[i];
            switch (expr->type) {
            case NodeType::LIST: 
                ((ListExpr*)expr)->print(space + 2);
                break;
            case NodeType::NUMBER: 
                repeatPrint(spaceChar, space + 2);
                printf("value:");
                ((NumericLiteral*)expr)->print();
                break;
            case NodeType::SYMBOL: 
                repeatPrint(spaceChar, space + 2);
                printf("value:");
                ((Symbol*)expr)->print();
                break;
            case NodeType::NULLISH: 
                repeatPrint(spaceChar, space + 2);
                printf("value:");
                ((Null*)expr)->print();
                break;
            case NodeType::BOOLEAN: 
                repeatPrint(spaceChar, space + 2);
                printf("value:");
                ((BooleanLiteral*)expr)->print();
                break;
            
            default:
                printf("Cannot print unknown NodeType: %d\n", (int)expr->type);
                break;
            }

            printf("\n");
        }
    }
};

*/

#endif