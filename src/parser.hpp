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
    ~Parser() {this->errors.clear();};

    Token currentToken;
    Token peekToken;
    int linenumber{1};
    std::vector<std::string> errors;

    bool expectPeek(std::string);
    void nextToken();
    std::shared_ptr<Statement> parseStatement();
    void peekErrors(std::string);
  private:
    std::shared_ptr<Lexer> lexer;

    void checkIdentifierDataType(std::shared_ptr<IdentifierStatement>);
    void checkFunctionReturn(std::shared_ptr<FunctionStatement>);
    void checkFunctionReturnDataType(std::shared_ptr<ReturnStatement>);
    int currentPrecedence();
    int peekPrecedence();

    std::shared_ptr<ArrayLiteral> parseArrayLiteral();
    std::shared_ptr<AssignmentExpressionStatement> parseAssignmentExpression();
    std::shared_ptr<BooleanLiteral> parseBooleanLiteral();
    std::shared_ptr<CallExpression> parseCallExpression(std::shared_ptr<Expression>);
    std::shared_ptr<DoExpression> parseDoExpression();
    std::shared_ptr<Expression> parseExpression(int);
    std::vector<std::shared_ptr<Expression>> parseExpressionList(std::string);
    std::shared_ptr<FloatLiteral> parseFloatLiteral();
    std::shared_ptr<ForExpression> parseForExpression();
    std::shared_ptr<LetStatement> parseForLetStatement();
    std::shared_ptr<FunctionLiteral> parseFunctionLiteral();
    std::vector<std::shared_ptr<IdentifierLiteral>> parseFunctionParameters();
    std::shared_ptr<Expression> parseGroupedExpression();
    std::shared_ptr<HashLiteral> parseHashLiteral();
    std::shared_ptr<IdentifierLiteral> parseIdentifier();
    std::shared_ptr<IfExpression> parseIfExpression();
    std::shared_ptr<Expression> parseIndexExpression(std::shared_ptr<Expression>);
    std::shared_ptr<InfixExpression> parseInfixExpression(std::shared_ptr<Expression>);
    std::shared_ptr<IntegerLiteral> parseIntegerLiteral();
    std::shared_ptr<Expression> parseLeftPrefix(int);
    std::shared_ptr<PostfixExpression> parsePostfixExpression(std::shared_ptr<Expression>);
    std::shared_ptr<PrefixExpression> parsePrefixExpression();
    std::shared_ptr<StringLiteral> parseStringLiteral();
    std::shared_ptr<WhileExpression> parseWhileExpression();

    std::shared_ptr<BlockStatement> parseBlockStatement();
    std::shared_ptr<ExpressionStatement> parseExpressionStatement();
    std::shared_ptr<FunctionStatement> parseFunctionStatement();
    std::shared_ptr<IdentifierStatement> parseIdentifierStatement();
    std::shared_ptr<LetStatement> parseLetStatement();
    std::shared_ptr<ReturnStatement> parseReturnStatement();
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
