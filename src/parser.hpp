#pragma once
#include "lexer.hpp"
#include <vector>


// Forward Declarations
struct Expression;
struct PrefixExpression;
struct InfixExpression;
struct IfExpression;
struct FunctionLiteral;
struct CallExpression;
struct IntegerLiteral;
struct FloatLiteral;
struct StringLiteral;
struct BooleanLiteral;
struct IdentifierLiteral;

struct Statement;
struct LetStatement;
struct IdentifierStatement;
struct FunctionStatement;
struct ReturnStatement;
struct ExpressionStatement;
struct BlockStatement;
struct AssignmentExpressionStatement;


class Parser {
  public:
    // Attributes
    Token currentToken;
    Token peekToken;
    int linenumber{1};
    std::vector<std::string> errors;

    // Constructors
    Parser(std::string input) {
      this->lexer = new Lexer(input);

      // reading two tokens so currentToken and peekToken both get set
      this->nextToken();
      this->nextToken();
    }

    ~Parser() {
      delete this->lexer;
    }

    // Methods
    void nextToken();
    bool expectPeek(std::string);
    void peekErrors(std::string);
    Statement* parseStatement();
  private:
    // Attributes
    Lexer* lexer;

    // Methods
    int currentPrecedence();
    int peekPrecedence();
    void checkIdentifierDataType(IdentifierStatement*);
    void checkFunctionReturn(FunctionStatement*);
    void checkFunctionReturnDataType(ReturnStatement*);

    // Expression Methods
    Expression* parseExpression(int);
    Expression* parseLeftPrefix(int);
    Expression* parseGroupedExpression();
    AssignmentExpressionStatement* parseAssignmentExpression();
    IfExpression* parseIfExpression();
    FunctionLiteral* parseFunctionLiteral();
    std::vector<IdentifierLiteral*> parseFunctionParameters();
    CallExpression* parseCallExpression(Expression*);
    std::vector<Expression*> parseCallArguments();
    PrefixExpression* parsePrefixExpression();
    InfixExpression* parseInfixExpression(Expression*);
    IdentifierLiteral* parseIdentifier();
    IntegerLiteral* parseIntegerLiteral();
    FloatLiteral* parseFloatLiteral();
    StringLiteral* parseStringLiteral();
    BooleanLiteral* parseBooleanLiteral();

    // Statement Methods
    LetStatement* parseLetStatement();
    BlockStatement* parseBlockStatement();
    ReturnStatement* parseReturnStatement();
    IdentifierStatement* parseIdentifierStatement();
    FunctionStatement* parseFunctionStatement();
    ExpressionStatement* parseExpressionStatement();
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
  PREFIX_INCREMENT,
  PREFIX_DECREMENT,
  PREFIX_GROUPED_EXPR,
  PREFIX_ASSIGN
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
  {TokenType.INCREMENT, PREFIX_INCREMENT},
  {TokenType.DECREMENT, PREFIX_DECREMENT},
  {TokenType.LPAREN, PREFIX_GROUPED_EXPR},
  {TokenType.IF, PREFIX_IF},
  {TokenType.FUNCTION, PREFIX_FUNCTION},
  {TokenType.PLUS_EQ, PREFIX_ASSIGN},
  {TokenType.MINUS_EQ, PREFIX_ASSIGN},
  {TokenType.MULT_EQ, PREFIX_ASSIGN},
  {TokenType.DIV_EQ, PREFIX_ASSIGN},
};


// Infix Functions
enum infix {
  INFIX_STD,
  INFIX_CALL,
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
};

