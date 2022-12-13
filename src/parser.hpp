#pragma once
#include "lexer.hpp"
#include <vector>


// Forward Declarations
struct ArrayLiteral;
struct BooleanLiteral;
struct CallExpression;
struct DoExpression;
struct Expression;
struct FloatLiteral;
struct ForExpression;
struct FunctionLiteral;
struct HashLiteral;
struct IdentifierLiteral;
struct IfExpression;
struct IndexExpression;
struct InfixExpression;
struct IntegerLiteral;
struct PostfixExpression;
struct PrefixExpression;
struct StringLiteral;
struct WhileExpression;

struct AssignmentExpressionStatement;
struct BlockStatement;
struct ExpressionStatement;
struct FunctionStatement;
struct IdentifierStatement;
struct LetStatement;
struct ReturnStatement;
struct Statement;


class Parser {
  public:
    Parser(std::string);
    ~Parser();

    Token currentToken;
    Token peekToken;
    int linenumber{1};
    std::vector<std::string> errors;

    bool expectPeek(std::string);
    void nextToken();
    Statement* parseStatement();
    void peekErrors(std::string);
  private:
    Lexer* lexer;

    void checkIdentifierDataType(IdentifierStatement*);
    void checkFunctionReturn(FunctionStatement*);
    void checkFunctionReturnDataType(ReturnStatement*);
    int currentPrecedence();
    int peekPrecedence();

    ArrayLiteral* parseArrayLiteral();
    AssignmentExpressionStatement* parseAssignmentExpression();
    BooleanLiteral* parseBooleanLiteral();
    CallExpression* parseCallExpression(Expression*);
    DoExpression* parseDoExpression();
    Expression* parseExpression(int);
    std::vector<Expression*> parseExpressionList(std::string);
    FloatLiteral* parseFloatLiteral();
    ForExpression* parseForExpression();
    FunctionLiteral* parseFunctionLiteral();
    std::vector<IdentifierLiteral*> parseFunctionParameters();
    Expression* parseGroupedExpression();
    HashLiteral* parseHashLiteral();
    IdentifierLiteral* parseIdentifier();
    IfExpression* parseIfExpression();
    Expression* parseIndexExpression(Expression*);
    InfixExpression* parseInfixExpression(Expression*);
    IntegerLiteral* parseIntegerLiteral();
    Expression* parseLeftPrefix(int);
    PostfixExpression* parsePostfixExpression(Expression*);
    PrefixExpression* parsePrefixExpression();
    StringLiteral* parseStringLiteral();
    WhileExpression* parseWhileExpression();

    BlockStatement* parseBlockStatement();
    ExpressionStatement* parseExpressionStatement();
    FunctionStatement* parseFunctionStatement();
    IdentifierStatement* parseIdentifierStatement();
    LetStatement* parseLetStatement();
    ReturnStatement* parseReturnStatement();
};


// Prefix Functions
enum prefix {
  PREFIX_STD,
  PREFIX_IDENT,
  PREFIX_INT,
  PREFIX_FLOAT,
  PREFIX_STRING,
  PREFIX_BOOL,
  PREFIX_IF,
  PREFIX_FUNCTION,
  PREFIX_DO,
  PREFIX_WHILE,
  PREFIX_FOR,
  PREFIX_INCREMENT,
  PREFIX_DECREMENT,
  PREFIX_GROUPED_EXPR,
  PREFIX_ASSIGN,
  PREFIX_ARRAY,
  PREFIX_HASH,
};


const std::unordered_map<std::string, int> prefixFunctions = {
  {TokenType.IDENT, PREFIX_IDENT},
  {TokenType.INT, PREFIX_INT},
  {TokenType.FLOAT, PREFIX_FLOAT},
  {TokenType._STRING, PREFIX_STRING},
  {TokenType.BANG, PREFIX_STD},
  {TokenType.MINUS, PREFIX_STD},
  {TokenType._TRUE, PREFIX_BOOL},
  {TokenType._FALSE, PREFIX_BOOL},
  {TokenType.LPAREN, PREFIX_GROUPED_EXPR},
  {TokenType.IF, PREFIX_IF},
  {TokenType.FUNCTION, PREFIX_FUNCTION},
  {TokenType.DO, PREFIX_DO},
  {TokenType.WHILE, PREFIX_WHILE},
  {TokenType.FOR, PREFIX_FOR},
  {TokenType.PLUS_EQ, PREFIX_ASSIGN},
  {TokenType.MINUS_EQ, PREFIX_ASSIGN},
  {TokenType.MULT_EQ, PREFIX_ASSIGN},
  {TokenType.DIV_EQ, PREFIX_ASSIGN},
  {TokenType.LBRACKET, PREFIX_ARRAY},
  {TokenType.LBRACE, PREFIX_HASH},
};


// Infix Functions
enum infix {
  INFIX_STD,
  INFIX_CALL,
  INFIX_INDEX,
};


const std::unordered_map<std::string, int> infixFunctions = {
  {TokenType.PLUS, INFIX_STD},
  {TokenType.MINUS, INFIX_STD},
  {TokenType.SLASH, INFIX_STD},
  {TokenType.ASTERISK, INFIX_STD},
  {TokenType.EQ, INFIX_STD},
  {TokenType.NOT_EQ, INFIX_STD},
  {TokenType.LT, INFIX_STD},
  {TokenType.GT, INFIX_STD},
  {TokenType.LPAREN, INFIX_CALL},
  {TokenType.LBRACKET, INFIX_INDEX},
};


enum postfix {
  POSTFIX
};

const std::unordered_map<std::string, int> postfixFunctions = {
  {TokenType.INCREMENT, POSTFIX},
  {TokenType.DECREMENT, POSTFIX},
};
