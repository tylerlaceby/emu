#ifndef EMU_AST_
#define EMU_AST_

#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "../emu.h"

// Used to define the growth rate for the print function.
#define DEPTH_FACTOR 2


typedef enum class NodeType {
    Program,
    VariableDeclaration,
    UnaryOperation,
    FunctionDeclaration,
    ObjectExpression,
    MemberExpression,
    ObjectProperty,
    VariableAssignment,
    BlockStatement,
    CallExression,
    BinaryOperation,
    BooleanLiteral,
    Null,
    Identifier,
    NumericLiteral,
} NodeType;

// The base AST Node. Expression, Statement and all other nodes are derived from this
struct Node {
    NodeType type;
};

// Method used to help print out the ast nodes for debugging purposes.
static void print_ast (Node* s, int depth);
// Repeats a character n amount of times in the console.
static void rprint (const char* chars, int n);

struct Statement : public Node {};
struct Expression : public Statement {};

// The program class contains the list of statements used throught the program.
struct Program : public Node {
    std::vector<Statement*> body;

    Program () {
        type = NodeType::Program;
        body = std::vector<Statement*>();
    }
}; 


struct BlockStatement : public Statement {
    std::vector<Statement*> body;
    BlockStatement () {
        type = NodeType::BlockStatement;
        body = std::vector<Statement*>();
    }

    void print (int depth) {

        rprint(" ", depth);
        printf("BlockStatement: \n");

        for (auto elem : body)
            print_ast(elem, depth + DEPTH_FACTOR);

        printf("\n");        
    }

};

struct FunctionDeclaration : public Statement {
    std::string identifier;
    std::vector<std::string> params;
    BlockStatement* body;

    FunctionDeclaration (std::string ident, BlockStatement* b,std::vector<std::string> p) {
        body = b;
        params = p;
        identifier = ident;
        type = NodeType::FunctionDeclaration;
    }

    void print (int depth) {

        rprint(" ", depth);
        printf("FunctionDeclaration: \n");

        rprint(" ", depth + DEPTH_FACTOR);
        printf("name:\x1B[32m %s\n" RST, identifier.c_str());

        rprint(" ", depth + DEPTH_FACTOR);
        printf("args: [ ");

        for (int a = 0; a < params.size(); a++) {
            printf(KCYN "%s" RST, params[a].c_str());
            if (a < params.size() - 1)
                printf(", ");
            else printf(" ");
        }

        printf("]\n");

        print_ast(body, depth);     
    }
};

struct VariableDeclaration : public Statement {

    std::string identifier;
    Expression* value;
    bool constDeclaration;

    VariableDeclaration (std::string n, Expression* v, bool cnst) {
        identifier = n;
        constDeclaration = cnst;
        value = v;
        type = NodeType::VariableDeclaration;
    }

    void print (int depth) {

        rprint(" ", depth);
        printf("VariableDeclaration: \n");

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("identifier:\x1B[32m %s\n" RST, identifier.c_str());

        rprint(" ", depth + DEPTH_FACTOR * 2);
        // add magenta coloring as well as rst
        printf("const: \x1B[35m%s \n" RST, std::string(constDeclaration? "true" : "false").c_str());
        
        print_ast(value, depth + DEPTH_FACTOR * 2);             

        printf("\n");
    }
};

struct VariableAssignment : public Expression {

    Expression* assigne;
    Expression* value;

    VariableAssignment (Expression* assigneExpr, Expression* v) {
        assigne = assigneExpr;
        value = v;
        type = NodeType::VariableAssignment;
    }

    void print (int depth) {

        rprint(" ", depth);
        printf("AssignmentExpression: \n");

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("assigne: \n");
        print_ast (assigne, depth + DEPTH_FACTOR * 3);

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("value: \n");
        print_ast(value, depth + DEPTH_FACTOR * 3);           

        printf("\n");
    }

};

struct CallExpression : public Expression {

    std::vector<Expression*> args;
    Expression* calle;

    CallExpression (Expression* _c, std::vector<Expression*> _a) {
        type = NodeType::CallExression;
        args = _a;
        calle = _c;
    }

    void print (int depth) {
        rprint(" ", depth);
        printf("CallExpression: \n");

        rprint(" ", depth + DEPTH_FACTOR);
        printf("\x1B[32mcalle:\n" RST);
        print_ast(calle, depth + DEPTH_FACTOR * 2);
        
        rprint(" ", depth + DEPTH_FACTOR);
        printf("args: \n");

        for (int a = 0; a < args.size(); a++) 
            print_ast(args[a], depth + DEPTH_FACTOR * 2);

        printf("\n");
        
    }

};


struct MemberExpression : public Expression {
    Expression* member;
    Expression* property;

    MemberExpression (Expression* mem, Expression* prop) {
        type = NodeType::MemberExpression;
        member = mem;
        property = prop;
    }

    void print (int depth) {

        rprint(" ", depth);
        printf("MemberExpression:\n");

        rprint(" ", depth + DEPTH_FACTOR);
        printf("member:\n");
        print_ast(member, depth + DEPTH_FACTOR * 2);

        rprint(" ", depth + DEPTH_FACTOR);
        printf("property:\n");
        print_ast(property, depth + DEPTH_FACTOR * 2);

    }

};

struct ObjectProperty : public Expression {
    std::string key;
    bool shorthand;
    Expression* rhs;

    ObjectProperty (std::string k, bool sh, Expression* v) {
        rhs = v;
        shorthand = sh;
        key = k;
        type = NodeType::ObjectProperty;
    }

    void print (int depth) {
        rprint(" ", depth);
        printf("Property:\n");

        rprint(" ", depth + DEPTH_FACTOR);
        printf("key:\x1B[32m %s\n" RST, key.c_str());

        rprint(" ", depth + DEPTH_FACTOR);
        printf("shorthand:\x1B[36m %s\n" RST, shorthand? "true":"false");

        if (!shorthand)
            print_ast(rhs, depth + DEPTH_FACTOR);

        printf("\n");
    }
};

struct ObjectExpression : public Expression {
    std::vector<ObjectProperty*> properties;

    ObjectExpression(std::vector<ObjectProperty*> props) {
        properties = props;
        type = NodeType::ObjectExpression;
    }

    void print (int depth) {
        rprint(" ", depth);
        printf("ObjectExpression: \n");
        for (auto prop : properties)
            print_ast(prop, depth + DEPTH_FACTOR);

        printf("\n");
    }
};


struct LiteralExpression : public Expression {};

enum BinaryOp {
    Or, And,
    IsEquals, 
    NotEquals,
    LessThan,
    GreaterThan,
    LessThanEq,
    GreaterThanEq,
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

        if (op == BinaryOp::Or)
            binop = "or";
        else if (op == BinaryOp::And)
            binop = "and";
        else if (op == BinaryOp::IsEquals)
            binop = "==";
        else if (op == BinaryOp::NotEquals)
            binop = "!=";
        else if (op == BinaryOp::LessThan)
            binop = "<";
        else if (op == BinaryOp::LessThanEq)
            binop = "<=";
        else if (op == BinaryOp::GreaterThan)
            binop = ">";
        else if (op == BinaryOp::GreaterThanEq)
            binop = ">=";
        else if (op == BinaryOp::Add)
            binop = "add";
        else if (op == BinaryOp::Subtract)
            binop = "subtract";
        else if (op == BinaryOp::Multiply)
            binop = "multiply";
        else if (op == BinaryOp::Divide)
            binop = "divide";

        rprint(" ", depth);
        printf("BinaryOperation: \n");

        // rprint(" ", depth + DEPTH_FACTOR * 2);
        // printf("left: \n");
        print_ast(left, DEPTH_FACTOR * 2 + depth);


        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("operator:\x1B[33m %s\n" RST, binop.c_str());
        
        // rprint(" ", depth + DEPTH_FACTOR * 2);
        // printf("right: \n");
        print_ast(right, DEPTH_FACTOR * 2 + depth);

        printf("\n");
    }
};

enum UnaryOp {
    Inverse,
    Not,
};

struct UnaryExpression : public Expression {
    UnaryOp op;
    Expression* right;
    
    UnaryExpression (UnaryOp o, Expression* r) {
        right = r;
        op = o;
        type = NodeType::UnaryOperation;
    }

    void print (int depth) {
        std::string uop = "UndefinedUnaryOperation";

        if (op == UnaryOp::Inverse)
            uop = "inverse";
        else if (op == UnaryOp::Not)
            uop = "not";

        rprint(" ", depth);
        printf("UnaryOperation: \n");

        rprint(" ", depth + DEPTH_FACTOR * 2);
        printf("operator:\x1B[33m %s\n" RST, uop.c_str());

        print_ast(right, DEPTH_FACTOR * 2 + depth);

        printf("\n");
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
       std::cout << F_YELLOW ("null\n");
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
        for (auto stmt : ((Program*)s)->body) {
            print_ast(stmt, 1);
        }
        printf("\n");
        break;

    case NodeType::BooleanLiteral:
        rprint(" ", depth);
        ((BooleanLiteral*)s)->print();
        break;

    case NodeType::Identifier:
        rprint(" ", depth);
        ((Identifier*)s)->print();
        break;

    case NodeType::Null:
        rprint(" ", depth);
        ((Null*)s)->print();
        break;

    case NodeType::NumericLiteral:
        rprint(" ", depth);
        ((NumericLiteral*)s)->print();
        break;
    case NodeType::BinaryOperation:
        ((BinaryExpression*)s)->print(depth);
        break;
    case NodeType::UnaryOperation:
        ((UnaryExpression*)s)->print(depth + DEPTH_FACTOR);
        break;
    case NodeType::CallExression:
        ((CallExpression*)s)->print(DEPTH_FACTOR + depth);
        break;

    case NodeType::VariableDeclaration:
        ((VariableDeclaration*)s)->print(depth + DEPTH_FACTOR);
        break;
    case NodeType::VariableAssignment:
        ((VariableAssignment*)s)->print(depth + DEPTH_FACTOR);
        break;

    case NodeType::BlockStatement:
        ((BlockStatement*)s)->print(depth + DEPTH_FACTOR);
        break;

    case NodeType::FunctionDeclaration:
        ((FunctionDeclaration*)s)->print(depth + DEPTH_FACTOR);
        break;

    case NodeType::ObjectExpression:
        ((ObjectExpression*)s)->print(depth + DEPTH_FACTOR);
        break;

    case NodeType::ObjectProperty:
        ((ObjectProperty*)s)->print(depth + DEPTH_FACTOR);
        break;
    case NodeType::MemberExpression:
        ((MemberExpression*)s)->print(depth + DEPTH_FACTOR);
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