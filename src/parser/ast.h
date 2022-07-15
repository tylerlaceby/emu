#ifndef EMU_AST_
#define EMU_AST_

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "../emu.h"

#define DEPTH_FACTOR 2

typedef enum class NodeType {
    Program,
    VariableDeclaration,
    CallExression,
    BinaryOperation,
    BooleanLiteral,
    Null,
    Identifier,
    NumericLiteral,
} NodeType;


struct Node {
    NodeType type;
};

static void print_ast (Node* s, int depth);
static void rprint (const char* chars, int n);

struct Statement : public Node {};
struct Expression : public Statement {};

struct Program : public Node {
    std::vector<Node*> body;

    Program () {
        type = NodeType::Program;
        body = std::vector<Node*>();
    }
}; 

struct CallExpression : public Expression {

    std::vector<Expression*> args;
    std::string name;

    CallExpression (std::string n, std::vector<Expression*> a) {
        type = NodeType::CallExression;
        args = a;
        name = n;
    }

    void print (int depth) {
        rprint(" ", depth);
        printf("CallExpression: {\n");

        rprint(" ", depth + DEPTH_FACTOR);
        printf("calle: %s\n", name.c_str());
        
        rprint(" ", depth + DEPTH_FACTOR);
        printf("args: [\n", name.c_str());

        for (int a = 0; a < args.size(); a++) 
            print_ast(args[a], depth + DEPTH_FACTOR * 2);
        

        rprint(" ", depth + DEPTH_FACTOR);
        printf("], \n", name.c_str());
        
        rprint(" ", depth);
        printf("}, \n");
    }

};

struct LiteralExpression : public Expression {};

enum BinaryOp {
    IsEquals, 
    Add,
    Subtract,
    Divide,
    Multiply,
};

struct BinaryExpression : public Expression {
    BinaryOp op;
    Expression* left;
    Expression* right;
    
    BinaryExpression (Expression* l, BinaryOp o, Expression* r) {
        if (l == nullptr || r == nullptr)
            emu::error("Invalid Pointer Access\n", "BinaryOperation () parameters cannot be nullptr\n");
        
        left = l;
        right = r;
        op = o;
        type = NodeType::BinaryOperation;  
    }

    void print (int depth) {
        std::string binop = "UndefinedBinop";

        if (op == BinaryOp::IsEquals)
            binop = "==";
        else if (op == BinaryOp::Add)
            binop = "add";
        else if (op == BinaryOp::Subtract)
            binop = "subtract";
        else if (op == BinaryOp::Multiply)
            binop = "multiply";
        else if (op == BinaryOp::Divide)
            binop = "divide";

        rprint(" ", depth);
        printf("BinaryOperation: {\n");

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("left: {\n");
        print_ast(left, DEPTH_FACTOR * 3 + depth);
        rprint(" ", depth + DEPTH_FACTOR * 2);

        printf("}, \n");

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("operator: %s,\n", binop.c_str());
        
        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("right: {\n");
        print_ast(right, DEPTH_FACTOR * 3 + depth);
        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("}, \n");

        rprint(" ", depth);
        printf("}, \n");
    }
};


struct NumericLiteral : public LiteralExpression {
    long double value;
    NumericLiteral (long double num) {
        value = num;
        type = NodeType::NumericLiteral;
    }

    void print () {
        printf("Numeric: " KGRN "%Lf \n" RST, value);
    }
};

struct BooleanLiteral : public LiteralExpression {
    bool value;
    BooleanLiteral (bool b) {
        value = b;
        type = NodeType::BooleanLiteral;
    }

    void print () {
        printf("Boolean: ");
        if (value)
            std::cout << F_MAGENTA("true \n");
        else std::cout << F_MAGENTA("false \n");
    }
};


struct Null : public LiteralExpression {
    Null () {
        type = NodeType::Null;
    }

    void print () {
       std::cout << "Null: " << F_YELLOW ("null\n");
    }
};


struct Identifier : public LiteralExpression {
    std::string value;
    Identifier (std::string symbol) {
        value = symbol;
        type = NodeType::Identifier;
    }

    void print () {
        printf("Identifier: " KGRN " %s \n" RST, value.c_str());
    }
};

static void print_ast (Node* s, int depth) {
    switch (s->type) {
    case NodeType::Program:
        printf("\nProgram:\n");
        printf("body: [\n");
        for (auto stmt : ((Program*)s)->body) {
            rprint(" ", 2);
            print_ast(stmt, DEPTH_FACTOR + depth);
        }
        printf("]\n");
        printf("\n");
        break;

    case NodeType::BooleanLiteral:
        rprint(" ", depth + DEPTH_FACTOR);
        ((BooleanLiteral*)s)->print();
        break;

    case NodeType::Identifier:
        rprint(" ", depth + DEPTH_FACTOR);
        ((Identifier*)s)->print();
        break;

    case NodeType::Null:
        rprint(" ", depth + DEPTH_FACTOR);
        ((Null*)s)->print();
        break;

    case NodeType::NumericLiteral:
        rprint(" ", depth + DEPTH_FACTOR);
        ((NumericLiteral*)s)->print();
        break;
    case NodeType::BinaryOperation:
        ((BinaryExpression*)s)->print(depth);
        break;
    case NodeType::CallExression:
        ((CallExpression*)s)->print(DEPTH_FACTOR + depth);
        break;

    default:
        printf("Unimplimented printing operation - ");
        printf(" enum=%d\n", (int)s->type);
        break;
    }

}


static void rprint (const char* c, int n) {
    for (int i = 0; i < n; i++)
        printf("%s", c);
}


#endif